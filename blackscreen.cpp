#include "blackscreen.h"
#include "ui_blackscreen.h"

BlackScreen::BlackScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BlackScreen)
{
    ui->setupUi(this);
    setStyleSheet("background-color: black");
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
}

BlackScreen::~BlackScreen()
{
    delete ui;
}
