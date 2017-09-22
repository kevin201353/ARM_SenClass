#include "mqmsgprocess.h"
#include "include.h"
#include "global.h"
#include <QDateTime>
#include <QByteArray>
#include "myqueue.h"
#include <QTime>
#include <QDebug>
#include "type.h"
#include <QFile>

#define   DEMOFLAG   "/tmp/DemoFlag"
extern LoginWidget * g_loginWnd;
static char g_szCmd[1024] = {0};
extern char g_szRetVm[1024];
void *ThreadForSystem(void *para);
extern int detect_process(char* szProcess);
extern MyBuffer g_myBuffer;
#ifdef ARM
volatile long long g_interval_time = 0;
#else
volatile long g_interval_time = 0;
#endif
extern int  get_spicy_ip(char* ip);
extern int  get_spicy_port(void);
#ifdef ARM
    extern long long st_heart_time;
#else 
    extern long st_heart_time;
#endif 
extern pthread_mutex_t g_hreadMutex;
extern pthread_mutex_t g_freestudyMutex;
extern bool  g_bExit_freeStuy_flag;

extern int  g_check_heart_flag;
volatile int g_currclass_state = 0;
extern volatile bool g_bSetupAmq;
char   g_szCommand[1024] = {0};
extern MyQueue  g_MsgQueue;

volatile bool  g_deamonflag = false;

volatile bool  g_showloal = false;
bool g_bTeacherFlag = false;

void _system(char * cmd)
{
    FILE *fp = NULL;
	if ((fp = popen(cmd, "r")) == NULL)
	{
	    g_pLog->WriteLog(0,"zhaosenhua _system  cmd failed.");
	}
	if (fp != NULL)
	    pclose(fp);
}

void _kill_spicy_eclass(char *spicy, char *eclass)
{
    char szCmd[100] = {0};
	if (spicy != NULL && strcmp("", spicy) != 0)
	{
	    if (detect_process(spicy) == 1)
	    {
            sprintf(szCmd, "killall -9 %s", spicy);
            system(szCmd);
		    g_pLog->WriteLog(0,"_kill_spicy_eclass kill spicy.");
#ifdef ARM
            system("rk3188_clean_display");
#endif
	    }
	}
	if (eclass != NULL && strcmp("", eclass) != 0)
	{
	    if (detect_process(eclass) == 1)
	    {
            sprintf(szCmd, "killall -9 %s", eclass);
            system(szCmd);
#ifdef ARM
            system("rk3188_clean_display");
#endif
            g_pLog->WriteLog(0,"_kill_spicy_eclass kill eclass_client_udp.");
	    }
	}
}

char port[20];
char data_xor[20];
void _display_vm2(char *cmd)
{
    pthread_t pid;
    if(pthread_create(&pid, NULL,ThreadForSystem, g_szCmd))
    {
        g_pLog->WriteLog(0,"_display_vm, pthread_create failed.");
        printf("create Thread Error");
    }
    int nCount = 0;
    while(access("/tmp/data_port",F_OK))
    {
        if (nCount >= 20)
            break;
        sleep(1);
        nCount++;
    }
    memset(port,0,20);
    FILE *pf = fopen("/tmp/data_port","r");
    if(pf != NULL)
    {
        fread(port,20,1,pf);
        g_pLog->WriteLog(0,"/tmp/data_port:%s",port);
        fclose(pf);
    }
    memset(data_xor,0,20);
    pf = fopen("/tmp/data_xor","r");
    if(pf != NULL)
    {
        fread(data_xor,20,1,pf);
        g_pLog->WriteLog(0,"/tmp/data_xor:%s",data_xor);
        fclose(pf);
    }
}
void _display_vm3(char *cmd)
{
    pthread_t pid;
    if (!g_deamonflag)
    {
        _system(cmd);
    }
    int nCount = 0;
    while(access("/tmp/data_port",F_OK))
    {
        if (nCount >= 200000)
            break;
        usleep(1000);
        nCount++;
    }
    memset(port,0,20);
    FILE *pf = fopen("/tmp/data_port","r");
    if(pf != NULL)
    {
        fread(port,20,1,pf);
        g_pLog->WriteLog(0,"/tmp/data_port:%s",port);
        fclose(pf);
    }
    memset(data_xor,0,20);
    pf = fopen("/tmp/data_xor","r");
    if(pf != NULL)
    {
        fread(data_xor,20,1,pf);
        g_pLog->WriteLog(0,"/tmp/data_xor:%s",data_xor);
        fclose(pf);
    }
}

void _display_vm(char *cmd)
{
    pthread_t pid;
    if (!g_deamonflag)
    {
        if(pthread_create(&pid, NULL,ThreadForSystem, g_szCmd))
        {
            g_pLog->WriteLog(0,"_display_vm, pthread_create failed.");
            printf("create Thread Error");
        }
    }
    int nCount = 0;
    while(access("/tmp/data_port",F_OK))
    {
        if (nCount >= 20)
            break;
        sleep(1);
        nCount++;
    }
    memset(port,0,20);
    FILE *pf = fopen("/tmp/data_port","r");
    if(pf != NULL)
    {
        fread(port,20,1,pf);
        g_pLog->WriteLog(0,"/tmp/data_port:%s",port);
        fclose(pf);
    }
    memset(data_xor,0,20);
    pf = fopen("/tmp/data_xor","r");
    if(pf != NULL)
    {
        fread(data_xor,20,1,pf);
        g_pLog->WriteLog(0,"/tmp/data_xor:%s",data_xor);
        fclose(pf);
    }
}

static void *DemoThreadForSystem(void *para)
{
    char SystemBuf[1024];
    memset(SystemBuf,0,1024);
    char *p = (char *)para;
    if(p)
    {
        strcpy(SystemBuf,p);
#ifdef _DEBUG_PRINT
        printf("sysTem:%s\n",SystemBuf);
#endif
        _system(SystemBuf);
    }
    pthread_detach(pthread_self());
    return NULL;
}

void _demo_display_vm(char *)
{
	pthread_t pid;
    if (!g_deamonflag)
    {
        if(pthread_create(&pid, NULL,DemoThreadForSystem, g_szCmd))
        {
            printf("create Thread Error");
        }
    }
    int nCount = 0;
    while(access("/tmp/data_port",F_OK))
    {
        if (nCount >= 20)
            break;
        sleep(1);
        nCount++;
    }
    memset(port,0,20);
    FILE *pf = fopen("/tmp/data_port","r");
    if(pf != NULL)
    {
        fread(port,20,1,pf);
        g_pLog->WriteLog(0,"/tmp/data_port:%s",port);
        fclose(pf);
    }
    memset(data_xor,0,20);
    pf = fopen("/tmp/data_xor","r");
    if(pf != NULL)
    {
        fread(data_xor,20,1,pf);
        g_pLog->WriteLog(0,"/tmp/data_xor:%s",data_xor);
        fclose(pf);
    }
}

MqMsgProcess::MqMsgProcess(QObject *parent) : QObject(parent)
{
    memset(g_szRetVm, 0, sizeof(g_szRetVm));
    m_pidsp = NULL;
    pthread_mutex_init(&m_stateMutex, NULL);
    //add 170727
    memset(m_Mac, 0, sizeof(m_Mac));
    NetConfig *pConfig = new NetConfig();
    pConfig->GetMacAdd(m_Mac,true);
}

MqMsgProcess::~MqMsgProcess()
{
    _abotThread();
    pthread_cancel(m_pidsp);
    pthread_join(m_pidsp, NULL);
    pthread_mutex_destroy(&m_stateMutex);
    if (NULL != m_MultiMsg)
    {
        m_MultiMsg->uinit();
    }
}

static void *ProcessFun(void *param)
{
    MqMsgProcess *Temp = (MqMsgProcess*)param;
    Temp->_MqMsgProcess();
    pthread_detach(pthread_self());
    return NULL;
}

void MqMsgProcess::start()
{
    GetAddrMac();
    if(pthread_create(&m_pid,NULL,ProcessFun,this))
    {
        printf("Create Error!\n");
    }
}

void MqMsgProcess::_abotThread()
{
    cMainExitFlag = 0;
    pthread_cancel(m_pid);
    pthread_join(m_pid, NULL);
}


void MqMsgProcess:: _MqMsgProcess()
{
    g_pLog->WriteLog(0, "thread _MqMsgProcess, pid: %u, tid: %u .", (unsigned int)getpid(), (unsigned int)syscall(SYS_gettid));
    char ActionBuf[100] = {0};
    char datetime[100] = {0};
#ifdef ARM
    long long last_time = 0;
#else
	long last_time = 0;
#endif

    char MessageBuf[1024];
    memset(MessageBuf,0,1024);

    //QueMsg   queMsg;
    cMainExitFlag = 1;
    int nRet = 0;
    while(cMainExitFlag)
    {
        //g_MsgQueue.GetQueMsg(&queMsg);
        //if (strlen(queMsg.Data) <= 0)
        if (g_myBuffer.isEmpty())
        {
#if 0
            qDebug() << "MqMsgProcess::_MqMsgProcess no msg : ";
		   g_pLog->WriteLog(0,"MqMsgProcess::_MqMsgProcess no msg : ");
#endif
            sleep(2);
            continue;
        }
        last_time = __GetTime();
        QString str_time = QString::number(last_time, 10);
#ifdef _DEBUG_PRINT
        qDebug() << "current time : " + str_time;
#endif
        g_pJson->Parse(g_myBuffer.getdata());
        QString strMsg(g_myBuffer.getdata());
        /*
        g_pJson->Parse(queMsg.Data);
        QString strMsg(queMsg.Data);
        */
#ifdef _DEBUG_PRINT
        qDebug() << "MqMsgProcess::_MqMsgProcess msg : " << strMsg;
#endif
        g_pJson->ReadJson(datetime,"datetime");
#ifdef ARM
	    long long  leaspedtmp = atoll(datetime);
	    leaspedtmp -= g_interval_time;
#endif 
#ifdef ARM
        unsigned long long leasped = abs(last_time - leaspedtmp);
#else

#if 0
        long  leaspedtmp = atol(datetime);
        long leasped = last_time - leaspedtmp;
#endif

#if 1
        //add 170515 new
        long  leaspedtmp = atoll(datetime);
        leaspedtmp -= g_interval_time;
        unsigned long leasped = abs(last_time - leaspedtmp);
        //add end
#endif

#endif
        QString str = QString::number(leasped, 10);
#ifdef _DEBUG_PRINT
        qDebug() << "current interval time : " + str;
#endif
        if (leasped > 30000)
        {
#ifdef _DEBUG_PRINT
              qDebug()<< "msg deal timeout, msg resend.";
#endif
              char szleasped[512] = {0};
#ifdef ARM
              sprintf(szleasped, "msg deal timeout, msg resend ARM, tim server[%s] + %lld=%lld, timeclient :%lld, time delay :%lld",datetime, g_interval_time, leaspedtmp, last_time, leasped);
#else
			 sprintf(szleasped, "msg deal timeout, msg resend x86, tim server[%s] + %ld=%ld, timeclient :%ld, time delay :%ld",datetime, g_interval_time, leaspedtmp, last_time, leasped);
#endif
              g_pLog->WriteLog(0, szleasped);
              g_myBuffer.clear();
              sleep(1);
              //continue;
        }
        g_pJson->ReadJson(ActionBuf,"action");
        g_pLog->WriteLog(0,"Action:%s",ActionBuf);
#ifdef _DEBUG_PRINT
        qDebug("Action:%s",ActionBuf);
#endif
        if (strcmp(ActionBuf,"classbegin") == 0)
        {
            if (!g_deamonflag)
            {
                _kill_spicy_eclass("spicy", "eclass_client_udp");
    #ifdef ARM
                system("rk3188_clean_display");
    #endif
                ReportMsg reportmsg;
                reportmsg.action = USER_WAITINGDLG_SHOW;
                call_msg_back(msg_respose, reportmsg);
                g_currclass_state = 0;
            }
        }//begin
        if (strcmp(ActionBuf,"display") == 0)
        {
            //connect vm
            char sz_host[100] = {0};
            char sz_port[50] = {0};
            char sz_vmid[100] = {0};
            //host
            g_pJson->ReadJson_v(sz_host, "data", "host");
            g_pJson->ReadJson_v(sz_port, "data", "port");
            g_pJson->ReadJson_v(sz_vmid, "data", "vmId");
            sprintf(g_szCmd, "spicy -h %s -p %s -f > %s 2>&1", sz_host, sz_port, SPICY_LOG_PATH);
            strcpy(g_szRetVm, g_szCmd);
            if (!g_deamonflag)
            {
                _kill_spicy_eclass("spicy", "eclass_client_udp");
                g_pLog->WriteLog(0,"begin class IP:%s Port:%s VmID:%s", sz_host, sz_port, sz_vmid);
                g_loginWnd->SetEnable(false);
                system("sudo rm -f /tmp/data_*");
                ReportMsg reportmsg;
                reportmsg.action = USER_WAITINGDLG_SHOW;
                call_msg_back(msg_respose, reportmsg);
                _display_vm3(g_szCmd);
                g_currclass_state = 1;
            }else
            {
                if (g_showloal)
                {
                    system("sudo rm -f /tmp/data_*");
                    QFile *file = new QFile(DEMOFLAG);
                    if (!file->open(QIODevice::WriteOnly))
                    {
                        g_pLog->WriteLog(0, "on_processBroadCastMsg, student create demoflag file failed.");
                    }
                    file->close();
                    _display_vm2(g_szCmd);
                    g_currclass_state = 1;
                }
            }
            g_pLog->WriteLog(0,"/tmp/data_port 00000:%ld", port);
            g_pLog->WriteLog(0,"/tmp/data_xor 00000:%ld", atol(data_xor));
            sprintf(MessageBuf,"###ap_confirmdisplay###{\"datetime\":\"%s\",\"data\":{\"action\":\"%s\",\"id\":\"%s\", \"apIp\":\"%s\",\"mac\":\"%s\", \"vmId\":\"%s\", \"connected\":%d, \"dsPort\":%ld, \"dsXor\":%ld}}", str_time.toStdString().c_str(), ActionBuf, g_strTerminalID, m_strIP, m_strMac, sz_vmid, 1, atol(port), atol(data_xor));
            g_Pproduce->send(MessageBuf, strlen(MessageBuf));
            g_pLog->WriteLog(0,"zhaosenhua send msg response display 00000: %s", MessageBuf);
            //qDebug("display response end.\n");
        }//display
        if (strcmp(ActionBuf,"classover") == 0)
        {
            //qDebug("classover enter.");
            g_currclass_state = 0;
            memset(MessageBuf,0,1024);
            g_loginWnd->SetEnable(false);
            g_pLog->WriteLog(0,"zhaosenhua send msg response classover.");
            _kill_spicy_eclass("spicy", "eclass_client_udp");
            nRet = detect_process("spicy");
            if (nRet == 0)
            {
                sprintf(MessageBuf,"###ap_confirmclassover###{\"datetime\":\"%s\",\"data\":{\"action\":\"%s\",\"id\":\"%s\"}}", str_time.toStdString().c_str(), ActionBuf, g_strTerminalID);
                g_Pproduce->send(MessageBuf, strlen(MessageBuf));
                g_pLog->WriteLog(0,"zhaosenhua send msg response classover: %s", MessageBuf);
            }
            //qDebug("classover end.");
        }//over
        if (strcmp(ActionBuf,"freeStudy") == 0)
        {
            memset(MessageBuf,0,1024);
            sprintf(MessageBuf,"###ap_confirmfreestudy###{\"datetime\":\"%s\",\"data\":{\"action\":\"%s\",\"id\":\"%s\"}}", str_time.toStdString().c_str(), ActionBuf, g_strTerminalID);
            if (g_deamonflag)
            {
                g_Pproduce->send(MessageBuf,strlen(MessageBuf));
                g_pLog->WriteLog(0,"zhaosenhua send msg response freeStudy: %s", MessageBuf);
            }else
            {
                g_currclass_state = 0;
                _kill_spicy_eclass("spicy", "eclass_client_udp");
    #ifdef ARM
                system("rk3188_clean_display");
    #endif
                ReportMsg reportmsg;
                reportmsg.action = USER_WAITINGDLG_EXIT;
                call_msg_back(msg_respose, reportmsg);
                myHttp http;
                http.SetUrlIP(g_strServerIP);
                char JsonBuf[102400];
                char ClassName[MAXCLASS][100];
                char ClassID[MAXCLASS][100];
                bool Recode = false;
                memset(JsonBuf,0,102400);
                //qDebug("\nfreeStudy\n");
                sprintf(JsonBuf,"qp.filterWhere={\"room_name\":\"%s\"}",g_strRoomNum);
                g_pLog->WriteLog(0,"Freestudy Get Connect Post:%s",JsonBuf);
                http.Post("/service/desktop_pools/list",JsonBuf);
                memset(JsonBuf,0,102400);
                http.GetData(JsonBuf);
                //qDebug("Recv Connect:%s",JsonBuf);
                g_pLog->WriteLog(0,"Recv Connect:%s",JsonBuf);
                g_pJson->Parse(JsonBuf);
                g_pJson->ReadJson(&Recode,"success");
                if(Recode)
                {
                    g_loginWnd->m_pClassNameConfig->m_iClassNum = 0;
                    g_loginWnd->showFullScreen();
                    g_loginWnd->SetChecked();
                    int iRecode = g_pJson->ReadJson(ClassName,"data","list","name",MAXCLASS);
                    iRecode = g_pJson->ReadJson(ClassID,"data","list","id",MAXCLASS);
                    g_pLog->WriteLog(0,"Study:%d",iRecode);
                    for(int i = 0;i < iRecode;i++)
                    {
                        g_loginWnd->m_pClassNameConfig->AddClass(ClassName[i],ClassID[i]);
                    }
                    memset(g_szRetVm, 0, sizeof(g_szRetVm));
                    g_loginWnd->SetEnable(true);
                    g_loginWnd->m_pClassNameConfig->ChooseOne();
                    g_loginWnd->m_pClassNameConfig->SetLabelName();
                    g_Pproduce->send(MessageBuf,strlen(MessageBuf));
                    g_pLog->WriteLog(0,"zhaosenhua send msg response freeStudy: %s", MessageBuf);
                }
            }
        }
        if (strcmp(ActionBuf,"free_display") == 0)
		{
            system("sudo rm -f /tmp/data_*");
            g_currclass_state = 0;
            memset(MessageBuf, 0, 1024);
			 //connect vm
            char sz_host[100] = {0};
            char sz_port[50] = {0};
            char sz_vmid[100] = {0};
            //host
            g_pJson->ReadJson_v(sz_host, "data", "host");
            g_pJson->ReadJson_v(sz_port, "data", "port");
            g_pJson->ReadJson_v(sz_vmid, "data", "vmId");
            g_loginWnd->SetEnable(false);
#ifdef _DEBUG_PRINT
            qDebug("begin class IP:%s Port:%s VmID:%s", sz_host, sz_port, sz_vmid);
#endif
            g_pLog->WriteLog(0,"begin class IP:%s Port:%s VmID:%s", sz_host, sz_port, sz_vmid);
            sprintf(g_szCmd, "spicy -h %s -p %s -f >> %s 2>&1", sz_host, sz_port, SPICY_LOG_PATH);
            strcpy(g_szRetVm, g_szCmd);
            _display_vm(g_szCmd);
            g_currclass_state = 1;
            sprintf(MessageBuf,"###ap_confirmfreedisplay###{\"datetime\":\"%s\",\"data\":{\"action\":\"%s\",\"id\":\"%s\", \"apIp\":\"%s\",\"mac\":\"%s\", \"vmId\":\"%s\", \"connected\":%d, \"dsPort\":%ld, \"dsXor\":%ld}}", str_time.toStdString().c_str(), ActionBuf, g_strTerminalID, m_strIP, m_strMac, sz_vmid, 1, atol(port), atol(data_xor));
            g_Pproduce->send(MessageBuf, strlen(MessageBuf));
            g_pLog->WriteLog(0,MessageBuf);
		}
        g_myBuffer.clear();
        sleep(1);
    }
    //qDebug("MqMsgProcess deal amq msg process exit ! \n");
}

void MqMsgProcess::startMulticast()
{
    //170817 modify
    m_MultiMsg = new MultiMsg();
    m_MultiMsg->setParent(this);
    connect(m_MultiMsg, SIGNAL(broadcastMsg()), this, SLOT(on_processBroadCastMsg()));
    //get state
    QFile file(CONFIGDEMON);
    if (file.exists())
    {
        int nFlag = 0;
        GetConfigInt(CONFIGDEMON, "DEMON", "demon", 0, &nFlag);
        g_deamonflag = nFlag == 1?true:false;
        GetConfigInt(CONFIGDEMON, "DEMON", "demonObj", 0, &nFlag);
        g_showloal = nFlag == 1?true:false;
        GetConfigInt(CONFIGDEMON, "DEMON", "student", 0, &nFlag);
        g_bTeacherFlag = nFlag == 1?false:true;
        g_pLog->WriteLog(0,"MqMsgProcess::GetAddrMac read config deamonflag = %d, showlocal = %d, g_bTeacherFlag = %d.", g_deamonflag, g_showloal, g_bTeacherFlag);
        if (g_deamonflag)
        {
            int ntmp = g_bTeacherFlag ? 1:0;
            if (g_bTeacherFlag)

            {
                sprintf(g_szCommand, "eclass_client_udp -vh %s -vp %d -ah %s -ap %d -t %d >/usr/local/shencloud/log/Eclass_udp.log 2>&1", g_strMultiVideo, g_nMultiVideo, g_strMultiAudio, g_nMultiAudio, ntmp);
            }else
            {
                sprintf(g_szCommand, "eclass_client_udp -vh %s -vp %d -ah %s -ap %d -t %d >/usr/local/shencloud/log/Eclass_udp.log 2>&1", g_strMultiStudentV, g_nMultiStudentVP, g_strMultiStudentA, g_nMultiStudentAP, ntmp);
            }
            g_pLog->WriteLog(0,"MqMsgProcess::GetAddrMac read config g_szCommand = %s.", g_szCommand);
        }
    }
}

void MqMsgProcess::GetAddrMac()
{
    NetConfig netconfig;
    memset(m_strIP,0,20);
    netconfig.GetIPAddr(m_strIP);
    memset(m_strMac,0,50);
    netconfig.GetMacAdd(m_strMac,true);
    strcpy(g_szLocalIP, m_strIP);
}
/********************************************************************/
   //add 170517
/********************************************************************/
bool  g_spicyProcessExit = false;
static void *SpicyThrdFun(void *param)
{
    MqMsgProcess *Temp = (MqMsgProcess*)param;
    Temp->_spicyProcess();
    pthread_detach(pthread_self());
    return NULL;
}

void MqMsgProcess::strart_spicyThrd()
{
    GetAddrMac();
    if(pthread_create(&m_pidsp,NULL,SpicyThrdFun,this))
    {
        printf("Create Error!\n");
    }
}

void MqMsgProcess::_spicyProcess()
{
    int nRet = 0;
    while(!g_spicyProcessExit)
    {
        if (g_currclass_state == 1)
        {
            //connected vm
            nRet = detect_process("spicy");
            if (nRet == 0)
            {
                if (g_bSetupAmq)
                {
                   //int ret = ping_net(g_strServerIP);
                   //if (ret == 1)
                   {
                      QString strRet(g_szRetVm);
                      if (!strRet.isEmpty() || strRet.length() > 0)
                      {
                          if (g_currclass_state == 1)
                          {
                                g_pLog->WriteLog(0,"zhaosenhua MqMsgProcess::_spicyProcess reconnect vm.");
                                if (detect_process("spicy") == 0)
                                    _display_vm(g_szRetVm);
                          }
                      }
                   }
                }
            }else
            {
               //int ret = ping_net(g_strServerIP);
               //if (ret == 0 && !g_bSetupAmq)
                if (!g_bSetupAmq)
               {
                    _kill_spicy_eclass("spicy", NULL);
               }
            }
        }

        if (g_deamonflag)
        {
            nRet = detect_process("eclass_client_udp");
            if (nRet == 0)
            {
                if (g_bSetupAmq)
                {
                   //int ret = ping_net(g_strServerIP);
                   //if (ret == 1)
                   {
                       QFile file(DEMOFLAG);
                       if (!file.exists() && !g_showloal)
                       {
                          QString strRet(g_szCommand);
                          if (!strRet.isEmpty() || strRet.length() > 0)
                          {
                              g_pLog->WriteLog(0,"zhaosenhua MqMsgProcess::_spicyProcess reconnect eclass_client_udp.");
                              _system(g_szCommand);
                          }
                       }
                   }
                }
            }else
            {
               //int ret = ping_net(g_strServerIP);
               //if (ret == 0 && !g_bSetupAmq)
               if (!g_bSetupAmq)
               {
                    _kill_spicy_eclass(NULL, "eclass_client_udp");
               }
            }
        }
        usleep(500);
    }
    //g_pLog->WriteLog(0,"zhaosenhua MqMsgProcess::_spicyProcess exit !!!");
}

void *ThreadForSystemMsgProcess(void *param)
{
    g_pLog->WriteLog(0, "ThreadForSystem, _system !!!!!, command: %s", g_szCommand);
    _system(g_szCommand);
    pthread_detach(pthread_self());
    return NULL;
}

void MqMsgProcess::on_processBroadCastMsg()
{
    char ActionBuf[1024] = {0};
    char mac[30] = {0};
    char type[30] = {0};
    pthread_t pid;
    g_pLog->WriteLog(0,"on_processBroadCastMsg, start xxxxx : %s", g_brodMsg);
    g_pJson->Parse(g_brodMsg);
    g_pJson->ReadJson(ActionBuf,"action");
    g_pLog->WriteLog(0,"on_processBroadCastMsg, Action:%s", ActionBuf);
    g_pJson->ReadJson_v(mac, "data", "mac");
    g_pJson->ReadJson_v(type, "data", "type");
    g_pLog->WriteLog(0,"on_processBroadCastMsg, mac: %s, type: %s", mac, type);

    int iflag = 0;
    if (strcmp(ActionBuf, "start_broadcast") == 0)
    {
        iflag = 1;
        g_deamonflag = true;
        g_currclass_state = 0;
    }
    if (strcmp(ActionBuf, "stop_broadcast") == 0)
    {
        iflag = 2;
        g_deamonflag = false;
        g_currclass_state = 1;
    }

    if (strcmp(type, "teacher") == 0)
        g_bTeacherFlag = true;
    else
        g_bTeacherFlag = false;

    switch (iflag)
    {
        case 1: //start broadcast
            _kill_spicy_eclass(NULL, "eclass_client_udp");
            if (g_bTeacherFlag)
            {
                g_showloal = false;
                _kill_spicy_eclass("spicy", NULL);
                sprintf(g_szCommand, "eclass_client_udp -vh %s -vp %d -ah %s -ap %d -t 0 >/usr/local/shencloud/log/Eclass_udp.log 2>&1", g_strMultiVideo, g_nMultiVideo, g_strMultiAudio, g_nMultiAudio);
                g_pLog->WriteLog(0, "on_processBroadCastMsg, teacher start_broadcast command : %s.", g_szCommand);
                if (detect_process("eclass_client_udp") == 0)
                {
                    if(pthread_create(&pid, NULL,ThreadForSystemMsgProcess, NULL))
                    {
                         g_pLog->WriteLog(0, "on_processBroadCastMsg, teacher create Thread Error");
                         return;
                    }
                }
            }else
            {
                sprintf(g_szCommand, "eclass_client_udp -vh %s -vp %d -ah %s -ap %d -t 1 >/usr/local/shencloud/log/Eclass_udp.log 2>&1", g_strMultiStudentV, g_nMultiStudentVP, g_strMultiStudentA, g_nMultiStudentAP);
                if (strcmp(mac, m_strMac) == 0)
                {
                    QFile *file = new QFile(DEMOFLAG);
                    if (file->exists())
                    {
                        file->remove();
                    }
                    if (!file->open(QIODevice::WriteOnly))
                    {
                        g_pLog->WriteLog(0, "on_processBroadCastMsg, student create demoflag file failed.");
                        return;
                    }
                    g_showloal = true;
                    file->close();
                }else
                {
                    g_showloal = false;
                    _kill_spicy_eclass("spicy", NULL);
                    g_pLog->WriteLog(0, "on_processBroadCastMsg, student start_broadcast command : %s.", g_szCommand);
                    if (detect_process("eclass_client_udp") == 0)
                    {
                        if(pthread_create(&pid, NULL,ThreadForSystemMsgProcess, NULL))
                        {
                             g_pLog->WriteLog(0, "on_processBroadCastMsg, student create Thread Error");
                             return;
                        }
                    }
                }
            }
            break;
        case 2: //stop broadcast
            QString strRet(g_szRetVm);
            g_showloal = false;
            _kill_spicy_eclass(NULL, "eclass_client_udp");
            memset(g_szCommand, 0, sizeof(g_szCommand));
            if (!g_bTeacherFlag)
            {
                if (strcmp(mac, m_strMac) == 0)
                {
                    QFile *file = new QFile(DEMOFLAG);
                    if (file->exists())
                    {
                        file->remove();
                    }
                }else
                {
                    if (detect_process("spicy") == 0)
                    {
                        if (!strRet.isEmpty() || strRet.length() > 0)
                        {
                             g_pLog->WriteLog(0, "on_processBroadCastMsg, student over, demonstrate connect vm command : %s.", g_szRetVm);
                             _demo_display_vm(g_szRetVm);
                        }
                    }
                }
            }else
            {
                if (detect_process("spicy") == 0)
                {
                    if (!strRet.isEmpty() || strRet.length() > 0)
                    {
                         g_pLog->WriteLog(0, "on_processBroadCastMsg, teacher over, demonstrate connect vm command : %s.", g_szRetVm);
                         _demo_display_vm(g_szRetVm);
                    }
                }
            }
            break;
    }
    saveState();
}

//demon :1
//demonObj :1 teacher  0:student
void MqMsgProcess::saveState()
{
    int nflag = g_deamonflag == true ? 1:0;
    WriteConfigInt(CONFIGDEMON, "DEMON", "demon", nflag);
    nflag = g_showloal == true ? 1:0;
    WriteConfigInt(CONFIGDEMON, "DEMON", "demonObj", nflag);
    nflag = g_bTeacherFlag == true ?0:1;
    WriteConfigInt(CONFIGDEMON, "DEMON", "student", nflag);
}
