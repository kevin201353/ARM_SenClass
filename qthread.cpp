#include "qthread.h"
#include <QDebug>

extern bool g_bSetupAmq;
qthread::qthread()
{
    m_stop = false;
}

qthread::~qthread()
{
    m_stop = true;
}

int ncount = 0;
void qthread::run()
{
    while(!m_stop)
    {
        if (!g_bSetupAmq)
        {
            //qDebug() << "start show netoff dailog.\n";
            if (ncount > 3)
            {
                ncount = 0;
                emit NoticeShow();
            }
            ncount++;
        }else
        {
            emit NoticeHide();
        }
        sleep(5);
    }
}

void qthread::stop()
{
    m_stop = true;
}


qthreadPing::qthreadPing()
{

}

qthreadPing::~qthreadPing()
{

}

QString  g_strPing;
void qthreadPing::run()
{
    char szcmd[100] = {0};
    QString strping;
    strping = "ping ";
    strping += m_szPing;
    strcpy(szcmd, strping.toStdString().c_str());
    strcat(szcmd, " -c 10");
    FILE *pp = popen(szcmd, "r");
    if (pp)
    {
        int nCount = 0;
        char tmp[512] = {0};
        while (fgets(tmp, sizeof(tmp), pp) != NULL)
        {
            if (nCount > 10)
                break;
            if (tmp[strlen(tmp) -1] == '\n')
            {
                tmp[strlen(tmp) - 1] = '\0';
            }
            g_strPing = QString::fromStdString(tmp);
            emit NoticeMsg();
            nCount++;
        }
        pclose(pp);
    }
}

