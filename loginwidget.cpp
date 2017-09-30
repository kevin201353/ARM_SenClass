#include "loginwidget.h"
#include "myui_loginwidget.h"
#include "include.h"
#include "global.h"
//#include "testnet.h"
#include <QDate>
#include <QDesktopWidget>
#include "qthread.h"
//#include "buildtime.h"
#include "type.h"
#include "netlink.h"
#include <semaphore.h>

extern char buildtime[32];
extern NetConfig   *g_pNetConfig;
extern bool g_bshowwaitstu;
extern volatile bool g_bSetupAmq;
extern int g_msgid;
static pthread_t g_spicypid = NULL;

LoginWidget * g_loginWnd = NULL;
void *InitThread(void *param);
static pthread_t g_xtid = 0;
static char g_szCmd[1024] = {0};
static bool g_ExitMsThread = false;
bool   g_processThread = false;
MqMsgProcess  g_mqMsgProcess;
static pthread_t g_amqpid = NULL;
pthread_mutex_t g_hreadMutex;

pthread_mutex_t g_freestudyMutex;

bool  g_bExit_freeStuy_flag = false;
extern char g_szRetVm[1024];
extern bool  g_spicyProcessExit;
char   g_szLocalIP[MAX_IP_SIZE] = {0};
extern waitstu2  *m_waitstu;

static void *thrd_exec(void *param)
{
    PIPE_SPICYLOG(g_szCmd);
    system(g_szCmd);
    return NULL;
}

#ifdef ARM
extern unsigned long long g_interval_time;
#else
extern long g_interval_time;
#endif

static void *MonitorSpicy(void *param)
{
    while(true)
    {
        if (g_ExitMsThread)
            break;
        if (g_bgetserTime)
        {
            myHttp http;
            http.SetUrlIP(g_strServerIP);
            char data[100] = {0};
            char json[100] = {0};
            bool bjson = false;
            sprintf(data,"%s","/service/aps/timestamp");
            http.Get(data);
            http.GetData(json);
            g_pLog->WriteLog(0,"student client get timestamp http return data:%s", json);
            g_pJson->Parse(json);
            g_pJson->ReadJson(&bjson, "success");
            if (bjson)
            {
                memset(data, 0, 100);
                g_pJson->ReadJson_v(data, "data", "systemTime");
                g_pLog->WriteLog(0,"student client recv server time:%s", data);
                g_interval_time = atoll(data);
                g_pLog->WriteLog(0,"student client recv server time atoll = :%lld", g_interval_time);
                //g_interval_time = (unsigned long long)abs(g_interval_time - __GetTime());
                g_interval_time = g_interval_time - __GetTime();
                g_pLog->WriteLog(0,"student client recv server time g_interval_time = :%lld,  _GetTime = %lld", g_interval_time, __GetTime());
            }
        }
        sleep(15);
    }
    pthread_detach(pthread_self());
    return NULL;
}

//heart process heart
#ifdef ARM 
    long long st_heart_time = 0;
#else
    long st_heart_time = 0;
#endif
#define  HEART_MAX_COUNT  2

int    g_check_heart_flag = 0;
LoginWidget::LoginWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginWidget)
{
    ui->setupUi(this);
    this->setAutoFillBackground(true);
    this->setWindowFlags(Qt::Window);
    g_loginWnd = NULL;
    g_loginWnd = this;
#if 0
    int scr_width = QApplication::desktop()->width();
    int scr_height = QApplication::desktop()->height();
    float factor_x = (float)scr_width/g_scr_old_width;
    float factor_y = (float)scr_height/g_scr_old_height;
    this->m_width = scr_width;
    this->m_height = scr_height;
    this->resize(width(), height());
    my_resize(this, factor_x, factor_y);
#endif
    InitMyMutex();
    m_pLogoQLable = ui->Logolabel;
    m_pLogoQLable->setAlignment(Qt::AlignCenter);
    //m_pLogoQLable->setPixmap(QPixmap(LOGOPNG));
    m_pLogoQLable2 = ui->label_Logo;
    m_pLogoQLable2->setAlignment(Qt::AlignCenter);
    m_pLogoQLable2->setPixmap(QPixmap(LOGOPNG2));
    m_pTimeLCDNumber = ui->TimeLcdNumber;
    m_pSoftInforLabel = ui->Informationlabel;
    m_pServerManPushButton = ui->ServerManpushButton;
    m_pSetPushButton = ui->SetpushButton;
    m_pInformationPushButton = ui->InformationpushButton;
    m_pShutdownPushButton = ui->ShutdownpushButton;
    m_pLeftPushButton = ui->LeftpushButton;
    m_pRightPushButton = ui->RightpushButton;
    m_pEnterPushButton = ui->EnterpushButton;
    m_pEnterPushButton->setVisible(true);
    //hide informations
    m_pSoftInforLabel->show();
    m_pSetPushButton->setEnabled(false);
    m_pSetPushButton->setVisible(false);
    m_pShutdownPushButton->setEnabled(false);
    m_pShutdownPushButton->setVisible(false);
    m_pTimeLCDNumber->setEnabled(false);
    m_pTimeLCDNumber->setVisible(false);
    //hide end
    m_pClassName1 = ui->label_1;
    m_pClassName2 = ui->label_2;
    m_pClassName3 = ui->label_3;

    //setTabOrder(m_pLoginPushButton,m_pUserNameLineEdit);

    m_pRightPushButton->setMinimumSize(34,59);
    m_pRightPushButton->setFlat(false);
   // m_pRightPushButton->setStyleSheet(QStringLiteral(RIGHTPNG));
    m_pRightPushButton->setStyleSheet(RIGHTPNG);


    m_pLeftPushButton->setMinimumSize(34,59);
    m_pLeftPushButton->setFlat(false);
    //m_pLeftPushButton->setStyleSheet(QStringLiteral(LEFTPNG));
    m_pLeftPushButton->setStyleSheet(LEFTPNG);
    // m_pLeftPushButton->setText("左翻");

    m_pTimeLCDNumber->setDigitCount(25);
    m_pTimeLCDNumber->setMode(QLCDNumber::Dec);
    m_pTimeLCDNumber->setSegmentStyle(QLCDNumber::Flat);
    m_pTimeLCDNumber->setFrameShape(QLCDNumber::NoFrame);

    m_pSetPushButton->setStyleSheet(QStringLiteral(SETLOGO));
    m_pSetPushButton->setMinimumSize(37,36);
//    m_pSetPushButton->setFlat(false);
    m_pInformationPushButton->setStyleSheet(QStringLiteral(INFORMATIONLOGO));
    m_pInformationPushButton->setMinimumSize(39,39);

    m_pServerManPushButton->setStyleSheet(QStringLiteral(SERVERMANLOGO));
    m_pServerManPushButton->setMinimumSize(34,34);

    m_pShutdownPushButton->setStyleSheet(QStringLiteral(SHUTDOWNLOGO));
    m_pShutdownPushButton->setMinimumSize(33,37);
   // QFont ft;
    m_pTimer = new QTimer(this);
    m_pTimer->setInterval(1000);
    m_pTime = new QDateTime();
    m_pTimer->start();
    connect(m_pTimer,SIGNAL(timeout()),this,SLOT(OnTimeOut()));
    SetEnable(false);
    m_pClassNameConfig = new ClassNameConfig();
    m_pClassNameConfig->AddLabel(m_pClassName1);
    m_pClassNameConfig->AddLabel(m_pClassName2);
    m_pClassNameConfig->AddLabel(m_pClassName3);
    m_pClassName1->SetCheckedFlag(true);

    //m_pEnterPushButton->setMinimumSize(284,66);
   //m_pEnterPushButton->setEnabled(false);
    widget_resize();
    QFont font;
    font.setPixelSize(30);
//    QPalette palette = m_pEnterPushButton->palette();
//    palette.setColor(QPalette::Text, QColor(255,255,255));
//    m_pEnterPushButton->setPalette(palette);
    m_pEnterPushButton->setFont(font);
    m_pEnterPushButton->setText("进入");
    m_pEnterPushButton->show();
    //170303
    //test
    //m_pGroupWigdet->setEnabled(true);
    //test end
    connect(m_pLeftPushButton,SIGNAL(clicked()),this,SLOT(on_LeftPushButton()));
    connect(m_pRightPushButton,SIGNAL(clicked()),this,SLOT(on_RightPushButton()));
    connect(m_pClassName1,SIGNAL(LabelChecked()),this,SLOT(on_LabelChecked()));
    connect(m_pClassName2,SIGNAL(LabelChecked()),this,SLOT(on_LabelChecked()));
    connect(m_pClassName3,SIGNAL(LabelChecked()),this,SLOT(on_LabelChecked()));
    connect(m_pEnterPushButton,SIGNAL(clicked()),this,SLOT(on_EnterPushButton()));
    connect(m_pClassName1,SIGNAL(LabelDoubleclicked()),this,SLOT(on_LableDoubleClicked()));
    connect(m_pClassName2,SIGNAL(LabelDoubleclicked()),this,SLOT(on_LableDoubleClicked()));
    connect(m_pClassName3,SIGNAL(LabelDoubleclicked()),this,SLOT(on_LableDoubleClicked()));
    m_passui = new PasswordUI();
    connect(m_passui, SIGNAL(ShowPassUI()),this,SLOT(on_ShowPassUI()));
    connect(m_waitstu, SIGNAL(wait_showPassUI()), this, SLOT(on_SetpushButton_clicked()));
    connect(m_waitstu, SIGNAL(wait_shutdown()), this, SLOT(on_ShutdownpushButton_clicked()));

	//heart pthread mutex
	pthread_mutex_init(&g_hreadMutex, NULL);
	pthread_mutex_init(&g_freestudyMutex, NULL);
    g_pNetConfig = new NetConfig();
    if(g_pJson == NULL)
    {
        g_pJson = new myJson();
    }
#if 1
    if(g_pMyHttp == NULL)
    {
        g_pMyHttp = new myHttp();
        g_pMyHttp->SetUrlIP(QString(g_strServerIP));
    }
#endif

    g_ExitMsThread = false;
    g_processThread = false;
	g_bExit_freeStuy_flag = false;
    g_bshowwaitstu = false;
    g_spicyProcessExit = false;
    g_mqMsgProcess.start();
    g_bSetupAmq = false;
    m_pqthread = NULL;
    m_pqthread = new qthread();
    connect(m_pqthread, SIGNAL(NoticeShow()), this, SLOT(UpdateNetOffDialog()));
    connect(m_pqthread, SIGNAL(NoticeHide()), this, SLOT(HideNetOffDialog()));
    m_pqthread->start();
    m_pSetRoomSeat = new SetRoomSeatDialog(this);
    m_bExitThread = false;
    m_pMyDialog = new MyDialog();
    m_pSetForm = new SetForm();
    m_pSetForm->move(QApplication::desktop()->width()/2 - m_pSetForm->width()/2, QApplication::desktop()->height()/2 - m_pSetForm->height()/2);
    m_pServerManPushButton->hide();
    m_pInformationPushButton->hide();
//    QDate *date = new QDate();
//    QDate date2 = date->currentDate();
//    QString strdate = date2.toString("yyyyMMdd");
//    QString strsoft = m_pSoftInforLabel->text();
//    strsoft += ": V1.5_";
//    strsoft += new QString(buildtime);
//    m_pSoftInforLabel->setText(strsoft);
//    if (date)
//    {
//        delete date;
//        date = NULL;
//    }
     QString strsoft(g_szLocalIP);
     m_pSoftInforLabel->setText(strsoft);
#if 1
#ifdef ARM
    if(pthread_create(&g_spicypid,NULL,MonitorSpicy,this))
    {
    }
#endif
#endif

#if 0
    if(pthread_create(&g_amqpid,NULL,InitThread,this))
    {
    }
#endif

#if 0
    SetEnable(true);
    m_pClassNameConfig->m_iClassNum = 0;
    m_pClassNameConfig->AddClass("win_64bit", "dad5e8f5-beb4-4a5b-85fa-468de06262d4");
    m_pClassNameConfig->AddClass("win7_32","dad5e8f5-beb4-4a5b-85fa-468de06262d4");
    m_pClassNameConfig->AddClass("winxp_32bit","c3831381-ca4b-4090-81e0-5128b8983b22");
    m_pClassNameConfig->ChooseOne();
    m_pClassNameConfig->SetLabelName();
#endif

    g_mqMsgProcess.startMulticast();
#if 1
    g_mqMsgProcess.strart_spicyThrd();
#endif
    char szVer[100] = {0};
    strcpy(szVer, "V1.1_");
    strcat(szVer, (char*)buildtime);
    WriteConfigString(CONFIGNAME, "ROOM", "Version", szVer);
}

void LoginWidget::SetEnable(bool flag)
{
    m_pLeftPushButton->setEnabled(flag);
    m_pRightPushButton->setEnabled(flag);
    m_pEnterPushButton->setEnabled(flag);
    m_pClassName1->setEnabled(flag);
    m_pClassName2->setEnabled(flag);
    m_pClassName3->setEnabled(flag);
}

void *InitThread(void *param)
{
    if (param == NULL)
        return NULL;
    LoginWidget *pLoginWidget = (LoginWidget *)param;
#if 1
    while(1)
    {
        if (g_ExitMsThread)
            break;
        if (g_bgetserTime)
        {
            myHttp http;
            http.SetUrlIP(g_strServerIP);
            char data[100] = {0};
            char json[100] = {0};
            bool bjson = false;
            sprintf(data,"%s","/service/aps/timestamp");
            http.Get(data);
            http.GetData(json);
            g_pLog->WriteLog(0,"student client get timestamp http return data initthread:%s", json);
            g_pJson->Parse(json);
            g_pJson->ReadJson(&bjson, "success");
            if (bjson)
            {
                memset(data, 0, 100);
                g_pJson->ReadJson_v(data, "data", "systemTime");
                g_pLog->WriteLog(0,"InitThread student client recv server time:%s", data);
                g_interval_time = atoll(data);
                g_pLog->WriteLog(0,"student client recv server time atoll = :%lld", g_interval_time);
                //g_interval_time = (unsigned long long)abs(g_interval_time - __GetTime());
                g_interval_time = g_interval_time - __GetTime();
                g_pLog->WriteLog(0,"student client recv server time g_interval_time = :%lld,  _GetTime = %lld", g_interval_time, __GetTime());
            }
        }
        usleep(1000);
    }
#endif
    pthread_detach(pthread_self());
    return NULL;
}
void *WhileFun(void *param)
{
    if (param == NULL)
        return NULL;

    LoginWidget *p = (LoginWidget *)param;
        p->Thread();
}

void LoginWidget::SetChecked()
{
    m_pClassName1->SetCheckedFlag(true);
}
void LoginWidget::Thread()
{
    char JsonBuf[1024];
    bool Recode = false;
    bool BackScreen  = false;
    bool USB = false;
    myHttp http;
    http.SetUrlIP(g_strServerIP);
    char GetBuf[100];
    memset(GetBuf,0,100);
    sprintf(GetBuf,"/service/classes/stu_askfor?apId=%s",g_strTerminalID);
    while(!m_bExitThread)
    {
        memset(JsonBuf,0,1024);
        http.Get(GetBuf);
        http.GetData(JsonBuf);
#ifdef _DEBUG_PRINT
        qDebug("While %s:",JsonBuf);
#endif
        g_pJson->Parse(JsonBuf);
        g_pJson->ReadJson(&Recode,"success");
        if(Recode)
        {
            g_pJson->ReadJson_v(&BackScreen,"data","enableBlack");
            g_pJson->ReadJson_v(&USB,"data","enableUSB");
#ifdef _DEBUG_PRINT
            qDebug("enbaleBack:%d enableUSB:%d",BackScreen,USB);
#endif
            if(BackScreen)
            {
                //system("");
            }
            else
            {
                //system("");
            }
            if(USB)
            {
                //system("");
            }
            else
            {
                //system("");
            }
        }
        else
        {

        }
        sleep(2);
    }
}
void LoginWidget::OnTimeOut()
{
    *m_pTime = QDateTime::currentDateTime();
    m_pTimeLCDNumber->display(m_pTime->toString("yyyy-MM-dd HH:mm:ss"));
}

LoginWidget::~LoginWidget()
{
    cMainExitFlag = 0;
    m_bExitThread = true;
    if(m_pMyDialog)
        delete m_pMyDialog;
    delete m_pTime;
    delete m_pSetForm;
    delete g_pNetConfig;
    delete g_pProcess;
    delete g_pJson;
    delete g_pLog;
    delete g_pMyHttp;
    if(g_LoadingFrame)
        delete g_LoadingFrame;
//    msg_queue_del();
    if (m_pqthread)
    {
        m_pqthread->stop();
        delete m_pqthread;
    }
    if (m_waitstu)
    {
        delete m_waitstu;
        m_waitstu = NULL;
    }
    g_ExitMsThread = true;
    MyMutex_destroy();
    pthread_mutex_destroy(&g_hreadMutex);
	pthread_mutex_destroy(&g_freestudyMutex);
    g_spicyProcessExit = true;
	activemq::library::ActiveMQCPP::shutdownLibrary();
    //qDebug() << "exit, main";
    delete ui;
}

void LoginWidget::on_SetpushButton_clicked()
{
    m_passui->show();
}

void LoginWidget::on_ShowPassUI()
{
    m_passui->hide();
    m_passui->setpasstext("");
    g_pNetConfig->ReadFile();
    if(g_pNetConfig->m_pNetConfig->s_bIP)
    {
        m_pSetForm->SetDHCPSetIPTrue();
    }
    else
    {
        m_pSetForm->SetNoDHCPSetIPTrue();
    }
    if(g_pNetConfig->m_pNetConfig->s_bDNS)
    {
        m_pSetForm->SetDHCPSetDNSTrue();
    }
    else
    {
        m_pSetForm->SetNoDHCPSetDNSTrue();
    }
    m_pSetForm->SetIPLineEdit(g_pNetConfig->m_pNetConfig->s_strIP);
    m_pSetForm->SetMaskIPLineEdit(g_pNetConfig->m_pNetConfig->s_strIPMask);
    m_pSetForm->SetGateWayIPLineEdit(g_pNetConfig->m_pNetConfig->s_strGateWay);

    m_pSetForm->SetDNS1LineEdit(g_pNetConfig->m_pNetConfig->s_strDNS[0]);
    m_pSetForm->SetDNS2LineEdit(g_pNetConfig->m_pNetConfig->s_strDNS[1]);
    m_pSetForm->SetDNS3LineEdit(g_pNetConfig->m_pNetConfig->s_strDNS[2]);

    m_pSetForm->setWindowModality(Qt::ApplicationModal);
    m_pSetForm->show();
}

void LoginWidget::on_ShutdownpushButton_clicked()
{
    //this->close();
    MyDialog *pshutdlg = new MyDialog();
    pshutdlg->setText("您确认要关机？");
    pshutdlg->setFlag(SHUTDOWN);
    pshutdlg->show();
}

void LoginWidget::on_InformationpushButton_clicked()
{
    //qDebug("About");
}
void LoginWidget::on_LeftPushButton()
{
    m_pClassNameConfig->MinusInode();
    m_pClassNameConfig->SetLabelName();
}
void LoginWidget::on_RightPushButton()
{
    m_pClassNameConfig->AddInode();
    m_pClassNameConfig->SetLabelName();
}
void LoginWidget::on_LabelChecked()
{
    g_pLog->WriteLog(0, "LoginWidget::on_LabelChecked  !!!!!!! .\n");
    m_pClassNameConfig->ChooseOne();
    m_pClassNameConfig->SetLabelName();
}

void LoginWidget::on_LableDoubleClicked()
{
    //double course
    //qDebug("LoginWidget::on_LableDoubleClicked  !!!!!!! .\n");
	g_pLog->WriteLog(0, "LoginWidget::on_LableDoubleClicked  !!!!!!! .\n");
    on_EnterPushButton();
}

void LoginWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,width(),height(),QPixmap(DESKTOPMAP));
}

pthread_t g_contid = 0;
void *thrd_connect(void *)
{
    LoginWidget  *loginWid = (LoginWidget  *)g_loginWnd;
    if (loginWid == NULL)
    {
         g_pLog->WriteLog(0,"thrd_connect, loginWid == NULL xxxxx .");
        return NULL;
    }
    QString name = loginWid->m_pClassNameConfig->GetClassName();
    QString id = loginWid->m_pClassNameConfig->GetClassID();
    //qDebug("Name :%s ID:%s",name.toStdString().c_str(),id.toStdString().c_str());
    g_pLog->WriteLog(0,"Name :%s ID:%s",name.toStdString().c_str(),id.toStdString().c_str());
    char TempBuf[1024];
    memset(TempBuf,0,1024);
#ifdef ARM
    long long last_time = 0;
#else
    long last_time = 0;
#endif
    last_time = __GetTime();
    QString str_time = QString::number(last_time, 10);
    if(strlen(g_strRoomNum)!= 0 && strlen(g_strSeatNum) != 0)
    {
        //sprintf(TempBuf,"/service/desktops/stu_display?seatNumber=%s&roomName=%s&courseId=%s&apId=%s",g_strSeatNum,g_strRoomNum,id.toStdString().c_str(), g_strTerminalID);
        sprintf(TempBuf,"###request_display###{\"datetime\":\"%s\",\"data\":{\"id\":\"%s\",\"templateId\":\"%s\"}}", str_time.toStdString().c_str(), g_strTerminalID, id.toStdString().c_str());
    }
    g_pLog->WriteLog(0,"Stuself Connect Post Buf:%s",TempBuf);
    if (NULL != g_Pproduce)
        g_Pproduce->send(TempBuf, strlen(TempBuf));
    pthread_detach(pthread_self());
    return NULL;
}

void LoginWidget::on_EnterPushButton()
{
    ReportMsg reportmsg;
    reportmsg.action = USER_WAITINGDLG_SHOW;
    call_msg_back(msg_respose, reportmsg);
    SetEnable(false);
    if(pthread_create(&g_contid,NULL,thrd_connect, NULL))
    {
        printf("create Thread Error");
    }
}

void LoginWidget::keyPressEvent(QKeyEvent *event)
{
  	qDebug("---------%8x", event->key());
    switch(event->key())
    {
        case Qt::Key_Left:
            {
                if (m_pClassNameConfig)
                    m_pClassNameConfig->MoveLeft();
            }
            break;
        case Qt::Key_Right:
            {
                if (m_pClassNameConfig)
                    m_pClassNameConfig->MoveRigth();
            }
            break;
        case Qt::Key_Enter:
        case Qt::Key_Return:
            {
                if (m_pClassNameConfig != NULL && m_pClassNameConfig->m_iClassNum)
                {
                    on_EnterPushButton();
                }
            }
            break;
    }
    QWidget::keyPressEvent(event);
}

void LoginWidget::createPipe()
{
    int res = 0;
    if (access(FIFO_PIPE, F_OK) == -1)
    {
        //Pipe file is not exist
        res = mkfifo(FIFO_PIPE, S_IRUSR | S_IWUSR);
        if (res != 0)
        {
             g_pLog->WriteLog(0,"Error, could not create fifo %s \n", FIFO_PIPE);
             return;
        }
    }
}

void LoginWidget::UpdateNetOffDialog()
{
    if (m_pMyDialog)
    {
        //qDebug() << "show netoff dailog 0000.";
        if (!m_pMyDialog->getShow())
        {
            //qDebug() << "show netoff dailog.";
            m_pMyDialog->setShow(true);
            m_pMyDialog->show();
            m_pMyDialog->setNetOff("网络异常，请联系管理员!", 5);
        }
    }
}

void LoginWidget::HideNetOffDialog()
{
    if (m_pMyDialog)
    {
        //qDebug() << "hide netoff dailog 11111.";
        m_pMyDialog->setShow(false);
        m_pMyDialog->hide();
    }
}

void LoginWidget::widget_resize()
{
    int scr_width = QApplication::desktop()->width();
    int scr_height = QApplication::desktop()->height();
    float factor_x = (float)scr_width/g_scr_old_width;
    float factor_y = (float)scr_height/g_scr_old_height;
    my_resize(this, factor_x, factor_y);
    QPixmap pixmap_enter(ENTERPUSH);
    m_pEnterPushButton->resize(pixmap_enter.width(), pixmap_enter.height());
    my_resize(m_pEnterPushButton, factor_x, factor_y);
    int enter_width = m_pEnterPushButton->width();
    int enter_height = m_pEnterPushButton->height();
    m_pEnterPushButton->setStyleSheet(ENTER);
    m_pEnterPushButton->move(scr_width/2 - enter_width/2, scr_height/2 + 2*m_pClassName2->height()/2);
}
