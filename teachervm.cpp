#include "teachervm.h"

TeacherVM::TeacherVM(QWidget *parent, const char *name,int status):QLabel(parent),m_strTeachetVMName(name),m_iStatus(status)
{

    this->setMinimumSize(264,256);
    this->setMaximumSize(264,256);
    //this->setStyleSheet("background-color: rgb(0,0,127)");
    this->setPixmap(QPixmap(BACKGROUNDPNG));
    m_Font.setPixelSize(16);
    m_pVMNameQLabel = new QLabel(this);
    m_pVMNameQLabel->setFont(m_Font);
    m_pVMNameQLabel->setText(m_strTeachetVMName);
    m_pVMNameQLabel->setAlignment(Qt::AlignCenter);
    m_pVMNameQLabel->move(0,0);
    m_pVMNameQLabel->setMinimumSize(264,30);
    m_pVMNameQLabel->setMaximumSize(264,30);

    m_pDeskTopQLabel = new QLabel(this);
    m_pDeskTopQLabel->setMinimumSize(247,168);
    m_pDeskTopQLabel->setMaximumSize(247,168);
    m_pDeskTopQLabel->move(8,30);

    m_pStartPushButton = new QPushButton(this);
    m_pStartPushButton->setFlat(false);
    m_pStartPushButton->setMinimumSize(15,20);
    m_pStartPushButton->setMaximumSize(15,20);
    m_pStartPushButton->move(8,218);

    m_pStopPushButton = new QPushButton(this);
    m_pStopPushButton->setFlat(false);
    m_pStopPushButton->setMinimumSize(16,16);
    m_pStopPushButton->setMaximumSize(16,16);
    m_pStopPushButton->move(45,220);

    m_pRestartPushButton = new QPushButton(this);
    m_pRestartPushButton->setMinimumSize(24,24);
    m_pRestartPushButton->setMaximumSize(24,24);

    m_pRestartPushButton->move(88,215);

    m_pConnectPushButton = new QPushButton(this);
    m_pConnectPushButton->setMinimumSize(30,30);
    m_pConnectPushButton->setMaximumSize(30,30);

    m_pConnectPushButton->move(200,210);
    SetStatus(m_iStatus);
}

void TeacherVM::SetStatus(int status)
{
    switch(status)
    {
    case CLOSE:
        m_pDeskTopQLabel->setPixmap(QPixmap(DESKTOPCLOSEPNG));
        m_pStartPushButton->setStyleSheet(QStringLiteral(STARTENABLE));
        m_pStartPushButton->setEnabled(true);
        m_pStopPushButton->setStyleSheet(QStringLiteral(STOPDISABLE));
        m_pStopPushButton->setEnabled(false);
        m_pRestartPushButton->setStyleSheet(QStringLiteral(RESTDISABLE));
        m_pRestartPushButton->setEnabled(false);
        m_pConnectPushButton->setStyleSheet(QStringLiteral(CONNECTDISABLE));
        m_pConnectPushButton->setEnabled(false);
        break;
    case RESTART:
        m_pDeskTopQLabel->setPixmap(QPixmap(DESKTOPRESTART));
        m_pStartPushButton->setStyleSheet(QStringLiteral(STARTDISABLE));
        m_pStartPushButton->setEnabled(false);
        m_pStopPushButton->setStyleSheet(QStringLiteral(STOPENABLE));
        m_pStopPushButton->setEnabled(true);
        m_pRestartPushButton->setStyleSheet(QStringLiteral(RESTDISABLE));
        m_pRestartPushButton->setEnabled(false);
        m_pConnectPushButton->setStyleSheet(QStringLiteral(CONNECTDISABLE));
        m_pConnectPushButton->setEnabled(false);
        break;
    case USING:
        m_pDeskTopQLabel->setPixmap(QPixmap(DESKTOPUSING));
        m_pStartPushButton->setStyleSheet(QStringLiteral(STARTDISABLE));
        m_pStartPushButton->setEnabled(false);
        m_pStopPushButton->setStyleSheet(QStringLiteral(STOPENABLE));
        m_pStopPushButton->setEnabled(true);
        m_pRestartPushButton->setStyleSheet(QStringLiteral(RESTENABLE));
        m_pRestartPushButton->setEnabled(true);
        m_pConnectPushButton->setStyleSheet(QStringLiteral(CONNECTENABLE));
        m_pConnectPushButton->setEnabled(true);
        break;
    case ERROR:
        m_pDeskTopQLabel->setPixmap(QPixmap(DESKTOPERROR));
        m_pStartPushButton->setStyleSheet(QStringLiteral(STARTDISABLE));
        m_pStartPushButton->setEnabled(false);
        m_pStopPushButton->setStyleSheet(QStringLiteral(STOPDISABLE));
        m_pStopPushButton->setEnabled(false);
        m_pRestartPushButton->setStyleSheet(QStringLiteral(RESTDISABLE));
        m_pRestartPushButton->setEnabled(false);
        m_pConnectPushButton->setStyleSheet(QStringLiteral(CONNECTDISABLE));
        m_pConnectPushButton->setEnabled(false);
        break;
    case WARNING:
        m_pDeskTopQLabel->setPixmap(QPixmap(DESKTOPWARNING));
        m_pStartPushButton->setStyleSheet(QStringLiteral(STARTDISABLE));
        m_pStartPushButton->setEnabled(false);
        m_pStopPushButton->setStyleSheet(QStringLiteral(STOPENABLE));
        m_pStopPushButton->setEnabled(true);
        m_pRestartPushButton->setStyleSheet(QStringLiteral(RESTENABLE));
        m_pRestartPushButton->setEnabled(true);
        m_pConnectPushButton->setStyleSheet(QStringLiteral(CONNECTENABLE));
        m_pConnectPushButton->setEnabled(true);
        break;
    }

}
