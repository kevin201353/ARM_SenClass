#ifndef PASSWORDUI_H
#define PASSWORDUI_H

#include <QWidget>

密码错误！
输入密码：
确定
取消

namespace Ui {
class PasswordUI;
}

class PasswordUI : public QWidget
{
    Q_OBJECT

public:
    explicit PasswordUI(QWidget *parent = 0);
    ~PasswordUI();

private:
    Ui::PasswordUI *ui;
};

#endif // PASSWORDUI_H
