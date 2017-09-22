#include "waitstu2.h"
#include "ui_waitstu2.h"
#include <QPainter>
#include <QDesktopWidget>
#include <QDebug>
#include <QPalette>
#include <QPixmap>
#include "loginwidget.h"
#include "aboutui.h"
#include "global.h"
#include <QLabel>
#include "include.h"

#define DESKTOPMAP3   "/usr/local/shencloud/image1/50_22.png"
#define GIFFILE2  "/usr/local/shencloud/image1/waitting.gif"
#define SETPNG    "/usr/local/shencloud/image1/Set.png"
#define SHUTPNG   "/usr/local/shencloud/image1/Shutdown.png"
#define WAITBG  "/usr/local/shencloud/image1/waitbg.png"

float g_scr_old_width = 1920.0;
float g_scr_old_height = 1080.0;
waitstu2  *m_waitstu = NULL;
int my_resize(QWidget* widget, float factorX, float factorY)
{
    int width = widget->width();
    int height = widget->height();
#ifdef _DEBUG_PRINT
    qDebug() << "width :" << width << "height :" << height;
#endif
    int x = widget->x();
    int y = widget->y();
    int fact_x = (int)(x*factorX);
    int fact_y = (int)(y*factorY);
    int fact_width = (int)(width*factorX);
    int fact_height = (int)(height*factorY);
#ifdef _DEBUG_PRINT
    qDebug() << "fact_x : " << factorX << "fact_y : " << factorY << "fact_width :" << fact_width << "fact_heiht :" << fact_height;
#endif
    widget->setGeometry(fact_x, fact_y, fact_width, fact_height);
}

waitstu2::waitstu2(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::waitstu2)
{
    ui->setupUi(this);
    this->setAutoFillBackground(true);
    this->setWindowModality(Qt::ApplicationModal);
 //   this->setWindowFlags(Qt::WindowStaysOnTopHint);
 //   this->setWindowFlags(this->windowFlags()& ~Qt::WindowMinMaxButtonsHint);
//    QDesktopWidget *desktopWidget = QApplication::desktop();
//    QRect deskRect = desktopWidget->availableGeometry();
//    this->setGeometry(QRect((deskRect.width() - width())/2, (deskRect.height() - this->height())/2, width(), height()));
//    QPixmap pixmap(DESKTOPMAP3);
//    QPalette palette = this->palette();
//    palette.setBrush(QPalette::Background, QBrush(pixmap));
//    this->setPalette(palette);

    /*
    QPixmap pix_bg(WAITBG);
    int bg_width = pix_bg.width();
    int bg_height = pix_bg.height();
    QPixmap pixmap(GIFFILE2);
    m_pMovie = NULL;
    m_pMovie = new QMovie(GIFFILE2);
    m_pMovie->setSpeed(150);
    int width = pixmap.width();
    int height = pixmap.height();
    qDebug() << "width : " << width;
    qDebug() << "height : " << height;
    ui->label_gif->resize(pixmap.width(), pixmap.height());
    ui->label_gif->move(bg_width/2 - width/2,  bg_height/2 - height/2);
    QPixmap  pix_set(SETPNG);
    QPixmap  pix_shut(SHUTPNG);
    ui->btn_Setting->resize(pix_set.width(), pix_set.height());
    ui->btn_shutdown->resize(pix_shut.width(), pix_shut.height());
    ui->btn_Setting->setStyleSheet(QStringLiteral(SETLOGO));
    ui->btn_shutdown->setStyleSheet(QStringLiteral(SHUTDOWNLOGO));
    ui->btn_Setting->move(bg_width - pix_set.width()*3 - 30, bg_height - pix_set.height()*2);
    ui->btn_shutdown->move(bg_width - pix_set.width() - 30, bg_height - pix_set.height()*2);
    ui->label_gif->setMovie(m_pMovie);
    m_pMovie->start();
    */
    widget_resize();
    connect(ui->btn_Setting,SIGNAL(clicked(bool)),this,SLOT(On_Setting()));
    connect(ui->btn_shutdown,SIGNAL(clicked(bool)),this,SLOT(On_Shutdown()));
    CpuThrd *cpuThread = new CpuThrd();
    connect(cpuThread, SIGNAL(NoticeMsg(const QString &)),this,SLOT(On_ShowCpu(const QString &)));
    connect(cpuThread, &CpuThrd::finished, cpuThread, &QObject::deleteLater);
    m_waitstu = this;
    cpuThread->start();
}

waitstu2::~waitstu2()
{
    delete ui;
    if (m_pMovie)
    {
        delete m_pMovie;
        m_pMovie = NULL;
    }
}

void waitstu2::waitstushow()
{
    this->show();
}

void waitstu2::waitstuhide()
{
    this->hide();
}

void waitstu2::paintEvent(QPaintEvent *event)
{
    QPixmap pixmap(WAITBG);
    QPainter painter(this);
    painter.drawPixmap(0,0,width(),height(),QPixmap(WAITBG));
}

void waitstu2::On_Setting()
{
    //qDebug() << "On_Setting.";
    emit wait_showPassUI();
}

void waitstu2::On_Shutdown()
{
    //qDebug() << "On_Shutdown.";
    emit wait_shutdown();
}


void waitstu2::keyPressEvent(QKeyEvent  *event)
{
    //qDebug("---------%8x", event->key());
    if ((event->modifiers() == Qt::ShiftModifier) && (event->key() == Qt::Key_R))
    {
        //qDebug("############# ---- shitf + r .");
        AboutUI *about = new AboutUI();
        about->show();
    }
    QWidget::keyPressEvent(event);
}

void waitstu2::widget_resize()
{
    int scr_width = QApplication::desktop()->width();
    int scr_height = QApplication::desktop()->height();
    float factor_x = (float)scr_width/g_scr_old_width;
    float factor_y = (float)scr_height/g_scr_old_height;
    this->m_width = scr_width;
    this->m_height = scr_height;
    this->resize(width(), height());
    my_resize(this, factor_x, factor_y);
    //label
    ui->label_gif->setScaledContents(true);
    QPixmap pixmap(GIFFILE2);
    m_pMovie = NULL;
    m_pMovie = new QMovie(GIFFILE2);
    m_pMovie->setSpeed(50);
    int width = pixmap.width();
    int height = pixmap.height();
#ifdef _DEBUG_PRINT
    qDebug() << "width : " << width;
    qDebug() << "height : " << height;
#endif
    ui->label_gif->resize(pixmap.width(), pixmap.height());
    my_resize(ui->label_gif, factor_x, factor_y);
    ui->label_gif->move(scr_width/2 - ui->label_gif->width()/2,  scr_height/2 - ui->label_gif->height()/2);
    ui->label_gif->setMovie(m_pMovie);
    m_pMovie->start();
    //set button
    QPixmap  pix_set(SETPNG);
    QPixmap  pix_shut(SHUTPNG);
    ui->btn_Setting->resize(pix_set.width(), pix_set.height());
    ui->btn_shutdown->resize(pix_shut.width(), pix_shut.height());
    ui->btn_Setting->setStyleSheet(QStringLiteral(SETLOGO));
    ui->btn_shutdown->setStyleSheet(QStringLiteral(SHUTDOWNLOGO));
    my_resize(ui->btn_Setting, factor_x, factor_y);
    my_resize(ui->btn_shutdown, factor_x, factor_y);
    ui->btn_Setting->move(scr_width - ui->btn_Setting->width()*3 - 30, scr_height - ui->btn_Setting->height()*2);
    ui->btn_shutdown->move(scr_width - ui->btn_shutdown->width() - 30, scr_height - ui->btn_shutdown->height()*2);

    //debug
    QString fontName = loadFontFromFile("/usr/local/share/fonts/msyh.ttf");
    QFont font(fontName);
    font.setPointSize(12);
    NetConfig netconfig;
    memset(m_strIP,0,20);
    netconfig.GetIPAddr(m_strIP);
    QLabel *labelip = new QLabel(this);
    labelip->resize(200, 30);
    labelip->setFont(font);
    my_resize(labelip, factor_x, factor_y);
    labelip->move(30, (scr_height - labelip->height() * 2));
    QString strsoft(m_strIP);
    labelip->setText(strsoft);
    labelip->show();

    //cpu
    labelcpu = new QLabel(this);
    labelcpu->resize(200, 30);
    labelcpu->setFont(font);
    my_resize(labelcpu, factor_x, factor_y);
    labelcpu->move(labelip->width() + 10, (scr_height - labelcpu->height() * 2));
    labelcpu->setText("");
    labelcpu->show();
}

void waitstu2::On_ShowCpu(const QString &str)
{
    QString tmp = "cpu(%):";
    tmp += str;
    labelcpu->setText(tmp);
}
