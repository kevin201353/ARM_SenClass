#include "waitstudialog.h"
#include "ui_waitstudialog.h"

#define DESKTOPMAP1   "/usr/local/shencloud/50_22.png"
WaitstuDialog::WaitstuDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WaitstuDialog)
{
    ui->setupUi(this);
    m_pConnectDlg = new QWidget();
    m_pConnectDlg->setAutoFillBackground(true);
    this->setWindowModality(Qt::ApplicationModal);
    QPixmap pixmap(DESKTOPMAP1);
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Window, QBrush(pixmap));
    m_pConnectDlg->setPalette(palette);
}

WaitstuDialog::~WaitstuDialog()
{
    delete ui;
}

void WaitstuDialog::WaitStuShow()
{
    if (m_pConnectDlg)
    {
        m_pConnectDlg->showFullScreen();
    }
}

void WaitstuDialog::WaitStuHide()
{
    if (m_pConnectDlg)
    {
        m_pConnectDlg->hide();
    }
}
