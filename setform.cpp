#include "setform.h"
#include "ui_setform.h"
#include "netconfig.h"
#include <pthread.h>
#include "include.h"
#include "global.h"
#include <QDesktopWidget>
#include <QDebug>
#include <QFontDatabase>

extern NetConfig   *g_pNetConfig;
extern char g_strRoomNum[100];
extern char g_strSeatNum[20];

int detect_process_comm(char *name)
{
    int nRet = 0;
    FILE* fp = NULL;
    int count = 0;
    char buf[100] = {0};
    char command[100] = {0};
    sprintf(command, "sudo ps -ef | grep %s | grep -v grep | wc -l", name);
    fp = popen(command, "r");
    if (fp == NULL)
    {
        qDebug("detect_process popen failed.\n");
        return -1;
    }
    if (fgets(buf, 100, fp) != NULL)
    {
        count = atoi(buf);
        if (count == 0)
            nRet = 0;
        else
            nRet = 1;
    }
    if (fp != NULL)
        pclose(fp);
    return nRet;
}

SetForm::SetForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetForm)
{
    ui->setupUi(this);
    //this->setWindowFlags(this->windowFlags()& ~Qt::WindowMinMaxButtonsHint);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    this->setWindowModality(Qt::ApplicationModal);
    //this->setFixedSize(this->width(), this->height());
    this->setWindowTitle("设置");
    //m_Font.setFamily("微软雅黑");
    m_Font.setPixelSize(15);
    m_pSavePushButton = ui->SavepushButton;
    m_pCancelPushButton = ui->CancelpushButton;
    m_pSetTabWidget = ui->SetTabWidget;
    m_pSetTabWidget->setFont(m_Font);
    m_pSetTabWidget->setTabText(0,"网络设置");
    m_pSetTabWidget->setTabText(1,"系统设置");
	m_pSetTabWidget->setTabText(2,"网络检测");

    m_pSavePushButton->setFont(m_Font);
    m_pSavePushButton->setText("保存");
    //m_pSavePushButton->setStyleSheet("background-color:rgb(0,123,214);color:rgb(255,255,255)");
    m_pCancelPushButton->setFont(m_Font);
    //m_pCancelPushButton->setStyleSheet("background-color:rgb(0,123,214);color:rgb(255,255,255)");
    m_pCancelPushButton->setText("取消");

    m_IPGroupBox = ui->IPgroupBox;
    m_DNSGroupBox = ui->DNSgroupBox;
    m_IPGroupBox->setTitle("IP设置");
    m_DNSGroupBox->setTitle("DNS设置");

    m_pIPLineEdit = ui->IPlineEdit;
    m_pChildIPLineEdit = ui->ChildIPlineEdit;
    m_pGetWayLineEdit = ui->GetWaylineEdit;
    m_pDNS1LineEdit = ui->DNS1lineEdit;
    m_pDNS2LineEdit = ui->DNS2lineEdit;
    m_pDNS3LineEdit = ui->DNS3lineEdit;
    m_pRoomNumcomboBox = ui->RoomNumcomboBox;
    m_pSeatLineEdit = ui->SeatlineEdit;
    m_tiplabel = ui->Tiplabel22;

    QRegExp    RegIPLineEdit("((25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])[\\.]){3}"
                             "(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])");       //expand reg
    m_pRegValue = new QRegExpValidator(RegIPLineEdit);

    m_pIPLineEdit->setValidator(m_pRegValue);
    m_pChildIPLineEdit->setValidator(m_pRegValue);
    m_pGetWayLineEdit->setValidator(m_pRegValue);
    m_pDNS1LineEdit->setValidator(m_pRegValue);
    m_pDNS2LineEdit->setValidator(m_pRegValue);
    m_pDNS3LineEdit->setValidator(m_pRegValue);

    m_pAutoSetIP = ui->AutoSetIPradioButton;
    m_pAutoSetDNS = ui->AutoSetDNSradioButton;
    m_pNoDHCPSetDNS = ui->ManualSetDNSradioButton;
    m_pNoDHCPSetIP = ui->ManualSetIPradioButton;
    m_pLoagingFrame = new LoadingFrame(this);
    m_pLoagingFrame->hide();
    m_pServerIPLineEdit = ui->ServerIPlineEdit;
    m_pServerIPLineEdit->setValidator(m_pRegValue);
    m_pServerIPLineEdit->setText(g_strServerIP);
    m_pAutoSetIP->setText("DHCP动态获取");
    m_pAutoSetDNS->setText("DHCP动态获取");
    m_pNoDHCPSetDNS->setText("静态获取");
    m_pNoDHCPSetIP->setText("静态获取");
    ui->DNS3label->setVisible(false);
    m_pDNS3LineEdit->setVisible(false);
    ui->RoomNumlabel->setText("教室号");
    //m_pSeatNumLabel->setFont(m_Font);
    ui->SeatNumlabel->setText("座位号");
    //ui->title_bar->setStyleSheet("background-color: rgb(0,123,214)");
    connect(ui->btnExit, SIGNAL(clicked(bool)), this, SLOT(exit_widget()));
    //connect(ui->lineEdit_Ping, SIGNAL(returnPressed()), this, SLOT(on_returnPressed()));
    connect(ui->btn_network, SIGNAL(clicked(bool)), this, SLOT(on_returnPressed()));
    ui->Tiplabel22->setText("");
//    m_pMyDialog = NULL;
//    m_pMyDialog = new MyDialog(this);
//    m_pMyDialog->hide();
//    m_bSetRoomfail = false;
    m_pqthread = NULL;
    m_pqthread = new qthreadPing();
    connect(m_pqthread, SIGNAL(NoticeMsg()), this, SLOT(update()));
    //ui->label_Tip->setText("输入ping ip地址，然后按回车键 如：ping 127.0.0.1  结束: ctrl + c");
    //ui->label_Tip->setText("结束: ctrl + c");
    ui->label_Tip->setText("");
    ui->SeatlineEdit->setText(g_strSeatNum);
    ui->listWidget_Ping->clear();
    ui->btn_network->setText("检测");
#ifdef CCLOUD
    ui->btn_network->setStyleSheet("background-color:rgb(255,153,19);color:rgb(255,255,255)");
    ui->title_bar->setStyleSheet("background-color: rgb(255,153,19)");
    m_pSavePushButton->setStyleSheet("background-color:rgb(255,153,19);color:rgb(255,255,255)");
    m_pCancelPushButton->setStyleSheet("background-color:rgb(255,153,19);color:rgb(255,255,255)");
#else
    ui->btn_network->setStyleSheet("background-color:rgb(5,149,245);color:rgb(255,255,255)");
    ui->title_bar->setStyleSheet("background-color: rgb(5,149,245)");
    m_pSavePushButton->setStyleSheet("background-color:rgb(5,149,245);color:rgb(255,255,255)");
    m_pCancelPushButton->setStyleSheet("background-color:rgb(5,149,245);color:rgb(255,255,255)");
#endif
    QString fontName = loadFontFromFile("/usr/local/share/fonts/msyh.ttf");
    QFont font(fontName);
    font.setPointSize(18);
    //font.setBold(true);
    ui->label_title->setFont(font);
    ui->label_title->setText("学生终端配置");

#if 1
    m_pRoomNumcomboBox->setEnabled(false);
    m_pRoomNumcomboBox->setVisible(false);
    ui->RoomNumlabel->setEnabled(false);
    ui->RoomNumlabel->setVisible(false);
#endif

    widget_resize();
}

SetForm::~SetForm()
{
    delete m_pRegValue;
    delete ui;
//    if (m_pMyDialog != NULL)
//    {
//        delete m_pMyDialog;
//        m_pMyDialog = NULL;
//    }
    if (m_pqthread)
    {
        delete m_pqthread;
        m_pqthread = NULL;
    }
}
void SetForm::SetIPLineEdit(const char *ip)
{
    QString IP(ip);
    m_pIPLineEdit->setText(IP);
}
void SetForm::SetMaskIPLineEdit(const char *ip)
{
    QString IP(ip);
    m_pChildIPLineEdit->setText(IP);
}
void SetForm::SetGateWayIPLineEdit(const char *ip)
{
    QString IP(ip);
    m_pGetWayLineEdit->setText(IP);
}
void SetForm::SetDNS1LineEdit(const char *ip)
{
    QString IP(ip);
    m_pDNS1LineEdit->setText(IP);
}
void SetForm::SetDNS2LineEdit(const char *ip)
{
    QString IP(ip);
    m_pDNS2LineEdit->setText(IP);
}
void SetForm::SetDNS3LineEdit(const char *ip)
{
    QString IP(ip);
    m_pDNS3LineEdit->setText(IP);
}
void SetForm::on_CancelpushButton_clicked()
{
    this->close();
}

void SetForm::on_AutoSetIPradioButton_clicked(bool checked)
{
    if(checked)
    {
        m_bDHCP_IP = true;
        m_pIPLineEdit->setEnabled(false);
        m_pChildIPLineEdit->setEnabled(false);
        m_pGetWayLineEdit->setEnabled(false);
        m_pAutoSetDNS->setEnabled(true);
    }
}
////////////////////////////
bool SetForm::isInvalidNetmask(const char *subnet)
{
    unsigned int b = 0, n[4];
    sscanf(subnet, "%u.%u.%u.%u", &n[3], &n[2], &n[1], &n[0]);
    for(int i = 0; i < 4; i++)
        b += n[i] << (i * 8);
    b = ~b + 1;
    if((b & (b - 1)) == 0)
        return false;
    return true;
}
void SetForm::on_ManualSetIPradioButton_clicked(bool checked)
{
    if(checked)
    {
        m_bDHCP_IP = false;
        m_pIPLineEdit->setEnabled(true);
        m_pChildIPLineEdit->setEnabled(true);
        m_pGetWayLineEdit->setEnabled(true);
        m_pAutoSetDNS->setEnabled(false);
        m_pNoDHCPSetDNS->setChecked(true);
        on_ManualSetDNSradioButton_clicked(true);
    }
}
void SetForm::on_AutoSetDNSradioButton_clicked(bool checked)
{
    if(checked)
    {
        m_bDHCP_DNS = true;
        m_pDNS1LineEdit->setEnabled(false);
        m_pDNS2LineEdit->setEnabled(false);
        m_pDNS3LineEdit->setEnabled(false);
    }
}
void SetForm::on_ManualSetDNSradioButton_clicked(bool checked)
{
    if(checked)
    {
        m_bDHCP_DNS = false;
        m_pDNS1LineEdit->setEnabled(true);
        m_pDNS2LineEdit->setEnabled(true);
        m_pDNS3LineEdit->setEnabled(true);
    }
}
void *SystemThread(void *param)
{
    SetForm *pTemp = (SetForm *)param;
//    char SystemCmd[1024];
//    memset(SystemCmd,0,1024);
//    sprintf(SystemCmd,"sudo ifdown %s \nsudo ifup %s",ETH0, ETH0);
//    system(SystemCmd);
//    memset(SystemCmd,0,1024);
//    sprintf(SystemCmd,"sudo ifup %s",ETH0);
//    system(SystemCmd);
//    g_pMyHttp->SetUrlIP(pTemp->m_strServerIP);
//    memset(g_strServerIP,0,25);
//    strncpy(g_strServerIP,pTemp->m_strServerIP.toStdString().c_str(),25);
//    WriteConfigString(CONFIGNAME,"ROOM","ServiceIP",g_strServerIP);
//    pTemp->SetLoadingFram(false);
//    //pTemp->SetSalveEnable(true);
//    pTemp->setEnabled(true);
//    if (pTemp->m_bSetRoomfail)
//        pTemp->m_tiplabel->setText("教室名称或座位号设置失败!");
//    else
//        pTemp->m_tiplabel->setText("");
//    pTemp->close();

    /*****************************************/
    /***************************************************/
         //add zsh
    /****************************************************/
    char SystemCmd[1024];
    memset(SystemCmd,0,1024);
    //sprintf(SystemCmd,"sudo ifdown %s",ETH0);
    if (!pTemp->m_bDHCP_IP)
    {
        sprintf(SystemCmd, "sudo ifconfig eth0 %s netmask %s \nsudo route add default gw %s", g_pNetConfig->m_pNetConfig->s_strIP, g_pNetConfig->m_pNetConfig->s_strIPMask, g_pNetConfig->m_pNetConfig->s_strGateWay);
    }
    else
    {
#ifdef ARM
        sprintf(SystemCmd, "sudo ifdown eth0 \nsudo ifup eth0");
#else
        sprintf(SystemCmd, "sudo /etc/init.d/networking restart");
#endif
    }
    system(SystemCmd);
    memset(SystemCmd,0,1024);
    if (!pTemp->m_bDHCP_DNS)
    {
    }else
    {
        sprintf(SystemCmd, "%s", "ifdown eth0 \nifup eth0");
    }
    system(SystemCmd);
   // memset(SystemCmd,0,1024);
   // sprintf(SystemCmd,"sudo ifup %s",ETH0);
   // system(SystemCmd);
//    if (m_bSetRoomfail)
//        m_tiplabel->setText("教室名称或座位号设置失败!");
//    else
//       m_tiplabel->setText("");
    pTemp->saveRoomSeat();
    pTemp->SetLoadingFram(false);
    pTemp->setEnabled(true);
    pTemp->close();
}
void SetForm::on_SavepushButton_clicked()
{
    pthread_t pid;

    if (!ui->IPlineEdit->isModified() &&
         !ui->ChildIPlineEdit->isModified() &&
            !ui->GetWaylineEdit->isModified() &&
            !ui->DNS1lineEdit->isModified() &&
            !ui->DNS2lineEdit->isModified() &&
            !ui->ServerIPlineEdit->isModified() &&
            !ui->SeatlineEdit->isModified())
    {
        this->close();
        //qDebug() << "setform no modified.";
        return;
    }

    memset(g_pNetConfig->m_pNetConfig,0,sizeof(g_pNetConfig->m_pNetConfig));
    g_pNetConfig->m_pNetConfig->s_bDNS = m_bDHCP_DNS;
    g_pNetConfig->m_pNetConfig->s_bIP = m_bDHCP_IP;
    strcpy(g_pNetConfig->m_pNetConfig->s_strIP,m_pIPLineEdit->text().toStdString().c_str());
    strcpy(g_pNetConfig->m_pNetConfig->s_strIPMask,m_pChildIPLineEdit->text().toStdString().c_str());
    strcpy(g_pNetConfig->m_pNetConfig->s_strGateWay,m_pGetWayLineEdit->text().toStdString().c_str());

    strcpy(g_pNetConfig->m_pNetConfig->s_strDNS[0],m_pDNS1LineEdit->text().toStdString().c_str());
    strcpy(g_pNetConfig->m_pNetConfig->s_strDNS[1],m_pDNS2LineEdit->text().toStdString().c_str());
    strcpy(g_pNetConfig->m_pNetConfig->s_strDNS[2],m_pDNS3LineEdit->text().toStdString().c_str());

    g_pNetConfig->WriteFile();
    m_pLoagingFrame->show();
    this->setEnabled(false);
    if(pthread_create(&pid,NULL,SystemThread,this))
    {
        printf("Create Thread Error!\n");
    }
}

void SetForm::on_ClassRoomNumllineEdit_textEdited(const QString &arg1)
{
    memset(g_strRoomNum,0,10);
    strcpy(g_strRoomNum,arg1.toStdString().c_str());
}

void SetForm::on_SeatNumlineEdit_textEdited(const QString &arg1)
{
    memset(g_strSeatNum,0,10);
    strcpy(g_strSeatNum,arg1.toStdString().c_str());
}

void SetForm::on_ServerIPlineEdit_textChanged(const QString &arg1)
{
    m_strServerIP = arg1;
}

void SetForm::on_IPlineEdit_textChanged(const QString &arg1)
{
    if(!arg1.length())
    {
        QPalette palette;
        palette.setColor(QPalette::Text,Qt::black);
        m_pIPLineEdit->setPalette(palette);
        m_pSavePushButton->setEnabled(true);
        return ;
    }
     QString args = arg1;
    int iRecode = 0;
    int pos;
    iRecode = m_pRegValue->validate(args,pos);
    if(iRecode == 1)
    {
//        QPalette palette;
//        palette.setColor(QPalette::Text,Qt::red);
//        m_pIPLineEdit->setPalette(palette);
        m_pSavePushButton->setEnabled(false);
    }
    if(iRecode == 2)
    {
        QPalette palette;
        palette.setColor(QPalette::Text,Qt::black);
        m_pIPLineEdit->setPalette(palette);
        m_pSavePushButton->setEnabled(true);
    }
}

void SetForm::on_ChildIPlineEdit_textChanged(const QString &arg1)
{
    if(!arg1.length())
    {
        QPalette palette;
        palette.setColor(QPalette::Text,Qt::black);
        m_pChildIPLineEdit->setPalette(palette);
        m_pSavePushButton->setEnabled(true);
        return ;
    }
     QString args = arg1;
    int iRecode = 0;
    int pos;
    iRecode = m_pRegValue->validate(args,pos);
    if(iRecode == 1)
    {
//        QPalette palette;
//        palette.setColor(QPalette::Text,Qt::red);
//        m_pChildIPLineEdit->setPalette(palette);
        m_pSavePushButton->setEnabled(false);
    }
    if(iRecode == 2)
    {
        QPalette palette;
        palette.setColor(QPalette::Text,Qt::black);
        m_pChildIPLineEdit->setPalette(palette);
        m_pSavePushButton->setEnabled(true);
    }
}

void SetForm::on_GetWaylineEdit_textChanged(const QString &arg1)
{
    if(!arg1.length())
    {
        QPalette palette;
        palette.setColor(QPalette::Text,Qt::black);
        m_pGetWayLineEdit->setPalette(palette);
        m_pSavePushButton->setEnabled(true);
        return ;
    }
     QString args = arg1;
    int iRecode = 0;
    int pos;
    iRecode = m_pRegValue->validate(args,pos);
    if(iRecode == 1)
    {
//        QPalette palette;
//        palette.setColor(QPalette::Text,Qt::red);
//        m_pGetWayLineEdit->setPalette(palette);
        m_pSavePushButton->setEnabled(false);
    }
    if(iRecode == 2)
    {
        QPalette palette;
        palette.setColor(QPalette::Text,Qt::black);
        m_pGetWayLineEdit->setPalette(palette);
        m_pSavePushButton->setEnabled(true);
    }
}

void SetForm::on_DNS1lineEdit_textChanged(const QString &arg1)
{
    if(!arg1.length())
    {
        QPalette palette;
        palette.setColor(QPalette::Text,Qt::black);
        m_pDNS1LineEdit->setPalette(palette);
        m_pSavePushButton->setEnabled(true);
        return ;
    }
     QString args = arg1;
    int iRecode = 0;
    int pos;
    iRecode = m_pRegValue->validate(args,pos);
    if(iRecode == 1)
    {
//        QPalette palette;
//        palette.setColor(QPalette::Text,Qt::red);
//        m_pDNS1LineEdit->setPalette(palette);
        m_pSavePushButton->setEnabled(false);
    }
    if(iRecode == 2)
    {
        QPalette palette;
        palette.setColor(QPalette::Text,Qt::black);
        m_pDNS1LineEdit->setPalette(palette);
        m_pSavePushButton->setEnabled(true);
    }
}

void SetForm::on_DNS2lineEdit_textChanged(const QString &arg1)
{
    if(!arg1.length())
    {
        QPalette palette;
        palette.setColor(QPalette::Text,Qt::black);
        m_pDNS2LineEdit->setPalette(palette);
        m_pSavePushButton->setEnabled(true);
        return ;
    }
     QString args = arg1;
    int iRecode = 0;
    int pos;
    iRecode = m_pRegValue->validate(args,pos);
    if(iRecode == 1)
    {
//        QPalette palette;
//        palette.setColor(QPalette::Text,Qt::red);
//        m_pDNS2LineEdit->setPalette(palette);
        m_pSavePushButton->setEnabled(false);
    }
    if(iRecode == 2)
    {
        QPalette palette;
        palette.setColor(QPalette::Text,Qt::black);
        m_pDNS2LineEdit->setPalette(palette);
        m_pSavePushButton->setEnabled(true);
    }
}

void SetForm::on_DNS3lineEdit_textChanged(const QString &arg1)
{
    if(!arg1.length())
    {
        QPalette palette;
        palette.setColor(QPalette::Text,Qt::black);
        m_pDNS3LineEdit->setPalette(palette);
        m_pSavePushButton->setEnabled(true);
        return ;
    }
     QString args = arg1;
    int iRecode = 0;
    int pos;
    iRecode = m_pRegValue->validate(args,pos);
    if(iRecode == 1)
    {
//        QPalette palette;
//        palette.setColor(QPalette::Text,Qt::red);
//        m_pDNS3LineEdit->setPalette(palette);
        m_pSavePushButton->setEnabled(false);
    }
    if(iRecode == 2)
    {
        QPalette palette;
        palette.setColor(QPalette::Text,Qt::black);
        m_pDNS3LineEdit->setPalette(palette);
        m_pSavePushButton->setEnabled(true);
    }
}

void SetForm::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons().testFlag(Qt::LeftButton) && m_moving)
    {
        this->move(this->pos() + (event->globalPos() - m_LastMousePosition));
        m_LastMousePosition = event->globalPos();
    }
}

void SetForm::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && event->localPos().y() > 0 && event->localPos().y() < 30)
    {
        m_moving = true;
        m_LastMousePosition = event->globalPos();
    }
}

void SetForm::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_moving = false;
    }
}

void SetForm::exit_widget()
{
    if (detect_process_comm("ping") == 1)
        system("sudo kill -9 $(pgrep ping)");
    close();
}

void SetForm::saveRoomSeat()
{
    QString strMqIP = m_pServerIPLineEdit->text();
    memset(g_strServerIP, 0, sizeof(g_strServerIP));
    m_strServerIP = strMqIP;
    strncpy(g_strServerIP, m_strServerIP.toStdString().c_str(),25);
    if (m_pServerIPLineEdit->text() != g_strServerIP)
    {
        //change server ip
        WriteConfigString(CONFIGNAME,"ROOM","ClassName",g_strRoomNum);
        WriteConfigString(CONFIGNAME,"ROOM","SeatName",g_strSeatNum);
        WriteConfigString(CONFIGNAME,"ROOM","ServiceIP",g_strServerIP);
        system("sudo reboot");
    }else
    {
        WriteConfigString(CONFIGNAME,"ROOM","ServiceIP",g_strServerIP);
        //memset(g_strRoomNum,0,100);
        memset(g_strSeatNum,0,20);
        //strncpy(g_strRoomNum,m_pRoomNumcomboBox->currentText().toStdString().c_str(),100);
        strncpy(g_strSeatNum,m_pSeatLineEdit->text().toStdString().c_str(),20);
        g_pLog->WriteLog(0,"Set Room Num:%s Sear Num:%s",g_strRoomNum,g_strSeatNum);
//        char TempBuf[1024];
//        char JsonBuf[10240];
//        bool Recode = false;
//        memset(JsonBuf,0,10240);
//        memset(TempBuf,0,1024);
//        sprintf(TempBuf,"/service/aps/config?id=%s&roomName=%s&seat=%s&reboot=false&sync=false", g_strTerminalID, g_strRoomNum, g_strSeatNum);
//        g_pLog->WriteLog(0,"UP SeatNum RoomNum:%s",TempBuf);
//        qDebug("TempBuf:%s",TempBuf);
//        myHttp *phttp = new myHttp();
//        phttp->SetUrlIP(m_strServerIP);
//        phttp->Get(TempBuf);
//        phttp->GetData(JsonBuf);
//        qDebug("UP SeatNum RoomNum%s\n",JsonBuf);
//        g_pLog->WriteLog(0,"Recv Json:%s",JsonBuf);
//        g_pJson->Parse(JsonBuf);
//        g_pJson->ReadJson(&Recode,"success");
//        if(Recode)
        {
            WriteConfigString(CONFIGNAME,"ROOM","ClassName",g_strRoomNum);
            WriteConfigString(CONFIGNAME,"ROOM","SeatName",g_strSeatNum);
//            m_bSetRoomfail = false;
        }
//        else
//        {
            //set failed
    //        m_pMyDialog->setFlag(1);
    //        m_pMyDialog->setText("设置失败!");
    //        m_pMyDialog->show();
//            m_bSetRoomfail = true;
//        }
    }
}

int myexec(const char *cmd, vector<string> &resvec)
{
    resvec.clear();
    FILE *pp = popen(cmd, "r");
    if (!pp)
    {
        return -1;
    }
    char tmp[1024] = {0};
    while (fgets(tmp, sizeof(tmp), pp) != NULL)
    {
        if (tmp[strlen(tmp) -1] == '\n')
        {
            tmp[strlen(tmp) - 1] = '\0';
        }

        resvec.push_back(tmp);
    }
    pclose(pp);
    return resvec.size();
}
void SetForm::on_returnPressed()
{
    ui->listWidget_Ping->clear();
    m_strPing = ui->lineEdit_Ping->text();
    m_pingCount = 0;
    m_pqthread->SetPingText(m_strPing);
    m_pqthread->start();
}

extern QString  g_strPing;
void SetForm::update()
{
    QListWidgetItem *newItem = new QListWidgetItem;
    newItem->setText(g_strPing);
    ui->listWidget_Ping->insertItem(m_pingCount, newItem);
    ui->listWidget_Ping->setFocus();
    m_pingCount++;
}

//void SetForm::keyPressEvent(QKeyEvent *event)
//{
    //qDebug("---------%8x", event->key());
    //comment by zhaosen
    /*
    if ((event->modifiers() == Qt::ControlModifier) && (event->key() == Qt::Key_C))
    {
        //qDebug("############# ---- ctrl +c .");
        system("sudo kill -9 $(pgrep ping)");
        QListWidgetItem *newItem = new QListWidgetItem;
        newItem->setText("ping over!");
        ui->listWidget_Ping->insertItem(m_pingCount, newItem);
        ui->lineEdit_Ping->setFocus();
    }*/
//}

void SetForm::widget_resize()
{
    int scr_width = QApplication::desktop()->width();
    int scr_height = QApplication::desktop()->height();
    float factor_x = (float)scr_width/g_scr_old_width;
    float factor_y = (float)scr_height/g_scr_old_height;
    my_resize(this, factor_x, factor_y);
    my_resize(ui->title_bar, factor_x, factor_y);
    my_resize(ui->label_title, factor_x, factor_y);
    my_resize(ui->btnExit, factor_x, factor_y);
    my_resize(ui->SetTabWidget, factor_x, factor_y);
    my_resize(ui->CancelpushButton, factor_x, factor_y);
    my_resize(ui->SavepushButton, factor_x, factor_y);
    ui->label_title->move(width()/2 - ui->label_title->width()/2, ui->label_title->y());
    //tab1
    my_resize(ui->DNSgroupBox, factor_x, factor_y);
    my_resize(ui->IPgroupBox, factor_x, factor_y);
//    my_resize(ui->AutoSetIPradioButton, factor_x, factor_y);
//    my_resize(ui->ManualSetIPradioButton, factor_x, factor_y);
//    my_resize(ui->IPlabel, factor_x, factor_y);
//    my_resize(ui->IPlineEdit, factor_x, factor_y);
//    my_resize(ui->ChildIPlabel, factor_x, factor_y);
//    my_resize(ui->GetWaylabel, factor_x, factor_y);
//    my_resize(ui->GetWaylineEdit, factor_x, factor_y);
//    my_resize(ui->AutoSetDNSradioButton, factor_x, factor_y);
//    my_resize(ui->ManualSetDNSradioButton, factor_x, factor_y);
//    my_resize(ui->DNS1label, factor_x, factor_y);
//    my_resize(ui->DNS1lineEdit, factor_x, factor_y);
//    my_resize(ui->DNS2label, factor_x, factor_y);
//    my_resize(ui->DNS2lineEdit, factor_x, factor_y);
//    my_resize(ui->DNS3label, factor_x, factor_y);
//    my_resize(ui->DNS3lineEdit, factor_x, factor_y);
//    //tab

      //tab2
      my_resize(ui->ServerIPlabel, factor_x, factor_y);
      my_resize(ui->ServerIPlineEdit, factor_x, factor_y);
      my_resize(ui->SeatNumlabel, factor_x, factor_y);
      my_resize(ui->SeatlineEdit, factor_x, factor_y);
      my_resize(ui->Tiplabel22, factor_x, factor_y);
      //tab3
      my_resize(ui->lineEdit_Ping, factor_x, factor_y);
      my_resize(ui->btn_network, factor_x, factor_y);
      my_resize(ui->label_Tip, factor_x, factor_y);
      //my_resize((QWidget *)ui->horizontalLayout_ping, factor_x, factor_y);
      my_resize(ui->listWidget_Ping, factor_x, factor_y);
      m_pLoagingFrame->move(width()/2 - m_pLoagingFrame->width()/2, height()/2 - m_pLoagingFrame->height()/2);
}
