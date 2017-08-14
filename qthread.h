#ifndef SHQTHREAD_H
#define SHQTHREAD_H

#include <QThread>
#include <QObject>

class qthread: public QThread
{
    Q_OBJECT
public:
    qthread();
    ~qthread();
public:
    void stop();
protected:
    void run();
signals:
    void NoticeShow();
    void NoticeHide();
private:
    bool m_stop;
};

class qthreadPing : public QThread
{
    Q_OBJECT
public:
    qthreadPing();
    ~qthreadPing();
public:
    inline void SetPingText(const QString str){
        m_szPing = str;
    }
protected:
    void run();
signals:
    void NoticeMsg();
private:
    QString  m_szPing;
};

#endif // SHQTHREAD_H
