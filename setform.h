#ifndef SETFORM_H
#define SETFORM_H

#include <QWidget>
#include <QPushButton>
#include <QTabWidget>
#include <QLineEdit>
#include <QRadioButton>
#include <QGroupBox>
#include <QRegExpValidator>
#include <QFont>
#include "loadingframe.h"
#include "QMessageBox"
#include <QMouseEvent>
#include <QComboBox>
#include "mydialog.h"
#include <QLabel>
#include <qthread.h>

namespace Ui {
class SetForm;
}

class SetForm : public QWidget
{
    Q_OBJECT

public:
    explicit SetForm(QWidget *parent = 0);
    ~SetForm();
    void SetNoDHCPSetDNSTrue(){m_bDHCP_DNS = false;m_pNoDHCPSetDNS->setChecked(true);on_ManualSetDNSradioButton_clicked(true);}
    void SetNoDHCPSetIPTrue(){m_bDHCP_IP = false;m_pNoDHCPSetIP->setChecked(true);on_ManualSetIPradioButton_clicked(true);}
    void SetDHCPSetDNSTrue(){m_bDHCP_DNS = true;m_pAutoSetDNS->setChecked(true);on_AutoSetDNSradioButton_clicked(true);}
    void SetDHCPSetIPTrue(){m_bDHCP_IP = true;m_pAutoSetIP->setChecked(true);on_AutoSetIPradioButton_clicked(true);}
    void SetIPLineEdit(const char *ip);
    void SetMaskIPLineEdit(const char *ip);
    void SetGateWayIPLineEdit(const char *ip);
    void SetDNS1LineEdit(const char *ip);
    void SetDNS2LineEdit(const char *ip);
    void SetDNS3LineEdit(const char *ip);
    bool GetDNSFlag(){return m_bDHCP_DNS;}
    bool GetNETFlag(){return m_bDHCP_IP;}
    void SetLoadingFram(bool falg)
    {
        if(falg)
            m_pLoagingFrame->show();
        else
            m_pLoagingFrame->hide();
    }
    void SetSalveEnable(bool falg)
    {
        m_pSavePushButton->setEnabled(falg);
    }
    void saveRoomSeat();
private slots:
    void on_CancelpushButton_clicked();

    void on_AutoSetIPradioButton_clicked(bool checked);

    void on_ManualSetIPradioButton_clicked(bool checked);

    void on_AutoSetDNSradioButton_clicked(bool checked);

    void on_ManualSetDNSradioButton_clicked(bool checked);

    void on_SavepushButton_clicked();

    void on_ClassRoomNumllineEdit_textEdited(const QString &arg1);

    void on_SeatNumlineEdit_textEdited(const QString &arg1);

    void on_ServerIPlineEdit_textChanged(const QString &arg1);

    void on_IPlineEdit_textChanged(const QString &arg1);

    void on_ChildIPlineEdit_textChanged(const QString &arg1);

    void on_GetWaylineEdit_textChanged(const QString &arg1);

    void on_DNS1lineEdit_textChanged(const QString &arg1);

    void on_DNS2lineEdit_textChanged(const QString &arg1);

    void on_DNS3lineEdit_textChanged(const QString &arg1);

    void exit_widget();
    void on_returnPressed();
    void update();
protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    Ui::SetForm *ui;
    QPushButton     *m_pSavePushButton;
    QPushButton     *m_pCancelPushButton;
    QTabWidget    *m_pSetTabWidget;
    QLineEdit       *m_pIPLineEdit;
    QLineEdit       *m_pChildIPLineEdit;
    QLineEdit       *m_pGetWayLineEdit;
    QLineEdit       *m_pDNS1LineEdit;
    QLineEdit       *m_pDNS2LineEdit;
    QLineEdit       *m_pDNS3LineEdit;
    QRadioButton    *m_pAutoSetIP;
    QRadioButton    *m_pNoDHCPSetIP;
    QRadioButton    *m_pAutoSetDNS;
    QRadioButton    *m_pNoDHCPSetDNS;
    QGroupBox       *m_IPGroupBox;
    QGroupBox       *m_DNSGroupBox;
    QRegExpValidator    *m_pRegValue;
    QFont           m_Font;
    LoadingFrame    *m_pLoagingFrame;
    QLineEdit       *m_pServerIPLineEdit;
    bool             m_moving;
    QPoint           m_LastMousePosition;
    QString          m_strPing;
    int              m_pingCount;
public:
    bool            m_bDHCP_IP;
    bool            m_bDHCP_DNS;
    QString         m_strServerIP;
    QComboBox        *m_pRoomNumcomboBox;
    QLineEdit        *m_pSeatLineEdit;
    MyDialog         *m_pMyDialog;
    bool            m_bSetRoomfail;
    QLabel*           m_tiplabel;
    qthreadPing*       m_pqthread;
private:
    bool isInvalidNetmask(const char *subnet);
    //void keyPressEvent(QKeyEvent  *event);
    void widget_resize();
};

#endif // SETFORM_H
