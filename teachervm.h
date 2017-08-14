#ifndef TEACHERVM_H
#define TEACHERVM_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QString>
#include <QFont>
#define BACKGROUNDPNG  "/home/pengjian/QtProject/CloudClassRoom/image/BackGround.png"
#define DESKTOPCLOSEPNG      "/home/pengjian/QtProject/CloudClassRoom/image/Close.png"
#define DESKTOPRESTART      "/home/pengjian/QtProject/CloudClassRoom/image/Restart.png"
#define DESKTOPUSING        "/home/pengjian/QtProject/CloudClassRoom/image/Using.png"
#define DESKTOPERROR        "/home/pengjian/QtProject/CloudClassRoom/image/Error.png"
#define DESKTOPWARNING      "/home/pengjian/QtProject/CloudClassRoom/image/Warning.png"

#define STARTENABLE     "QPushButton{border-image: url(/home/pengjian/QtProject/CloudClassRoom/image/StartEnable.png);}"\
                        "QPushButton:hover{border-image: url(/home/pengjian/QtProject/CloudClassRoom/image/StartEnablePress.png);}"\
                        "QPushButton:pressed{border-image: url(/home/pengjian/QtProject/CloudClassRoom/image/StartEnable.png)}"

#define STOPENABLE      "QPushButton{border-image: url(/home/pengjian/QtProject/CloudClassRoom/image/StopEnable.png);}"\
                        "QPushButton:hover{border-image: url(/home/pengjian/QtProject/CloudClassRoom/image/StopEnablePress.png);}"\
                        "QPushButton:pressed{border-image: url(/home/pengjian/QtProject/CloudClassRoom/image/StopEnable.png)}"

#define RESTENABLE      "QPushButton{border-image: url(/home/pengjian/QtProject/CloudClassRoom/image/ReastEnable.png);}"\
                        "QPushButton:hover{border-image: url(/home/pengjian/QtProject/CloudClassRoom/image/ReastEnablePress.png);}"\
                        "QPushButton:pressed{border-image: url(/home/pengjian/QtProject/CloudClassRoom/image/ReastEnable.png)}"


#define CONNECTENABLE   "QPushButton{border-image: url(/home/pengjian/QtProject/CloudClassRoom/image/ConnectEnable.png);}"\
                        "QPushButton:hover{border-image: url(/home/pengjian/QtProject/CloudClassRoom/image/ConnectEnablePress.png);}"\
                        "QPushButton:pressed{border-image: url(/home/pengjian/QtProject/CloudClassRoom/image/ConnectEnable.png)}"

#define STARTDISABLE         "border-image: url(/home/pengjian/QtProject/CloudClassRoom/image/StartDisable.png);"
#define STOPDISABLE         "border-image: url(/home/pengjian/QtProject/CloudClassRoom/image/StopDisable.png);"
#define RESTDISABLE         "border-image: url(/home/pengjian/QtProject/CloudClassRoom/image/ReastDisable.png);"
#define CONNECTDISABLE         "border-image: url(/home/pengjian/QtProject/CloudClassRoom/image/ConnectDisable.png);"

#define CLOSE   1
#define RESTART 2
#define USING   3
#define ERROR   4
#define WARNING  5

class TeacherVM:public QLabel
{
public:
    TeacherVM(QWidget *parent, const char *name, int status);
    QLabel  *m_pVMNameQLabel;
    QLabel  *m_pDeskTopQLabel;
    QPushButton *m_pStartPushButton;
    QPushButton *m_pStopPushButton;
    QPushButton *m_pRestartPushButton;
    QPushButton *m_pConnectPushButton;
    QString     m_strTeachetVMName;
    int m_iStatus;
    QFont   m_Font;
public:
    void SetStatus(int status);
};

#endif // TEACHERVM_H
