#ifndef LOADINGFRAME_H
#define LOADINGFRAME_H

#include <QFrame>
#include <QLabel>
#include <QMovie>
#define GIFFILE  "/usr/local/shencloud/image1/Loaging.gif"
namespace Ui {
class LoadingFrame;
}

class LoadingFrame : public QFrame
{
    Q_OBJECT

public:
    explicit LoadingFrame(QWidget *parent = 0);
    ~LoadingFrame();

private:
    Ui::LoadingFrame *ui;
    QLabel  *m_pGIFLabel;
    QLabel  *m_pWordLabel;
    QFont   m_Font;
    QMovie  *m_pMovie;
};

#endif // LOADINGFRAME_H
