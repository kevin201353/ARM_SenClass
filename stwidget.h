#ifndef STWIDGET_H
#define STWIDGET_H

#include <QWidget>

namespace Ui {
    class stuwidgetUI;
}

class stwidget : public QWidget
{
    Q_OBJECT
public:
    explicit stwidget(QWidget *parent = 0);
     ~stwidget();
signals:
public slots:
protected:
    void paintEvent(QPaintEvent *event);
private:
    Ui::stuwidgetUI *ui;
};

#endif // STWIDGET_H
