#include "stwidget.h"
#include "ui_stwidget.h"
#include <QPainter>

#define STUUI_BG  "/usr/local/shencloud/image1/bj.png"
#define WINDOWSXP    "/usr/local/shencloud/image1/windowsxp.png"
#define WINDOWS7     "/usr/local/shencloud/image1/windows7.png"
#define LINUX        "/usr/local/shencloud/image1/linux.png"

stwidget::stwidget(QWidget *parent):
    QWidget(parent),
    ui(new Ui::stuwidgetUI)
{
    ui->setupUi(this);
    this->setAutoFillBackground(true);
    this->setWindowFlags(Qt::FramelessWindowHint);
    QPixmap pixmap(STUUI_BG);
    QPixmap pix_linux(LINUX);
     //QPixmap  pix_linux(LINUX);
    this->resize(pixmap.width(), pixmap.height());
    int x2 = pixmap.width()/2 - pix_linux.width()/2;
    int y2 = pixmap.height()/2 - pix_linux.height()/2;
    ui->btn_class2->move(x2, y2);
}

stwidget::~stwidget()
{
    delete ui;
}

void stwidget::paintEvent(QPaintEvent *event)
{
    QPixmap pixmap(STUUI_BG);
    QPainter painter(this);
    painter.drawPixmap(0,0,width(),height(),QPixmap(STUUI_BG));
}
