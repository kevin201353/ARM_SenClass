#ifndef WAITSTUDIALOG_H
#define WAITSTUDIALOG_H

#include <QDialog>

namespace Ui {
class WaitstuDialog;
}

class WaitstuDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WaitstuDialog(QWidget *parent = 0);
    ~WaitstuDialog();

private:
    Ui::WaitstuDialog *ui;
public:
    QWidget *m_pConnectDlg;
    void WaitStuShow();
    void WaitStuHide();
};

#endif // WAITSTUDIALOG_H
