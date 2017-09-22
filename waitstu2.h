#ifndef WAITSTU2_H
#define WAITSTU2_H

#include <QWidget>
#include <QMovie>
#include <QTimer>
#include "global.h"
#include "qthread.h"
#include <QLabel>

namespace Ui {
class waitstu2;
}

class waitstu2 : public QWidget
{
    Q_OBJECT

public:
    explicit waitstu2(QWidget *parent = 0);
    ~waitstu2();
    void waitstushow();
    void waitstuhide();
    void widget_resize();
    int width() const {
        return m_width;
    };
    int height() const {
        return m_height;
    };
protected:
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent  *event);
signals:
    void wait_showPassUI();
    void wait_shutdown();
private slots:
    void On_Setting();
    void On_Shutdown();
    void On_ShowCpu(const QString &str);
private:
    Ui::waitstu2 *ui;
    QMovie  *m_pMovie;
    int     m_width;
    int     m_height;
    char    m_strIP[MAX_IP_SIZE];
    QLabel *labelcpu;
};

#endif // WAITSTU2_H
