#include "myqueue.h"
#include <QString>

MyQueue::MyQueue()
{

}

MyQueue::~MyQueue()
{

}

void MyQueue::GetQueMsg(QueMsg *queMsg)
{
     QMutexLocker locker(&m_mutex);
     if (queMsg != NULL)
     {
        if (!m_queue.isEmpty())
        {
            QueMsg msg = m_queue.dequeue();
            strcpy(queMsg->Data, msg.Data);
        }else
        {
            memset(queMsg->Data, 0, sizeof(queMsg->Data));
        }
     }
}

void MyQueue::AddQueMsg(QueMsg queMsg)
{
     QMutexLocker locker(&m_mutex);
     m_queue.enqueue(queMsg);
}


MyBuffer::MyBuffer()
{
   memset(m_szMqMsg, 0, 102400);
}
MyBuffer::~MyBuffer()
{

}

void MyBuffer::add(char *data)
{
    QMutexLocker locker(&m_mutex);
    strcpy(m_szMqMsg, data);
}

void MyBuffer::clear()
{
    QMutexLocker locker(&m_mutex);
    memset(m_szMqMsg, 0, 102400);
}

bool MyBuffer::isEmpty()
{
    QMutexLocker locker(&m_mutex);
    QString strMsg(m_szMqMsg);
    if (strMsg.isEmpty() || strMsg.length() <= 0)
        return true;
    return false;
}

char * MyBuffer::getdata()
{
    QMutexLocker locker(&m_mutex);
    return m_szMqMsg;
}
