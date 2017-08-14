#include "setroomseatdialog.h"
#include "ui_setroomseatdialog.h"
#include "include.h"
#include "ryanConfig.h"

SetRoomSeatDialog::SetRoomSeatDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetRoomSeatDialog)
{
    ui->setupUi(this);
    m_Font.setPixelSize(15);
    this->setFont(m_Font);
    m_pOKPushButton = ui->OKpushButton;
    m_pCancelPushButton = ui->CancelpushButton;
    m_pRoomNumLabel = ui->RoomNumlabel;
    m_pSeatNumLabel = ui->SeatNumlabel;
    m_pRoomNumLineEdit = ui->RoomNumlineEdit;
    m_pSeatNumLineEdit = ui->SeatNumlineEdit;
   // m_pRoomNumLabel->setFont(m_Font);
    m_pRoomNumLabel->setText("教室号");
    //m_pSeatNumLabel->setFont(m_Font);
    m_pSeatNumLabel->setText("座位号");
    m_pOKPushButton->setText("确定");
    m_pCancelPushButton->setText("取消");
}

SetRoomSeatDialog::~SetRoomSeatDialog()
{
    delete ui;
}

void SetRoomSeatDialog::on_OKpushButton_clicked()
{
    m_pOKPushButton->setEnabled(false);
    memset(g_strRoomNum,0,100);
    memset(g_strSeatNum,0,20);
    strncpy(g_strRoomNum,m_pRoomNumLineEdit->text().toStdString().c_str(),100);
    strncpy(g_strSeatNum,m_pSeatNumLineEdit->text().toStdString().c_str(),20);
    g_pLog->WriteLog(0,"Set Room Num:%s Sear Num:%s",g_strRoomNum,g_strSeatNum);
    char TempBuf[1024];
    char JsonBuf[10240];
    bool Recode = false;
    memset(JsonBuf,0,10240);
    memset(TempBuf,0,1024);
    sprintf(TempBuf,"/service/aps/config?id=%s&roomName=%s&seat=%s&reboot=false", g_strTerminalID, g_strRoomNum, g_strSeatNum);
    g_pLog->WriteLog(0,"UP SeatNum RoomNum:%s",TempBuf);
    //qDebug("TempBuf:%s",TempBuf);
    g_pMyHttp->Get(TempBuf);
    g_pMyHttp->GetData(JsonBuf);
    //qDebug("UP SeatNum RoomNum%s\n",JsonBuf);
    g_pLog->WriteLog(0,"Recv Json:%s",JsonBuf);
    g_pJson->Parse(JsonBuf);
    g_pJson->ReadJson(&Recode,"success");
    if(Recode)
    {
        WriteConfigString(CONFIGNAME,"ROOM","ClassName",g_strRoomNum);
        WriteConfigString(CONFIGNAME,"ROOM","SeatName",g_strSeatNum);
        this->close();
    }
    else
    {
        m_pRoomNumLineEdit->setText("设置失败");
        m_pSeatNumLineEdit->setText("设置失败");
        m_pOKPushButton->setEnabled(false);
        return ;
    }
    m_pOKPushButton->setEnabled(true);
}

void SetRoomSeatDialog::on_CancelpushButton_clicked()
{
    this->close();
}
void SetRoomSeatDialog::SetRoomSeatText(char *Room, char *Seat)
{
    m_pRoomNumLineEdit->setText(Room);
    m_pSeatNumLineEdit->setText(Seat);
}

void SetRoomSeatDialog::on_RoomNumlineEdit_textChanged(const QString &arg1)
{
    m_pOKPushButton->setEnabled(true);
}

void SetRoomSeatDialog::on_SeatNumlineEdit_textChanged(const QString &arg1)
{
    m_pOKPushButton->setEnabled(true);
}
