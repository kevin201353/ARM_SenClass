#include "mylable.h"

myLable::myLable(QWidget *parent):QLabel(parent)
{
    memset(m_strIP,0,IPLEN);
    memset(m_strName,0,NAMELEN);
    m_bFrontLableEnable = false;
    m_bEnable = false;
    m_iSocket = 0;
    m_bThreadFlag = false;
    m_bExit = false;
    m_OldPosX = 0;
    m_OldPosY = 0;
    m_bDoubleCheckFlag = false;
    m_bMousePressFlag = false;
    m_pNormalPixmap = new QPixmap(NORMALPIC);
    this->setScaledContents(true);
    this->setPixmap(*m_pNormalPixmap);
    this->resize(300,200);
    this->setMinimumSize(300,200);
//    this->setMaximumSize(300,200);
    m_pFrontLabel = new QLabel((QLabel*)this);
    m_pFrontLabel->resize(300,50);
    m_pFrontLabel->setAlignment(Qt::AlignCenter);
    m_pFrontLabel->move(0,150);
    m_pFrontLabel->setAttribute(Qt::WA_TranslucentBackground);
    m_pFrontLabel->setVisible(false);
    m_pUphandPixmap = new QPixmap(UPHANDPIC);
    m_pNoNetworkPixmap = new QPixmap(NONETWORKPIC);
    m_pDarkScreenPixmap = new QPixmap(DARKSCREENPIC);
    m_pNoUSBPixmap = new QPixmap(NOUSBPIC);

//    pFrontLabel->setStyleSheet("background:transparent");
//    label->setAttribute(Qt::WA_TranslucentBackground);

    m_pDarkScreenAction = new QAction("启动",this);
    m_pRestartAction = new QAction("关闭",this);
    m_pStudentShowAction = new QAction("重启桌面",this);
    m_pSendFileAction = new QAction("重启终端",this);
    m_pRemoteAction = new QAction("远程协助",this);

    m_pDarkScreenAction->setEnabled(m_bEnable);
    m_pRestartAction->setEnabled(m_bEnable);
    m_pStudentShowAction->setEnabled(m_bEnable);
    m_pSendFileAction->setEnabled(m_bEnable);
    m_pRemoteAction->setEnabled(m_bEnable);

    m_pDarkScreenAction->setIcon(QIcon(DARKSCREENICON));
    m_pRestartAction->setIcon(QIcon(RESTARTICON));
    m_pStudentShowAction->setIcon(QIcon(STUDENTSHOWICON));
    m_pSendFileAction->setIcon(QIcon(SENDFILEICON));
    m_pRemoteAction->setIcon(QIcon(REMOTEACTIONICON));

    this->addAction(m_pDarkScreenAction);
    this->addAction(m_pRestartAction);
    this->addAction(m_pStudentShowAction);
    this->addAction(m_pSendFileAction);
    this->addAction(m_pRemoteAction);

    connect(m_pDarkScreenAction, SIGNAL(triggered()), this, SLOT(DarkScreenFun()));
    connect(m_pRestartAction, SIGNAL(triggered()), this, SLOT(RestartFun()));
    connect(m_pStudentShowAction, SIGNAL(triggered()), this, SLOT(StudentShowFun()));
    connect(m_pSendFileAction, SIGNAL(triggered()), this, SLOT(SendFileFun()));
    connect(m_pRemoteAction, SIGNAL(triggered()), this, SLOT(RemoteFun()));

    this->setContextMenuPolicy(Qt::ActionsContextMenu);

    if(pthread_create(&m_pid,NULL,ThreadFun,this))
    {
        printf("Create pthread Error!\n");
    }
}
void *ThreadFun(void *param)
{
    myLable *pLable = (myLable*)param;
    while(!pLable->m_bExit)
    {
        if(pLable->m_bThreadFlag)
        {
            //chu li shiping shuju
        }
        else
        {
            sleep(1);
        }

    }
    return 0;
}
void myLable::DarkScreenFun()
{
    qDebug("DarkScreen");
}
void myLable::RestartFun()
{
    qDebug("Restart");
}
void myLable::StudentShowFun()
{
    qDebug("StudentShow");
}
void myLable::SendFileFun()
{
    qDebug("SendFile");
}
void myLable::RemoteFun()
{
    qDebug("RemoteFun");
}
myLable::~myLable()
{
    m_bExit = true;
    if(m_pNormalPixmap)
        delete m_pNormalPixmap;
    if(m_pUphandPixmap)
        delete m_pUphandPixmap;
    if(m_pNoNetworkPixmap)
        delete m_pNoNetworkPixmap;
    if(m_pDarkScreenPixmap)
        delete m_pDarkScreenPixmap;
    if(m_pNoUSBPixmap)
        delete m_pNoUSBPixmap;
    if(m_pDarkScreenAction)
        delete m_pDarkScreenAction;
    if(m_pRestartAction)
        delete m_pRestartAction;
    if(m_pStudentShowAction)
        delete m_pStudentShowAction;
    if(m_pSendFileAction)
        delete m_pSendFileAction;
    if(m_pRemoteAction)
        delete m_pRemoteAction;
}

void myLable::ShowUpHand(bool flag)
{
    if(!m_bFrontLableEnable)
        return ;
    if(flag)
    {
        m_pFrontLabel->setPixmap(*m_pUphandPixmap);
        m_pFrontLabel->setVisible(flag);
    }
    else
    {
        m_pFrontLabel->setVisible(flag);
    }
}
void myLable::ShowNoNetwork(bool flag)
{
    if(!m_bFrontLableEnable)
        return ;
    if(flag)
    {
        m_pFrontLabel->setPixmap(*m_pNoNetworkPixmap);
        m_pFrontLabel->setVisible(flag);
    }
    else
    {
        m_pFrontLabel->setVisible(flag);
    }
}
void myLable::ShoeDarkScreen(bool flag)
{
    if(!m_bFrontLableEnable)
        return ;
    if(flag)
    {
        m_pFrontLabel->setPixmap(*m_pDarkScreenPixmap);
        m_pFrontLabel->setVisible(flag);
    }
    else
    {
        m_pFrontLabel->setVisible(flag);
    }
}
void myLable::ShowNoUSB(bool flag)
{
    if(!m_bFrontLableEnable)
        return ;
    if(flag)
    {
        m_pFrontLabel->setPixmap(*m_pNoUSBPixmap);
        m_pFrontLabel->setVisible(flag);
    }
    else
    {
        m_pFrontLabel->setVisible(flag);
    }
}
void myLable::Enable(bool flag)
{

    if(flag)
    {
        m_bEnable = true;
        m_bFrontLableEnable = true;
        m_bThreadFlag = true;
        m_pDarkScreenAction->setEnabled(m_bEnable);
        m_pRestartAction->setEnabled(m_bEnable);
        m_pStudentShowAction->setEnabled(m_bEnable);
        m_pSendFileAction->setEnabled(m_bEnable);
        m_pRemoteAction->setEnabled(m_bEnable);
    }
    else
    {
        m_bEnable = false;
        m_bFrontLableEnable = false;
        m_pDarkScreenAction->setEnabled(m_bEnable);
        m_pRestartAction->setEnabled(m_bEnable);
        m_pStudentShowAction->setEnabled(m_bEnable);
        m_pSendFileAction->setEnabled(m_bEnable);
        m_pRemoteAction->setEnabled(m_bEnable);
        memset(m_strIP,0,IPLEN);
        memset(m_strName,0,NAMELEN);
        m_iSocket = 0;
        m_bThreadFlag = false;
    }
}

void myLable::SetIP(const char *ip)
{
    if(ip == NULL)
        return ;
    strncpy(m_strIP,ip,IPLEN);
}
void myLable::SetName(const char *name)
{
    if(name == NULL)
        return ;
    strncpy(m_strName,name,NAMELEN);
}
void myLable::SetSocket(int sock)
{
    if(sock <= 0)
        return ;
    m_iSocket = sock;
}
void myLable::mousePressEvent(QMouseEvent *ev)
{
    qDebug("------------------");
    if(!m_bMousePressFlag)
    {
        //this->setFrameShape (QFrame::Box);
        //this->setStyleSheet("border: 5px solid  #ff0000");
    }
    else
    {
        //this->setFrameShape (QFrame::NoFrame);
        //this->setStyleSheet("border: 5px solid  #ffff00");
    }
    m_bMousePressFlag = !m_bMousePressFlag;
}
void myLable::mouseDoubleClickEvent(QMouseEvent *ev)
{
    if(m_bEnable)
    {
        qDebug("asdfasdg");
        m_bDoubleCheckFlag = !m_bDoubleCheckFlag;
        if(m_bDoubleCheckFlag)
        {
            this->move(0,0);
            this->resize(1920,1080);
            this->raise();
        }
        else
        {
            this->move(m_OldPosX,m_OldPosY);
            this->resize(300,200);
        }
    }
    else
    {

    }
}
void myLable::RemberPos(int x, int y)
{
    m_OldPosX = x;
    m_OldPosY = y;
}
