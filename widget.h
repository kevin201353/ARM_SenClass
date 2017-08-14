#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDialog>
#include <QEvent>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>

#include "sendfile.h"
#include "callname.h"
#include "blackscreen.h"
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void disableRaiseHand();
    void enableRaiseHand();
//    BlackScreen *bs;
    SendFile    *sf;
    CallName    *cn;
    int         pressed;
    QNetworkReply *reply;

private slots:
    void send_file_done();
    void on_filePushButton_clicked();
    void on_raiseHandPushButton_clicked();
    void on_cancelPushButton_clicked();
    void on_showPushButton_clicked();
    void on_exitShowPushButton_clicked();
    void replyFinished();
    void on_readyRead();

private:
    Ui::Widget  *ui;
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
};

#endif // WIDGET_H
