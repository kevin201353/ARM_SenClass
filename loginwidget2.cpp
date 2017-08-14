#include "loginwidget.h"
#include "myui_loginwidget.h"
#include "include.h"
#include "global.h"
#include "testnet.h"
#include <QDate>
#include <QDesktopWidget>
#include "qthread.h"

extern NetConfig   *g_pNetConfig;
extern void amq_monitor();
extern volatile bool g_resetamq;
extern bool g_bshowwaitstu;
extern volatile bool g_bSetupAmq;
extern int g_msgid;

LoginWidget * g_loginWnd = NULL;
void *InitThread(void *param);
static pthread_t g_xtid = 0;
static char g_szCmd[1024] = {0};
static void *thrd_exec(void *param)
{
    PIPE_SPICYLOG(g_szCmd);
    FILE *fp;
    if ((fp = popen(g_szCmd, "r")) == NULL)
    {
        g_pLog->WriteLog(0,"zhaosenhua thrd_exec spicy cmd failed.");
    }
    pclose(fp);
    return NULL;
}

LoginWidget::LoginWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginWidget)
{
    ui->setupUi(this);
    this->setAutoFillBackground(true);
    g_loginWnd = this;
    m_pLogoQLable = ui->Logolabel;
    m_pLogoQLable->setAlignment(Qt::AlignCenter);
    //m_pLogoQLable->setPixmap(QPixmap(LOGOPNG));

    m_pTimeLCDNumber = ui->TimeLcdNumber;
    m_pSoftInforLabel = ui->Informationlabel;
    m_pServerManPushButton = ui->ServerManpushButton;
    m_pSetPushButton = ui->SetpushButton;
    m_pInformationPushButton = ui->InformationpushButton;
    m_pShutdownPushButton = ui->ShutdownpushButton;
    m_pLeftPushButton = ui->LeftpushButton;
    m_pRightPushButton = ui->RightpushButton;
    m_pEnterPushButton = ui->EnterpushButton;
    m_pClassName1 = ui->label_1;
    m_pClassName2 = ui->label_2;
    m_pClassName3 = ui->label_3;

    //setTabOrder(m_pLoginPushButton,m_pUserNameLineEdit);

    m_pRightPushButton->setMinimumSize(43,169);
    m_pRightPushButton->setFlat(false);
   // m_pRightPushButton->setStyleSheet(QStringLiteral(RIGHTPNG));
    m_pRightPushButton->setStyleSheet(RIGHTPNG);


    m_pLeftPushButton->setMinimumSize(43,169);
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

    m_pGroupWigdet = ui->groupBox;
    m_pGroupWigdet->setEnabled(false);
    m_pClassNameConfig = new ClassNameConfig();
//    m_pClassNameConfig->AddClass("语文");
//    m_pClassNameConfig->AddClass("数学");
//    m_pClassNameConfig->AddClass("英语");
//    m_pClassNameConfig->AddClass("1");
//    m_pClassNameConfig->AddClass("2");
//    m_pClassNameConfig->AddClass("3");
//    m_pClassNameConfig->AddClass("4");
    m_pClassNameConfig->AddLabel(m_pClassName1);
    m_pClassNameConfig->AddLabel(m_pClassName2);
    m_pClassNameConfig->AddLabel(m_pClassName3);
    m_pClassName1->SetCheckedFlag(true);
//   m_pClassNameConfig->ChooseOne();
//    m_pClassNameConfig->SetLabelName();

    m_pEnterPushButton->setMinimumSize(284,66);
    m_pEnterPushButton->setStyleSheet(ENTER);
    //m_pEnterPushButton->setEnabled(false);
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
    g_pNetConfig = new NetConfig();
    if(g_pLog == NULL)
    {
        g_pLog = new Log();
        g_pLog->InitLog();
    }
    if(g_pJson == NULL)
    {
        g_pJson = new myJson();
    }
    if(g_pMyHttp == NULL)
    {
        g_pMyHttp = new myHttp();
        g_pMyHttp->SetUrlIP(QString(g_strServerIP));
    }
    if(g_pProcess == NULL)
    {
        g_pProcess = new process(this);
        int iRecode = g_pProcess->Start();
        if(iRecode < 0)
        {
            printf("New process Error\n");
        }
    }
    initConfig();
    activemq::library::ActiveMQCPP::initializeLibrary();
    g_Pconsume = NULL;
    g_Pproduce = NULL;
    if(g_Pconsume == NULL)
    {
        g_Pconsume = new ActiveMQConsumer();
        g_Pconsume->start(g_strConsumerAdd,g_strConsumerQueue,false,false);
    }
    if(g_Pproduce == NULL)
    {
        g_Pproduce = new ActiveMQProduce();
    }
    g_bSetupAmq = false;
    create_msg_queue();
    wait_net_setup();
    g_resetamq = false;
    g_exitMonitoramq = false;
    g_bshowwaitstu = false;
    amq_monitor();
    if(pthread_create(&g_amqpid,NULL,InitThread,this))
    {

    }
    m_pqthread = NULL;
    m_pqthread = new qthread();
    connect(m_pqthread, SIGNAL(NoticeShow()), this, SLOT(UpdateNetOffDialog()));
    m_pqthread->start();
    m_pSetRoomSeat = new SetRoomSeatDialog(this);
    m_bExitThread = false;
    m_pMyDialog = new MyDialog();
    m_pSetForm = new SetForm();
    m_pSetForm->move(QApplication::desktop()->width()/2 - m_pSetForm->width()/2, QApplication::desktop()->height()/2 - m_pSetForm->height()/2);
    m_pServerManPushButton->hide();
    m_pInformationPushButton->hide();
    QDate *date = new QDate();
    QDate date2 = date->currentDate();
    QString strdate = date2.toString("yyyyMMdd");
    QString strsoft = m_pSoftInforLabel->text();
    strsoft += ": V1.0_";
    strsoft += strdate;
    m_pSoftInforLabel->setText(strsoft);
    if (date)
    {
        delete date;
        date = NULL;
    }
}

void *InitThread(void *param)
{
    if (param == NULL)
        return NULL;

    LoginWidget *pLoginWidget = (LoginWidget *)param;
    g_resetamq = false;
    char data[100] = {0};
    for(int i=0; i<10; i++)
    {
        memset(data, 0, 100);
        msg_recv(data);
        if (strcmp(data, "0") == 0)
            break;
        qDebug() << "msg queue have message:  " + QString(data);
    }
    while(true)
    {
        if (g_bSetupAmq)
        {
           int ret = ping_net(g_strServerIP);
           if (ret == 1)
           {
               break;
           }
        }
        qDebug() << "wait net running.\n";
        sleep(2);
    }
    if(g_Pconsume != NULL)
    {
        g_Pconsume->runConsumer();
    }
    if (g_Pproduce != NULL)
    {
        g_Pproduce->start(g_strProduceAdd,20,g_strProduceQueue,false,false);
    }
    ///////////////////////////////////////////////////////////////
    char TempBuf[1024];
    char JsonBuf[10240];
    memset(JsonBuf,0,10240);
    memset(TempBuf,0,1024);
    sprintf(TempBuf,"/service/aps/config?id=%s&roomName=%s&seat=%s&reboot=false",g_strTerminalID,g_strRoomNum,g_strSeatNum);
    g_pLog->WriteLog(0,"UP SeatNum RoomNum:%s",TempBuf);
   // qDebug("TempBuf:%s",TempBuf);
    myHttp http;
    http.SetUrlIP(g_strServerIP);
    http.Get(TempBuf);
    http.GetData(JsonBuf);
    qDebug("xxxxxx amq UP SeatNum RoomNum%s\n",JsonBuf);
    g_pLog->WriteLog(0,"Recv Json:%s",JsonBuf);

    ///////////////////////////////////////////////////////////////
    char strClassRoomName[20][100];
    memset(JsonBuf,0,10240);
    memset(TempBuf,0,1024);
    sprintf(TempBuf, "/service/rooms/list?qp.sortby=name&qp.direction=asc");
    http.Get(TempBuf);
    http.GetData(JsonBuf);
    g_pLog->WriteLog(0,"Recv Json:%s", JsonBuf);
    memset(strClassRoomName, 0, sizeof(strClassRoomName));
    myJson *pMyJson = new myJson();
    int iRecode = 0;
    pMyJson->Parse(JsonBuf);
    iRecode = pMyJson->ReadJson(strClassRoomName, "data", "list", "name", 20);
    for (int i=0; i<iRecode; i++)
    {
        if (strcmp(g_strRoomNum, strClassRoomName[i] == 0))
        {
            pLoginWidget->m_pSetForm->m_pRoomNumcomboBox->addItem(QString(g_strRoomNum));
            break;
        }
    }
    for (int i=0; i<iRecode; i++)
    {
        if (strcmp(g_strRoomNum, strClassRoomName[i]))
        {
            pLoginWidget->m_pSetForm->m_pRoomNumcomboBox->addItem(QString(strClassRoomName[i]));
        }
    }

    if (pMyJson)
        delete pMyJson;
    pLoginWidget->m_pSetForm->m_pRoomNumcomboBox->setCurrentIndex(0);
}
void *WhileFun(void *param)
{
    if (param == NULL)
        return NULL;

    LoginWidget *p = (LoginWidget *)param;
        p->Thread();
}

void LoginWidget::initConfig()
{
    g_pNetConfig->GetMacAdd(g_strTerminalID,false);
    strcat(g_strConsumerQueue, "edu_");
    strcat(g_strConsumerQueue, g_strTerminalID);
    GetConfigString(CONFIGNAME,"MQ","ProduceQueue","edu_Queue",g_strProduceQueue,50);
    GetConfigString(CONFIGNAME,"ROOM","ServiceIP","192.168.120.36",g_strServerIP,25);
    qDebug("Service:%s",g_strServerIP);
    strcpy(g_strConsumerAdd,"tcp://");
    strcat(g_strConsumerAdd,g_strServerIP);
    strcat(g_strConsumerAdd,":61616");
    strcpy(g_strProduceAdd,g_strConsumerAdd);
    qDebug("ActiveMQ Server:%s Recv Queue:%s Send Queue:%s",g_strConsumerAdd, g_strConsumerQueue,g_strProduceQueue);
    qDebug(g_strProduceAdd);
    GetConfigString(CONFIGNAME,"ROOM","ClassName","Default",g_strRoomNum,100);
    GetConfigString(CONFIGNAME,"ROOM","SeatName","A1",g_strSeatNum,20);
    qDebug("%s %s",g_strRoomNum,g_strSeatNum);
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
        qDebug("While %s:",JsonBuf);
        g_pJson->Parse(JsonBuf);
        g_pJson->ReadJson(&Recode,"success");
        if(Recode)
        {
            g_pJson->ReadJson_v(&BackScreen,"data","enableBlack");
            g_pJson->ReadJson_v(&USB,"data","enableUSB");
            qDebug("enbaleBack:%d enableUSB:%d",BackScreen,USB);
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
    g_exitMonitoramq = true;
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
//    if (g_Loadingwnd != NULL)
//    {
//        delete g_Loadingwnd;
//        g_Loadingwnd = NULL;
//    }
    msg_queue_del();
    pthread_join(g_monitoramq, NULL);
    if (m_pqthread)
    {
        m_pqthread->stop();
        delete m_pqthread;
    }
    delete ui;
}

void LoginWidget::on_SetpushButton_clicked()
{
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
    m_pMyDialog->setText("确定关机！");
    m_pMyDialog->setFlag(SHUTDOWN);
    m_pMyDialog->show();
}

void LoginWidget::on_InformationpushButton_clicked()
{
    qDebug("About");
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
    m_pClassNameConfig->ChooseOne();
    m_pClassNameConfig->SetLabelName();
}

void LoginWidget::on_LableDoubleClicked()
{
    //double course
    qDebug("LoginWidget::on_LableDoubleClicked  !!!!!!! .\n");
    on_EnterPushButton();
}

void LoginWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,width(),height(),QPixmap(DESKTOPMAP));
}

static pthread_t g_contid = 0;
static void *thrd_connect(void *)
{
    LoginWidget  *loginWid = (LoginWidget  *)g_loginWnd;
    if (loginWid == NULL)
        return NULL;
    myHttp http;
    http.SetUrlIP(g_strServerIP);
    QString name = loginWid->m_pClassNameConfig->GetClassName();
    QString id = loginWid->m_pClassNameConfig->GetClassID();
    qDebug("Name :%s ID:%s",name.toStdString().c_str(),id.toStdString().c_str());
    g_pLog->WriteLog(0,"Name :%s ID:%s",name.toStdString().c_str(),id.toStdString().c_str());
    bool ReturnCode = false;
    char TempBuf[100];
    char JsonBuf[1024];
    int Port = 0;
    char IP[20];
    char Ticket[50];
    memset(IP,0,20);
    memset(Ticket,0,50);
    memset(JsonBuf,0,1024);
    memset(TempBuf,0,100);
    if(strlen(g_strRoomNum)!= 0 && strlen(g_strSeatNum) != 0)
    {
        sprintf(TempBuf,"seatNumber=%s&roomName=%s&courseId=%s",g_strSeatNum,g_strRoomNum,id.toStdString().c_str());
    }
    g_pLog->WriteLog(0,"Stuself Connect Post Buf:%s",TempBuf);
   // qDebug("Stuself Connect Post Buf:%s",TempBuf);
    int run_count = 0;
    while(run_count <= 180)  //tree miniture exit
    {
        http.Post("/service/desktops/stu_display",TempBuf);
        http.GetData(JsonBuf);
        //qDebug("Connect Json:%s",JsonBuf);
        g_pLog->WriteLog(0,"Connect Json:%s",JsonBuf);
        g_pJson->Parse(JsonBuf);
        ReturnCode = false;
        g_pJson->ReadJson(&ReturnCode,"success");
        if(ReturnCode)
        {
            g_pJson->ReadJson_v(&Port,"data","port");
            g_pJson->ReadJson_v(IP,"data","host");
            g_pJson->ReadJson_v(Ticket,"data","ticket");
            memset(g_pProcess->m_strVmID,0,50);
            g_pJson->ReadJson_v(g_pProcess->m_strVmID,"data","vmId");
            qDebug("IP:%s Port:%d Ticket:%s",IP,Port,Ticket);
            g_pLog->WriteLog(0,"IP:%s Port:%d Ticket:%s VmID:%s",IP,Port,Ticket,g_pProcess->m_strVmID);
            system("cat /tmp/data_*");
            system("rm -f /tmp/data_*");
            sprintf(g_szCmd, "spicy.sh -h %s -p %d -f > %s", IP, Port, "/usr/local/shencloud/log/spicy.log");
            if (pthread_create(&g_xtid, NULL, thrd_exec, NULL) != 0)
            {
                g_pLog->WriteLog(0,"zhaosenhua create spicy thread failed.");
            }
            int ncount = 0;
            while(access("/tmp/data_port",F_OK))
            {
                if (ncount >= 10)
                {
                    break;
                }
                ncount++;
                sleep(1);
            }
            system("cat /tmp/data_*");
            char port[20];
            memset(port,0,20);
            FILE *pf = fopen("/tmp/data_port","r");
            if(pf != NULL)
            {
                fread(port,20,1,pf);
                g_pLog->WriteLog(0,"/tmp/data_port:%s",port);
                fclose(pf);
            }
            char data_xor[20];
            memset(data_xor,0,20);
            pf = fopen("/tmp/data_xor","r");
            if(pf != NULL)
            {
                fread(data_xor,20,1,pf);
                g_pLog->WriteLog(0,"/tmp/data_xor:%s",data_xor);
                fclose(pf);
            }
            char append[1024];
            memset(append,0,1024);
            sprintf(append,"vmId=%s&connected=true&apIp=%s&apMac=%s&dsPort=%s&dsXor=%s",\
                    g_pProcess->m_strVmID,g_pProcess->m_strIP,g_pProcess->m_strMac,port,data_xor);
            g_pLog->WriteLog(0,"Report Action:%s",append);
            http.Post("/service/aps/report_action",append);
            memset(JsonBuf,0,1024);
            http.GetData(JsonBuf);
            g_pLog->WriteLog(0,"student report action:%s\n",JsonBuf);
            g_pJson->Parse(JsonBuf);
            g_pJson->ReadJson(&ReturnCode,"success");
            if(ReturnCode)
            {
                g_pLog->WriteLog(0,"Report connect[true] Success");
                break;
            }
            else
            {
                g_pLog->WriteLog(0,"Report connect[true] Error");
            }
        }
        run_count++;
        sleep(1);
    }//while
    return NULL;
}

void LoginWidget::on_EnterPushButton()
{
    ReportMsg reportmsg;
    reportmsg.action = USER_WAITINGDLG_SHOW;
    call_msg_back(msg_respose, reportmsg);
    m_pEnterPushButton->setEnabled(false);
    if(pthread_create(&g_contid,NULL,thrd_connect, NULL))
    {
        printf("create Thread Error");
    }
    pthread_join(g_contid, NULL);
    reportmsg.action = USER_WAITINGDLG_EXIT;
    call_msg_back(msg_respose, reportmsg);
    m_pEnterPushButton->setEnabled(true);
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
        if (!m_pMyDialog->getShow())
        {
            m_pMyDialog->setShow(true);
            m_pMyDialog->show();
            m_pMyDialog->setNetOff("network off, Please contact manager!", 5);
        }
    }
}
