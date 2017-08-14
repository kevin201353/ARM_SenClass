#include "classmould.h"
#include "global.h"
#include "log.h"
extern Log *g_pLog;

ClassMould::ClassMould(QWidget *parent,QString name):QLabel(parent),m_strClassName(name)
{
    m_bIsChecked = false;
    //QPixmap  pix_linux(LINUX);
    this->setPixmap(QPixmap(CLASS));
    this->setMinimumSize(270,270);
    this->setMaximumSize(270,270);
    //this->setPixmap(QPixmap(LINUX));
   // this->resize(pix_linux.width(), pix_linux.height());
    m_pShowName = new QLabel(this);
    m_pShowName->setMinimumSize(185,33);
    m_pShowName->setMaximumSize(185,33);
    m_pShowName->move(42,197);
    m_pShowName->setAlignment(Qt::AlignCenter);
    m_Font.setPixelSize(30);
    m_pShowName->setFont(m_Font);
    m_pShowName->setText(m_strClassName);
   // m_pShowName->setText("asdfasdf");
   //this->setStyleSheet("background-color:rgb(23,34,56)");
   //this->move(0, 300);
    m_pClassPic = new QLabel(this);
    m_pClassPic->setPixmap(QPixmap(CLASSPIC));
    m_pClassPic->move(42,20);
}

void ClassMould::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
      if(m_bIsChecked)
      {
          //SetChecked(true);
      }
      else
      {
          //SetChecked(false);
      }
      m_bIsChecked = true;
      emit LabelChecked();
    }
}
void ClassMould::SetClassName(QString name)
{
    m_strClassName = name;
    m_pShowName->setText(m_strClassName);
}
void ClassMould::SetChecked(bool flag)
{
    if(flag)
    {
        this->setPixmap(QPixmap(CLASSENTER));
    }
    else
    {
        this->setPixmap(QPixmap(CLASS));
    }
}
void ClassMould::enterEvent(QEvent *event)
{
    //if(m_bIsChecked)
    //qDebug("EnterEvent[%d]...............................\n",m_bIsChecked);
    if(m_bIsChecked)
    {

    }
    else
    {
        //this->setPixmap(QPixmap(CLASSENTER));
    }
}
void ClassMould::leaveEvent(QEvent *event)
{
    //qDebug("LeaveEvent[%d].................................\n",m_bIsChecked);
    if(m_bIsChecked)
    {

    }
    else
    {
        //this->setPixmap(QPixmap(CLASS));
    }
}
bool ClassMould::IsChecked()
{
    return m_bIsChecked;
}
void ClassMould::SetCheckedFlag(bool flag)
{
    m_bIsChecked = flag;
}

void ClassMould::mouseDoubleClickEvent(QMouseEvent *event)
{
    //qDebug("ClassMould::mouseMoveEvent  !!!!!!! .\n");
    g_pLog->WriteLog(0,"ClassMould::mouseDoubleClickEvent  !!!!!!! .\n");
//    emit LabelDoubleclicked();
//    ReportMsg reportmsg;
//    reportmsg.action = USER_WAITINGDLG_SHOW;
//    call_msg_back(msg_respose, reportmsg);

//    reportmsg.action = USER_MSG_FREESTUDY;
//    call_msg_back(msg_respose, reportmsg);
}
