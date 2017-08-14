#include "loadingframe.h"
#include "ui_loadingframe.h"

LoadingFrame::LoadingFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::LoadingFrame)
{
    ui->setupUi(this);
    m_Font.setPixelSize(23);
    this->setStyleSheet("background:rgba(51,44,43,95);border-radius:12px;");
    m_pGIFLabel = ui->GIFlabel;
    m_pWordLabel = ui->label;
    m_pWordLabel->setAlignment(Qt::AlignCenter);
    m_pWordLabel->setFont(m_Font);
    m_pWordLabel->setStyleSheet("background:rgba(255,255,255,0);color:rgb(255,255,255)");
    m_pMovie = new QMovie(GIFFILE);
    m_pGIFLabel->setStyleSheet("background:rgba(255,255,255,0)");
    m_pGIFLabel->setAlignment(Qt::AlignCenter);
    m_pGIFLabel->setMovie(m_pMovie);
    m_pMovie->setSpeed(150);
    m_pMovie->start();
    this->move((parent->width()/2)-(width()/2),(parent->height()/2)-(height()/2));
}

LoadingFrame::~LoadingFrame()
{
    delete ui;
}
