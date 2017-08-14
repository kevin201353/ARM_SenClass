#ifndef WAITSTU_H
#define WAITSTU_H

#include <QFrame>

namespace Ui {
class waitstu;
}

class waitstu : public QFrame
{
    Q_OBJECT

public:
    explicit waitstu(QWidget *parent = 0);
    ~waitstu();
protected:
    void paintEvent(QPaintEvent *event);
private:
    Ui::waitstu *ui;
};

#endif // WAITSTU_H
