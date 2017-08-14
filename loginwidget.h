#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLCDNumber>
#include <QLineEdit>
#include <QKeyEvent>
#include <QTimer>
#include <QDateTime>
#include <QPainter>
#include <QFont>
#include <QPalette>
#include <QGroupBox>
#include "setform.h"
#include "classmould.h"
#include "classnameconfig.h"
#include "netconfig.h"
#include "setroomseatdialog.h"
#include "mydialog.h"
//linux
#include <sys/types.h>
#include <sys/stat.h>
#include "qthread.h"
#include "waitstudialog.h"
#include "waitstu2.h"
#include "passwordui.h"
#include "mqmsgprocess.h"

#define LOGOPNG      "/usr/local/shencloud/image1/Logo_Student.png"
#define LOGOPNG2     "/usr/local/shencloud/image1/LeTOS_03.png"
#define DESKTOPMAP  "/usr/local/shencloud/image1/Top_Student1.png"
#define ENTERPUSH   "/usr/local/shencloud/image1/enter_new.png"
//#define USRLOGO      "/usr/local/shencloud/pengjian/QtProject/CloudClassRoom/image1/Username.png"
//#define PASSWDLOGO  "/usr/local/shencloud/pengjian/QtProject/CloudClassRoom/image1/Userpasswd.png"

#define SETLOGO         "QPushButton{border-image: url(/usr/local/shencloud/image1/Set.png);}"\
                        "QPushButton:hover{border-image: url(/usr/local/shencloud/image1/Set_hover.png);}"\
                        "QPushButton:pressed{border-image: url(/usr/local/shencloud/image1/SetPress.png)}"

#define INFORMATIONLOGO     "QPushButton{border-image: url(/usr/local/shencloud/image1/Information.png);}"\
                            "QPushButton:hover{border-image: url(/usr/local/shencloud/image1/InformationPress.png);}"\
                            "QPushButton:pressed{border-image: url(/usr/local/shencloud/image1/Information.png)}"

#define SERVERMANLOGO   "QPushButton{border-image: url(/usr/local/shencloud/image1/ServerMan.png);}"\
                        "QPushButton:hover{border-image: url(/usr/local/shencloud/image1/ServerManPress.png);}"\
                        "QPushButton:pressed{border-image: url(/usr/local/shencloud/image1/ServerMan.png)}"

#define LOGINLOGO       "QPushButton{border-image: url(/usr/local/shencloud/image1/Login.png);}"\
                        "QPushButton:hover{border-image: url(/usr/local/shencloud/image1/LoginPress.png);}"\
                        "QPushButton:pressed{border-image: url(/usr/local/shencloud/image1/Login.png)}"

#define SHUTDOWNLOGO    "QPushButton{border-image: url(/usr/local/shencloud/image1/Shutdown.png);}"\
                        "QPushButton:hover{border-image: url(/usr/local/shencloud/image1/Shutdown_hover.png);}"\
                        "QPushButton:pressed{border-image: url(/usr/local/shencloud/image1/ShutdownPress.png)}"

#define RIGHTPNG        "QPushButton{border-image: url(/usr/local/shencloud/image1/Right.png);}"\
                        "QPushButton:hover{border-image: url(/usr/local/shencloud/image1/RightPress.png);}"\
                        "QPushButton:pressed{border-image: url(/usr/local/shencloud/image1/Right.png)}"

#define LEFTPNG         "QPushButton{border-image: url(/usr/local/shencloud/image1/Left.png);}" \
                        "QPushButton:hover{border-image: url(/usr/local/shencloud/image1/LeftPress.png);}" \
                        "QPushButton:pressed{border-image: url(/usr/local/shencloud/image1/Left.png)}"

#define ENTER           "QPushButton{border-image: url(/usr/local/shencloud/image1/enter_new.png);}" \
                        "QPushButton:hover{border-image: url(/usr/local/shencloud/image1/enter_new_move.png);}" \
                        "QPushButton:pressed{border-image: url(/usr/local/shencloud/image1/enter_new_pass.png);}"\
                        "QPushButton{color:rgb(255,255,255);}"

namespace Ui {
class LoginWidget;
}

class LoginWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWidget(QWidget *parent = 0);
    ~LoginWidget();
    ClassNameConfig *m_pClassNameConfig;
    //Group   *m_pGroupWigdet;
    SetForm     *m_pSetForm;
    MyDialog    *m_pMyDialog;
    //WaitstuDialog  *m_waitstuDialog;
    waitstu2    *m_waitstu;
    void Thread();
    void SetChecked();
    void createPipe();
    void initConfig();
    void SetEnable(bool flag);
    void rebootAmqThrd();
    void widget_resize();
    int width() const {
        return m_width;
    };
    int height() const {
        return m_height;
    };
private slots:
    void OnTimeOut();
    void on_SetpushButton_clicked();

    void on_ShutdownpushButton_clicked();

    void on_InformationpushButton_clicked();

    void on_LeftPushButton();

    void on_RightPushButton();

private slots:
    void on_LabelChecked();
    void on_EnterPushButton();
    void on_LableDoubleClicked();
    void UpdateNetOffDialog();
    void HideNetOffDialog();
    void on_ShowPassUI();
private:
    Ui::LoginWidget *ui;
    QLabel *m_pLogoQLable;
    QLCDNumber *m_pTimeLCDNumber;
    QLabel  *m_pSoftInforLabel;
    QPushButton *m_pInformationPushButton;
    QPushButton *m_pSetPushButton;
    QPushButton *m_pServerManPushButton;
    QPushButton *m_pShutdownPushButton;
    QTimer      *m_pTimer;
    QDateTime       *m_pTime;
    QFont       m_Font;

    QPushButton *m_pLeftPushButton;
    QPushButton *m_pRightPushButton;
    QPushButton *m_pEnterPushButton;
    QLabel      *m_pLogoQLable2;
    ClassMould *m_pClassName1;
    ClassMould *m_pClassName2;
    ClassMould *m_pClassName3;
    SetRoomSeatDialog *m_pSetRoomSeat;
    bool m_bExitThread;
    LoadingFrame * g_Loadingwnd;
    qthread    *m_pqthread;
    PasswordUI *m_passui;
    int     m_width;
    int     m_height;
protected:
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent  *event);
};

#endif // LOGINWIDGET_H
