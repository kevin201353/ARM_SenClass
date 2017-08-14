#ifndef MULTIMSG_H
#define MULTIMSG_H

#include <QObject>
#include <QHostAddress>
#include <QUdpSocket>

class MultiMsg : public QObject
{
    Q_OBJECT
public:
    MultiMsg();
    void startSending();
    void sendDatagram();
    void uinit();
private slots:
    void processPendingDatagrams();
signals:
    void broadcastMsg();
private:
    QUdpSocket *udpSocket;
    QHostAddress  groupAddress;
};

#endif // MULTIMSG_H
