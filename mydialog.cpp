#include "mydialog.h"
#include "ui_mydialog.h"
#include <QDesktopWidget>
#include <QDebug>

MyDialog::MyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    //this->setWindowTitle("提示");
    m_pTextLabel = ui->Textlabel;
    m_pOKPushButton = ui->OKpushButton;
    m_pCancelPushButton = ui->CancelpushButton;
    this->setWindowModality(Qt::ApplicationModal);
//    this->setMinimumSize(400,200);
//    this->setMaximumSize(400,200);
    m_show = false;
    ui->label_title->setText("提示");
    m_Font.setPixelSize(20);
    m_pTextLabel->setFont(m_Font);
    m_pOKPushButton->setMinimumSize(80,30);
    m_pOKPushButton->setMaximumSize(80,30);
    //m_pOKPushButton->setStyleSheet(DIALOGOKPNG);
    m_pOKPushButton->setText("确定");
 #ifdef CCLOUD
    m_pOKPushButton->setStyleSheet("background-color: rgb(255,153,19); color:rgb(255,255,255)");
    m_pCancelPushButton->setStyleSheet("background-color: rgb(255,153,19); color:rgb(255,255,255)");
 #else
    m_pOKPushButton->setStyleSheet("background-color: rgb(5,149,245); color:rgb(255,255,255)");
    m_pCancelPushButton->setStyleSheet("background-color: rgb(5,149,245); color:rgb(255,255,255)");
 #endif
    m_pCancelPushButton->setMinimumSize(80,30);
    m_pCancelPushButton->setMaximumSize(80,30);
    m_pCancelPushButton->setText("取消");
    //m_pCancelPushButton->setStyleSheet(DIALOGCANCELPNG);
   // this->setAttribute(Qt::WA_TranslucentBackground);
   // this->setStyleSheet("background:rgba(255,255,255,0);border:none;");
   // ui->title_bar->setStyleSheet("background-color: rgb(0,123,214)");
   // ui->title_bar->setStyleSheet("background-color: rgb(255,153,19)");
    ui->title_bar->setVisible(false);
    connect(ui->btnClose, SIGNAL(clicked(bool)), this, SLOT(exit_widget()));
    ui->btnClose->setVisible(true);
    QString fontName = loadFontFromFile("/usr/local/share/fonts/msyh.ttf");
    QFont font(fontName);
    font.setPointSize(16);
    //font.setBold(true);
    ui->label_title->setFont(font);
    widget_resize();
}

MyDialog::~MyDialog()
{
    delete ui;
}

void MyDialog::setFlag(int flag)
{
    m_flag = flag;
    switch(m_flag)
    {
        case SHUTDOWN:
             {
                ui->label_title->setVisible(true);
                ui->label_title->setText("关机提示");
             }
            break;
        default:
        {
            ui->label_title->setText("提示");
            ui->label_title->setVisible(false);
        }
    }
}

void MyDialog::on_OKpushButton_clicked()
{

    switch(m_flag)
    {
        case SHUTDOWN:
            {
                system("shutdown -h now");
            }
            break;
        default:
        {

        }
        break;
    }

    this->close();
}

void MyDialog::on_CancelpushButton_clicked()
{
    this->close();
}

void MyDialog::setText(QString Text)
{
    m_pTextLabel->setText(Text);
}
void MyDialog::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    //qDebug() << "MyDialog::paintEvent , width :" << width() << "height :" << height();
    painter.drawPixmap(0,0,width(),height(),QPixmap(DIALOGPNG1));
}

void MyDialog::exit_widget()
{
    close();
}

void MyDialog::setNetOff(QString text, int flag)
{
    m_flag = flag;
    m_content = text;
    m_pTextLabel->setText(text);
    ui->label_title->setText("提示");
    //m_nCount = 10;
    //m_timer = NULL;
    //m_timer = new QTimer(this);
    //connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));
    //m_timer->start(1000);
}

void MyDialog::update()
{
//    if (m_nCount == 0)
//    {
//        if (m_timer != NULL)
//        {
//            m_timer->stop();
//            delete m_timer;
//            m_timer = NULL;
//            setShow(false);
//            this->hide();
//        }
//    }
//    QString str;
//    str = str.number(m_nCount, 10);
//    QString strMsg;
//    strMsg = m_content + " " + "(" + str + ")";
//    qDebug() << strMsg;
//    m_pTextLabel->setText(strMsg);
//    m_nCount--;
}

void MyDialog::widget_resize()
{
    int scr_width = QApplication::desktop()->width();
    int scr_height = QApplication::desktop()->height();
    float factor_x = (float)scr_width/g_scr_old_width;
    float factor_y = (float)scr_height/g_scr_old_height;
    my_resize(this, factor_x, factor_y);
    //qDebug() << "MyDialog::widget_resize width: " << width() << "height :" << height();
    my_resize(ui->title_bar, factor_x, factor_y);
    my_resize(ui->btnClose, factor_x, factor_y);
    my_resize(ui->Textlabel, factor_x, factor_y);
    my_resize(ui->OKpushButton, factor_x, factor_y);
    my_resize(ui->CancelpushButton, factor_x, factor_y);
    this->move(scr_width/2 - width()/2, scr_height/2 - height()/2);
}
