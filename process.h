#ifndef PROCESS_H
#define PROCESS_H

#include <pthread.h>
#include "listbuf.h"
#include "loginwidget.h"
#define DATABUFLEN 10240
struct DataBuf
{
    volatile bool s_IsArray;
    char Data[DATABUFLEN];
};

class process
{
public:
    process(LoginWidget *widget);
    ~process();
    int Start();
    int ProcessThread();
    int ProcessThreadNew();
    ListBuf *m_RecvBuf;
    pid_t   m_SpicyPid;
    char m_strVmID[50];
    char m_strMac[50];
    char m_strIP[20];
    DataBuf m_data;
    void Lock();
    void UnLock();
    void GetAddrMac();
public:
    int connect_vm(char *ip, char *port, char *vmid);
private:
    pthread_t m_pid;
    LoginWidget *m_pWidget;
    pthread_mutex_t  m_Locak;

};

#endif // PROCESS_H
