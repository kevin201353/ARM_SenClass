#include "qthread.h"
#include <QDebug>
#include "include.h"
#include "sysInfo.h"

extern volatile bool g_bSetupAmq;
extern int ping_net(char *ip);
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
        usleep(500);
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

/***************************************************************************************/
CpuThrd::CpuThrd()
{
    m_stop = false;
}

CpuThrd::~CpuThrd()
{
    m_stop = true;
}

void CpuThrd::run()
{
    QString strcpu;
    char szcmd[100] = {0};
    sprintf(szcmd, "top -b -n 1 -p %u 2>&1 | awk -v pid=%u '{if($1==%u)print $9}'", (unsigned int)getpid(), (unsigned int)getpid(), (unsigned int)getpid());
    while(!m_stop)
    {
        FILE *pp = popen(szcmd, "r");
        if (pp)
        {
            char tmp[512] = {0};
            while (fgets(tmp, sizeof(tmp), pp) != NULL)
            {
                if (tmp[strlen(tmp) -1] == '\n')
                {
                    tmp[strlen(tmp) - 1] = '\0';
                }
                strcpu = QString::fromStdString(tmp);
                emit NoticeMsg(strcpu);
            }
            pclose(pp);
        }
        sleep(1);
    }
}

