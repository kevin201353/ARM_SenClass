#include "netconfig.h"
#include "include.h"
#include <sys/msg.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/time.h>
#include "type.h"

NetConfig   *g_pNetConfig;
process     *g_pProcess = NULL;
myJson      *g_pJson = NULL;
ActiveMQConsumer *g_Pconsume = NULL;
ActiveMQProduce *g_Pproduce = NULL;
char g_strProduceAdd[1024];
char g_strConsumerAdd[1024];
char g_strProduceQueue[50];
char g_strConsumerQueue[50];
char g_strRoomNum[100];
char g_strSeatNum[20];
char g_strTerminalID[30];
Log *g_pLog;
myHttp *g_pMyHttp = NULL;
char g_strServerIP[25];
char cMainExitFlag;
LoadingFrame *g_LoadingFrame;
pthread_mutex_t  g_mymutex;
bool   g_reconntvm;

////////////////////////////////
//msg queue
#define  MAX_TEXT   100
struct msg_st {
    long int msg_type;
    char text[MAX_TEXT];
};
void msg_queue_del();
void msg_send(char* dataTmp);
int g_msgid = -1;
void create_msg_queue()
{
    if (g_msgid != -1)
    {
        msg_queue_del();
        g_msgid = -1;
    }
    g_msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
    if (g_msgid == -1)
    {
        fprintf(stderr, "msgget failed with error: %d\n", errno);
        //exit(EXIT_FAILURE);
    }
    msg_send("0");
}

void msg_send(char* dataTmp)
{
    struct msg_st data;
    data.msg_type = 1;
    strcpy(data.text, dataTmp);
    if (msgsnd(g_msgid, (void*)&data, MAX_TEXT, 0) == -1)
    {
        fprintf(stderr, "msgsnd failed\n");
        //exit(EXIT_FAILURE);
    }
}

void msg_queue_del()
{
    if (g_msgid != -1)
    {
        if (msgctl(g_msgid, IPC_RMID, 0) == -1)
        {
            fprintf(stderr, "msgctl(IPC_RMID) failed.\n");
            //exit(EXIT_FAILURE);
        }
        g_msgid = -1;
    }
}

struct msg_st data;
void msg_recv(char* dataT)
{
    long int msgtype = 1;
    if (msgrcv(g_msgid, (void*)&data, MAX_TEXT, msgtype, IPC_NOWAIT) == -1)
    {
        fprintf(stderr, "msgrcv failed width errno: %d\n", errno);
    }
    strcpy(dataT, (char*)data.text);
}

//return 0: net Unreachable
int ping_net(char *ip)
{
    if (ip != NULL)
    {
        char szbuf[100] = {0};
        char data[100] = {0};
        sprintf(szbuf, "ping %s -c 4 -i 0.5 > /usr/local/shencloud/log/ping_net.log", ip);
        system(szbuf);
        //MyMutex_lock();
        FILE *fp = fopen("/usr/local/shencloud/log/ping_net.log", "r");
        if (fp != NULL)
        {
            int ncount = 0;
            while(fgets(data, 100, fp) != NULL)
            {
                if (ncount == 2)
                {
                    if (strstr(data, "ttl=")!= NULL && strstr(data, "time=") != NULL)
                    {
                        //MyMutex_unlock();
                        fclose(fp);
                        return 1;
                    }else
                    {
                        //MyMutex_unlock();
                        fclose(fp);
                        return 0;
                    }
                }
                ncount++;
            }
            fclose(fp);
        }
        //MyMutex_unlock();
    }
    return 0;
}

//current time millls
#ifdef ARM
long long  __GetTime()
#else
long __GetTime()
#endif
{
    struct timeval iTime;
    gettimeofday(&iTime, NULL);
#ifdef ARM
    long long lTime = ((long long )iTime.tv_sec) * 1000 + (long long)iTime.tv_usec/1000;
#else
    long lTime = ((long)iTime.tv_sec) * 1000 + (long)iTime.tv_usec/1000;
#endif
    return lTime;
}

//mutex
void InitMyMutex()
{
    pthread_mutex_init(&g_mymutex, NULL);
}
void MyMutex_lock()
{
    pthread_mutex_lock(&g_mymutex);
}
void MyMutex_unlock()
{
    pthread_mutex_unlock(&g_mymutex);
}
void MyMutex_destroy()
{
    pthread_mutex_destroy(&g_mymutex);
}

bool bMyGetConnectVm()
{
    bool bRet = false;
    MyMutex_lock();
    bRet = g_reconntvm;
    MyMutex_unlock();
    return bRet;
}

void MySetConnectVm(bool bval)
{
    MyMutex_lock();
    g_reconntvm = bval;
    MyMutex_unlock();
}
