#include "waitstu.h"
#include "ui_waitstu.h"
#include <QPainter>
#include <QDesktopWidget>
#include <QDebug>
#include <QPalette>

#define DESKTOPMAP   ":/image1/50_22.png"

waitstu::waitstu(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::waitstu)
{
    ui->setupUi(this);
    this->setAutoFillBackground(true);
    this->setWindowFlags(this->windowFlags()& ~Qt::WindowMinMaxButtonsHint);
    QDesktopWidget *desktopWidget = QApplication::desktop();
    QRect deskRect = desktopWidget->availableGeometry();
    this->setGeometry(QRect((deskRect.width() - width())/2, (deskRect.height() - this->height())/2, width(), height()));
//    QPixmap pixmap(DESKTOPMAP);
//    QPalette palette = this->palette();
//    palette.setBrush(QPalette::Background, QBrush(pixmap));
//    this->setPalette(palette);
    this->setStyleSheet("QFrame#waitstu{ background-image:url(:/image1/50_22.png)}");
    this->showFullScreen();
}

waitstu::~waitstu()
{
    delete ui;
}

void waitstu::paintEvent(QPaintEvent *event)
{
    //QPixmap pixmap(DESKTOPMAP);
    QPainter painter(this);
   // painter.drawPixmap(0,0,width(),height(),QPixmap(DESKTOPMAP));
}
