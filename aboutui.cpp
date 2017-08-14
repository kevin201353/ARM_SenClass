#include "aboutui.h"
#include "ui_aboutui.h"
#include "buildtime.h"
#include <QDesktopWidget>
#include <QRect>
#include "global.h"
#include <QDebug>

AboutUI::AboutUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AboutUI)
{
    ui->setupUi(this);
    this->setAutoFillBackground(true);
    this->setWindowModality(Qt::ApplicationModal);
    this->setWindowFlags(Qt::FramelessWindowHint);
    widget_resize();
    QDesktopWidget *desktopWidget = QApplication::desktop();
    QRect deskRect = desktopWidget->availableGeometry();
    this->setGeometry(QRect((deskRect.width() - width())/2, (deskRect.height() - this->height())/2, width(), height()));
    ui->frame->setStyleSheet("background-color: rgb(255,153,19)");
    QString strsoft = "Version: V2.1_";
    strsoft += new QString(buildtime);
#ifdef _DEBUG_PRINT
    qDebug()<< "aboutui version : " << strsoft;
#endif
    ui->label_version->setText(strsoft);
    connect(ui->btn_close, SIGNAL(clicked(bool)), this, SLOT(On_Close()));
}

AboutUI::~AboutUI()
{
    delete ui;
}


void AboutUI::keyPressEvent(QKeyEvent  *event)
{

}

void AboutUI::On_Close()
{
    close();
}

void AboutUI::widget_resize()
{
    int scr_width = QApplication::desktop()->width();
    int scr_height = QApplication::desktop()->height();
    float factor_x = (float)scr_width/g_scr_old_width;
    float factor_y = (float)scr_height/g_scr_old_height;
    my_resize(this, factor_x, factor_y);
    my_resize(ui->btn_close, factor_x, factor_y);
    my_resize(ui->frame, factor_x, factor_y);
    my_resize(ui->label_version, factor_x, factor_y);
}
