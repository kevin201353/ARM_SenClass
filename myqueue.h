#ifndef MYQUEUE_H
#define MYQUEUE_H
#include <QObject>
#include <QQueue>
#include <QMutexLocker>
#include "global.h"

class MyQueue
{
public:
    MyQueue();
    ~MyQueue();
public:
    void GetQueMsg(QueMsg *queMsg);
    void AddQueMsg(QueMsg queMsg);
    inline bool IsEmpty() {
        return m_queue.isEmpty();
    }
private:
    QQueue<QueMsg> m_queue;
    QMutex  m_mutex;
};

class MyBuffer
{
public:
    MyBuffer();
    ~MyBuffer();
public:
    void add(char *data);
    void clear();
    bool isEmpty();
    char *getdata();
    char m_szMqMsg[102400];
private:
    QMutex  m_mutex;
};

#endif // MYQUEUE_H
