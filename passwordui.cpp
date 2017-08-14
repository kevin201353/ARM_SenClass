#include "passwordui.h"
#include "ui_passwordui.h"
#include "QCryptographicHash"
#include <QByteArray>
#include <QDesktopWidget>
#include "mydialog.h"
#include "global.h"

#define SH_PASS_CONFIG   "@1en0v0"

#define DIALOGPNG1       "/usr/local/shencloud/image1/msgboxbg.png"

bool PasswordUI::m_isAdjust = false;
PasswordUI::PasswordUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PasswordUI)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowModality(Qt::ApplicationModal);
    //ui->frame_title->setStyleSheet("background-color: rgb(0,123,214)");
#ifdef CCLOUD
    ui->frame_title->setStyleSheet("background-color: rgb(255,153,19)");
    ui->btn_OK->setStyleSheet("background-color:rgb(255,153,19);color:rgb(255,255,255)");
    ui->btn_Cancel->setStyleSheet("background-color:rgb(255,153,19);color:rgb(255,255,255)");
#else
    ui->frame_title->setStyleSheet("background-color: rgb(5,149,245)");
    ui->btn_OK->setStyleSheet("background-color:rgb(5,149,245);color:rgb(255,255,255)");
    ui->btn_Cancel->setStyleSheet("background-color:rgb(5,149,245);color:rgb(255,255,255)");
#endif
    connect(ui->btn_exit, SIGNAL(clicked(bool)), this, SLOT(exit_widget()));
    connect(ui->btn_OK, SIGNAL(clicked(bool)), this, SLOT(OnOk()));
    connect(ui->btn_Cancel, SIGNAL(clicked(bool)), this, SLOT(OnCancel()));
    QByteArray ba, bb;
    QString pwd = SH_PASS_CONFIG;
    QCryptographicHash md(QCryptographicHash::Md5);
    ba.append(pwd);
    md.addData(ba);
    bb = md.result();
    m_strPassword.append(bb.toHex());
    m_isAdjust = false;
    QString fontName = loadFontFromFile("/usr/local/share/fonts/msyh.ttf");
    QFont font(fontName);
    font.setPointSize(16);
    //font.setBold(true);
    ui->label_title->setFont(font);
    ui->label_title->setText("输入密码");
    font.setBold(false);
    font.setPixelSize(15);
    ui->label_pass->setText("输入密码");
    ui->label_pass->setFont(font);
    ui->btn_OK->setFont(font);
    ui->btn_OK->setText("确定");
    ui->btn_Cancel->setFont(font);
    ui->btn_Cancel->setText("取消");
    ui->lineEdit_pass->setText("");
    ui->lineEdit_pass->setEchoMode(QLineEdit::Password);
    ui->frame_title->setVisible(false);
    widget_resize();
    this->hide();
}

PasswordUI::~PasswordUI()
{
    delete ui;
}


void PasswordUI::OnOk()
{
    m_isAdjust = false;
    QString strPass = ui->lineEdit_pass->text();
    QString newpass;
    QByteArray ba, bb;
    QCryptographicHash md(QCryptographicHash::Md5);
    ba.append(strPass);
    md.addData(ba);
    bb = md.result();
    newpass.append(bb.toHex());
    if (m_strPassword == newpass)
    {
        m_isAdjust = true;
        emit ShowPassUI();
    }else
    {
        ui->lineEdit_pass->setText("");
        MyDialog *pshutdlg = new MyDialog();
        pshutdlg->setText("密码错误！");
        pshutdlg->setFlag(PASSFAILD);
        pshutdlg->show();
    }
}

void PasswordUI::OnCancel()
{
    this->close();
}

void PasswordUI::exit_widget()
{
    this->close();
}

bool PasswordUI::isAdjustPass()
{
    return m_isAdjust;
}

void PasswordUI::setpasstext(QString szpass)
{
    ui->lineEdit_pass->setText(szpass);
}

void PasswordUI::keyPressEvent(QKeyEvent  *event)
{
    if ( event->key() == Qt::Key_Return )
    {
        OnOk();
    }
    QWidget::keyPressEvent(event);
}

void PasswordUI::widget_resize()
{
    int scr_width = QApplication::desktop()->width();
    int scr_height = QApplication::desktop()->height();
    float factor_x = (float)scr_width/g_scr_old_width;
    float factor_y = (float)scr_height/g_scr_old_height;
    my_resize(this, factor_x, factor_y);
    my_resize(ui->frame_title, factor_x, factor_y);
    my_resize(ui->label_title, factor_x, factor_y);
    my_resize(ui->label_pass, factor_x, factor_y);
    my_resize(ui->lineEdit_pass, factor_x, factor_y);
    my_resize(ui->btn_Cancel, factor_x, factor_y);
    my_resize(ui->btn_exit, factor_x, factor_y);
    my_resize(ui->btn_OK, factor_x, factor_y);
    ui->label_pass->setScaledContents(true);
    this->move(scr_width/2 - width()/2, scr_height/2 - height()/2);
}

void PasswordUI::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,width(),height(),QPixmap(DIALOGPNG1));
}
