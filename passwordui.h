#ifndef PASSWORDUI_H
#define PASSWORDUI_H

#include <QWidget>
#include <QString>
#include <QDialog>
#include <QMouseEvent>
#include <QFont>

namespace Ui {
class PasswordUI;
}

class PasswordUI : public QWidget
{
    Q_OBJECT

public:
    explicit PasswordUI(QWidget *parent = 0);
    ~PasswordUI();
public:
    static bool isAdjustPass();
    void setpasstext(QString szpass);
signals:
    void ShowPassUI();
private slots:
    void OnOk();
    void OnCancel();
    void exit_widget();
protected:
    void keyPressEvent(QKeyEvent  *event);
    void widget_resize();
    void paintEvent(QPaintEvent *event);
private:
    Ui::PasswordUI *ui;
    QString   m_strPassword;
    static bool   m_isAdjust;
    QDialog*     m_tmpDialog;
};
#endif // PASSWORDUI_H
