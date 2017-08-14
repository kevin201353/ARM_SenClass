#ifndef ABOUTUI_H
#define ABOUTUI_H

#include <QWidget>

namespace Ui {
class AboutUI;
}

class AboutUI : public QWidget
{
    Q_OBJECT

public:
    explicit AboutUI(QWidget *parent = 0);
    ~AboutUI();
protected:
    void keyPressEvent(QKeyEvent  *event);
    void widget_resize();
private slots:
    void On_Close();
private:
    Ui::AboutUI *ui;
};

#endif // ABOUTUI_H
