#ifndef BLACKSCREEN_H
#define BLACKSCREEN_H

#include <QWidget>

namespace Ui {
class BlackScreen;
}

class BlackScreen : public QWidget
{
    Q_OBJECT

public:
    explicit BlackScreen(QWidget *parent = 0);
    ~BlackScreen();

private:
    Ui::BlackScreen *ui;
};

#endif // BLACKSCREEN_H
