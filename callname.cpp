#include "callname.h"
#include "ui_callname.h"
#include <QDesktopWidget>
#include <QRect>
#include <QRegExp>

CallName::CallName(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CallName)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    ui->frame->setStyleSheet("border-image: url(:/images/call_name.png);");
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect screenRect = desktopWidget->screenGeometry();
    ui->frame->resize(screenRect.width(), screenRect.height());
    ui->groupBox->setAttribute(Qt::WA_TranslucentBackground, true);
    ui->groupBox->setStyleSheet("QGroupBox{border:none;}");
    ui->groupBox->setGeometry(QRect((screenRect.width() - ui->groupBox->width()) / 2,
                                    (screenRect.height() - ui->groupBox->height()) / 2 - 100,
                                    ui->groupBox->width(), ui->groupBox->height()));
    QRegExp regExp1("[1-9][0-9]{1,5}");
    ui->classNum->setValidator(new QRegExpValidator(regExp1, this));
//    QRegExp regExp2("[a-z|A-Z]{1,16}");
//    ui->name->setValidator(new QRegExpValidator(regExp2, this));
    ui->classTextEdit->hide();
    ui->nameTextEdit->hide();
}

CallName::~CallName()
{
    delete ui;
}

void CallName::on_pushButtonEnsure_clicked()
{
    classNum.clear();
    name.clear();
    classNum = ui->classNum->text();
    name = ui->name->text();
    if(classNum.isEmpty())
    {
        ui->nameTextEdit->hide();
        ui->classTextEdit->show();
    }
    else if(name.isEmpty())
    {
        ui->classTextEdit->hide();
        ui->nameTextEdit->show();
    }
    else
    {
        //发送签到的信息

        ui->classTextEdit->hide();
        ui->nameTextEdit->hide();
        close();
    }
}
