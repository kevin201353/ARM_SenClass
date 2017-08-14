#ifndef MQMSGPROCESS_H
#define MQMSGPROCESS_H

#include <QObject>
#include <pthread.h>
#include "multimsg.h"

class MqMsgProcess : public QObject
{
    Q_OBJECT
public:
    explicit MqMsgProcess(QObject *parent = 0);
    ~MqMsgProcess();
signals:
public slots:
     void on_processBroadCastMsg();
public:
    void _MqMsgProcess();
    void GetAddrMac();
    void start();
    void _abotThread();
    void strart_spicyThrd();
    void _spicyProcess();
public:
    char m_strVmID[50];
    char m_strMac[50];
    char m_strIP[20];
    pthread_t  m_pid;
    pthread_t  m_pidsp;
    pthread_mutex_t m_stateMutex;

    MultiMsg  *m_MultiMsg;
    char       m_Mac[30];
};

#endif // MQMSGPROCESS_H
