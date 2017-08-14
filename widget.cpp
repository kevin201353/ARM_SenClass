#include "widget.h"
#include "ui_widget.h"
#include "transferdata.h"
#include <QDebug>
#include <QColor>
#include <QGraphicsDropShadowEffect>
#include <QDesktopWidget>
#include <QRect>
#include <QByteArray>

#define PORT 5555
#define FTP_PATH "C:\\ftp_upload\\"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    pressed = 0;

    setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect deskRect = desktopWidget->availableGeometry();
    setGeometry(QRect((deskRect.width() - width()) / 2, 2 - height(), width(), height()));

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(3, 3);
    shadow->setColor(QColor(37, 110, 181));
    shadow->setBlurRadius(12);
    ui->groupBox->setGraphicsEffect(shadow);

    ui->course_name->setText("化学212");
    ui->class_num->setText("初中一班");
    ui->user_name->setText("A25 李某某");

    ui->label->setStyleSheet("background-color: rgb(37,110,181)");

    ui->filePushButton->setStyleSheet("border-image: url(:/images/send_file.png);");
    ui->raiseHandPushButton->setStyleSheet("QPushButton {border-image: url(:/images/raise_hand.png);}"
                                           "QPushButton:enabled {border-image: url(:/images/raise_hand.png);}"
                                           "QPushButton:disabled {border-image: url(:/images/no_raise_hand.png);}");
    ui->cancelPushButton->setStyleSheet("border-image: url(:/images/cancel_raise_hand.png);");
    ui->networkLabel->setStyleSheet("QLabel {border-image: url(:/images/network.png);}"
                                    "QLabel:enabled {border-image: url(:/images/network.png);}"
                                    "QLabel:disabled {border-image: url(:/images/no_network.png);}");
    ui->USBLabel->setStyleSheet("QLabel {border-image: url(:/images/usb.png);}"
                                "QLabel:enabled {border-image: url(:/images/usb.png);}"
                                "QLabel:disabled {border-image: url(:/images/no_usb.png);}");
    ui->showPushButton->setStyleSheet("QPushButton {border-image: url(:/images/show.png);}"
                                      "QPushButton:hover {border-image: url(:/images/hover_show.png);}");
    ui->exitShowPushButton->setStyleSheet("border-image: url(:/images/exit_show.png);");

#if 0
    bs = NULL;
    bs = new BlackScreen();
//    bs->showFullScreen();
#endif

    sf = NULL;
    sf = new SendFile();

    cn = NULL;
    cn = new CallName();
//    cn->showFullScreen();

    QNetworkRequest request;
    request.setUrl(QUrl("http://www.baidu.com"));

    QNetworkAccessManager *qnam = new QNetworkAccessManager();
    reply = qnam->get(request);

    connect(reply, SIGNAL(finished()), this, SLOT(replyFinished()));
    connect(reply, SIGNAL(readyRead()), this, SLOT(on_readyRead()));
}

Widget::~Widget()
{
    if(cn)
        delete cn;
    if(sf)
        delete sf;
//    if(bs)
//        delete bs;
    delete ui;
}

void Widget::replyFinished()
{

}

void Widget::on_readyRead()
{
    qDebug("qqqq");
    QByteArray data = reply->readAll();
    qDebug("hhhh");
    qDebug("data: %s", data.data());
    qDebug("wwww");
}

void Widget::enterEvent(QEvent *)
{
    move(x(), 0);
}

void Widget::leaveEvent(QEvent *)
{
    move(x(), 2 - height());
}

void Widget::disableRaiseHand()
{
    ui->raiseHandPushButton->setEnabled(false);
    qDebug("disabled");
}

void Widget::enableRaiseHand()
{
    ui->raiseHandPushButton->setEnabled(true);
    qDebug("enabled");
}

void Widget::send_file_done()
{
    //此处需将文件名sf->fileName发送给教师端
    ui->filePushButton->setStyleSheet("border-image: url(:/images/send_file.png);");
    pressed = 0;
}

void Widget::on_filePushButton_clicked()
{
    if(0 == pressed)
    {
        ui->filePushButton->setStyleSheet("border-image: url(:/images/pressed_send_file.png);");
        sf->show();
    }
    else
    {
        ui->filePushButton->setStyleSheet("border-image: url(:/images/send_file.png);");
        sf->close();
    }
    pressed ^= 1;
    connect(sf, SIGNAL(finished()), this, SLOT(send_file_done()));
}

void Widget::on_raiseHandPushButton_clicked()
{
    if(ui->raiseHandPushButton->isEnabled())
    {
        ui->raiseHandPushButton->hide();
        ui->cancelPushButton->show();
        //give signal to teacher
    }
}

void Widget::on_cancelPushButton_clicked()
{
    ui->cancelPushButton->hide();
    ui->raiseHandPushButton->show();
    //give signal to teacher
}

void Widget::on_exitShowPushButton_clicked()
{
    ui->exitShowPushButton->hide();
    ui->showPushButton->show();
    //give signal to teacher
}

void Widget::on_showPushButton_clicked()
{
    ui->showPushButton->hide();
    ui->exitShowPushButton->show();
    //give signal to teacher
}
