#ifndef MYLABLE_H
#define MYLABLE_H

#include <QLabel>
#include <QWidget>
#include <QPixmap>
#include <QAction>
#include <pthread.h>
#include <unistd.h>
#include <QMouseEvent>

#define NORMALPIC           "/home/pengjian/QtProject/CloudClassRoom/image/Using.png"
#define UPHANDPIC           "/home/pengjian/QtProject/CloudClassRoom/image/2.png"
//#define DOWNHANDPIC       "/home/pengjian/QtProject/CloudClassRoom/image/2.png"
#define NONETWORKPIC        "/home/pengjian/QtProject/CloudClassRoom/image/2.png"
#define DARKSCREENPIC       "/home/pengjian/QtProject/CloudClassRoom/image/2.png"
#define NOUSBPIC            "/home/pengjian/QtProject/CloudClassRoom/image/2.png"

#define DARKSCREENICON      "/home/pengjian/QtProject/CloudClassRoom/image/StartIcon.png"
#define RESTARTICON         "/home/pengjian/QtProject/CloudClassRoom/image/ShutDownIcon.png"
#define STUDENTSHOWICON     "/home/pengjian/QtProject/CloudClassRoom/image/RestDeskTopIcon.png"
#define SENDFILEICON        "/home/pengjian/QtProject/CloudClassRoom/image/RestVMIcon.png"
#define REMOTEACTIONICON    "/home/pengjian/QtProject/CloudClassRoom/image/DesktopHelp.png"
#define IPLEN           20
#define NAMELEN         50

void *ThreadFun(void *param);

class myLable:public QLabel
{
    Q_OBJECT

public:
    myLable(QWidget *parent = NULL);
    ~myLable();
    void ShowUpHand(bool flag);
    void ShowNoNetwork(bool flag);
    void ShoeDarkScreen(bool flag);
    void ShowNoUSB(bool flag);
    void Enable(bool flag);
    void SetIP(const char *ip);
    void SetName(const char *name);
    void SetSocket(int sock);
    void RemberPos(int x,int y);
private:
    QLabel *m_pFrontLabel;
    QPixmap *m_pNormalPixmap;
    QPixmap *m_pUphandPixmap;
    //QPixmap *m_pDownhandPixmap;
    QPixmap *m_pNoNetworkPixmap;
    QPixmap *m_pDarkScreenPixmap;
    QPixmap *m_pNoUSBPixmap;
    QAction *m_pDarkScreenAction;
    QAction *m_pRestartAction;
    QAction *m_pStudentShowAction;
    QAction *m_pSendFileAction;
    QAction *m_pRemoteAction;
    bool m_bEnable;
    bool m_bFrontLableEnable;
    bool m_bDoubleCheckFlag;
    bool m_bMousePressFlag;
    char m_strIP[IPLEN];
    char m_strName[NAMELEN];
    int m_iSocket;
    int m_OldPosX;
    int m_OldPosY;
    pthread_t m_pid;
public:
    bool m_bThreadFlag;
    bool m_bExit;
private slots:
    void DarkScreenFun();
    void RestartFun();
    void StudentShowFun();
    void SendFileFun();
    void RemoteFun();
private:
    void mousePressEvent(QMouseEvent *ev);
    void mouseDoubleClickEvent(QMouseEvent *ev);
};

#endif // MYLABLE_H
