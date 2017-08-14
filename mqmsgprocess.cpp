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
int g_currclass_state = 0;
extern bool g_bSetupAmq;
char   g_szCommand[1024] = {0};

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
            sprintf(szCmd, "sudo killall %s", spicy);
	        _system(szCmd);
		    g_pLog->WriteLog(0,"_kill_spicy_eclass kill spicy.");
	    }
	}
	if (eclass != NULL && strcmp("", eclass) != 0)
	{
	    if (detect_process(eclass) == 1)
	    {
            sprintf(szCmd, "sudo killall %s", eclass);
	        _system(szCmd);
            g_pLog->WriteLog(0,"_kill_spicy_eclass kill eclass_client_udp.");
	    }
	}
#ifdef ARM
	_system("sudo rk3188_clean_display");
#endif
}

char port[20];
char data_xor[20];
void _display_vm(char *cmd)
{
    pthread_t pid;
    if(pthread_create(&pid, NULL,ThreadForSystem, g_szCmd))
    {
        printf("create Thread Error");
    }
    int nCount = 0;
    while(access("/tmp/data_port",F_OK))
    {
        if (nCount >= 50)
            break;
        usleep(100000);
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
}

void _demo_display_vm(char *)
{
	pthread_t pid;
    if(pthread_create(&pid, NULL,DemoThreadForSystem, g_szCmd))
    {
        printf("create Thread Error");
    }
    int nCount = 0;
    while(access("/tmp/data_port",F_OK))
    {
        if (nCount >= 20)
            break;
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
    return NULL;
}

void MqMsgProcess::start()
{
    GetAddrMac();
    m_MultiMsg = new MultiMsg();
    m_MultiMsg->setParent(this);
    connect(m_MultiMsg, SIGNAL(broadcastMsg()), this, SLOT(on_processBroadCastMsg()));
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
    char ActionBuf[100] = {0};
    char datetime[100] = {0};
#ifdef ARM
    long long last_time = 0;
#else
	long last_time = 0;
#endif

    char MessageBuf[1024];
    memset(MessageBuf,0,1024);
    cMainExitFlag = 1;
    int nRet = 0;
    while(cMainExitFlag)
    {
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
              continue;
        }
        g_pJson->ReadJson(ActionBuf,"action");
        g_pLog->WriteLog(0,"Action:%s",ActionBuf);
#ifdef _DEBUG_PRINT
        qDebug("Action:%s",ActionBuf);
#endif
        if (strcmp(ActionBuf, "unReconnect") == 0)
        {
            pthread_mutex_lock(&m_stateMutex);
            g_currclass_state = 0;
            memset(g_szRetVm, 0, sizeof(g_szRetVm));
            pthread_mutex_unlock(&m_stateMutex);
        }
		if (strcmp(ActionBuf,"heartbeat") == 0)
		{
            g_pLog->WriteLog(0,"zhaosenhua send msg heartbeat enter.");
			pthread_mutex_lock(&g_hreadMutex);
			//st_heart_time = __GetTime();
			g_check_heart_flag = 0;
			pthread_mutex_unlock(&g_hreadMutex);
//            //test
//            memset(MessageBuf,0,1024);
//            sprintf(MessageBuf,"###ap_confirmheartbeat###{\"datetime\":\"%s\",\"data\":{\"action\":\"%s\",\"id\":\"%s\"}}", str_time.toStdString().c_str(), ActionBuf, g_strTerminalID);
//            g_Pproduce->send(MessageBuf, strlen(MessageBuf));
//            g_pLog->WriteLog(0,"zhaosenhua send msg response heartbeat: %s", MessageBuf);
//            //test
#ifdef _DEBUG_PRINT
            qDebug() << "aciton : heartbeat";
#endif
		}//heart
        if (strcmp(ActionBuf,"classbegin") == 0)
        {
            ReportMsg reportmsg;
            reportmsg.action = USER_WAITINGDLG_SHOW;
            call_msg_back(msg_respose, reportmsg);
			pthread_mutex_lock(&g_freestudyMutex);
			g_bExit_freeStuy_flag = true;
			pthread_mutex_unlock(&g_freestudyMutex);
            pthread_mutex_lock(&m_stateMutex);
            g_currclass_state = 0;
            pthread_mutex_unlock(&m_stateMutex);
        }//begin
        if (strcmp(ActionBuf,"display") == 0)
        {
            pthread_mutex_lock(&g_freestudyMutex);
			g_bExit_freeStuy_flag = true;
			pthread_mutex_unlock(&g_freestudyMutex);
            //connect vm
            char sz_host[100] = {0};
            char sz_port[50] = {0};
            char sz_vmid[100] = {0};
            //host
            g_pJson->ReadJson_v(sz_host, "data", "host");
            g_pJson->ReadJson_v(sz_port, "data", "port");
            g_pJson->ReadJson_v(sz_vmid, "data", "vmId");
            g_loginWnd->SetEnable(false);
            system("sudo rm -f /tmp/data_*");
#ifdef _DEBUG_PRINT
            qDebug("begin class IP:%s Port:%s VmID:%s", sz_host, sz_port, sz_vmid);
#endif
            g_pLog->WriteLog(0,"begin class IP:%s Port:%s VmID:%s", sz_host, sz_port, sz_vmid);
            sprintf(g_szCmd, "sudo spicy -h %s -p %s -f > %s 2>&1", sz_host, sz_port, "/usr/local/shencloud/log/spicy.log");
            //sprintf(g_szCmd, "sudo spicy -h %s -p %s -f", sz_host, sz_port);
#ifdef _DEBUG_PRINT
            qDebug() << g_szCmd;
#endif
            strcpy(g_szRetVm, g_szCmd);
		    //=======add by linyuhua================//
		    nRet = detect_process("spicy");
            if (nRet == 1)
            {
                char ip[64];
				int nRet2 = 0;
				int port1;
				memset(ip, 0, 64);
				get_spicy_ip(ip);
				port1 = get_spicy_port();
				if((strcmp(ip, sz_host) != 0) || (atol(sz_port) != port1))
				{
					_display_vm(g_szCmd);
				}
				nRet2 = detect_process("spicy");
				if (nRet2 == 1)
				{
                    pthread_mutex_lock(&m_stateMutex);
                    g_currclass_state = 1;
                    pthread_mutex_unlock(&m_stateMutex);
                    g_pLog->WriteLog(0,"/tmp/data_port 00000:%ld", port1);
                    g_pLog->WriteLog(0,"/tmp/data_xor 00000:%ld", atol(data_xor));
					sprintf(MessageBuf,"###ap_confirmdisplay###{\"datetime\":\"%s\",\"data\":{\"action\":\"%s\",\"id\":\"%s\", \"apIp\":\"%s\",\"mac\":\"%s\", \"vmId\":\"%s\", \"connected\":%d, \"dsPort\":%ld, \"dsXor\":%ld}}", str_time.toStdString().c_str(), ActionBuf, g_strTerminalID, m_strIP, m_strMac, sz_vmid, 1, atol(port), atol(data_xor));
					g_Pproduce->send(MessageBuf, strlen(MessageBuf));
                    g_pLog->WriteLog(0,"zhaosenhua send msg response display 00000: %s", MessageBuf);
				}
			//=======add by linyuhua end================//
            	}else
            	{
			   _display_vm(g_szCmd);
	            nRet = detect_process("spicy");
	            if (nRet == 1)
	            {
                    pthread_mutex_lock(&m_stateMutex);
                    g_currclass_state = 1;
                    pthread_mutex_unlock(&m_stateMutex);
	                g_pLog->WriteLog(0,"/tmp/data_port 11111:%ld", atol(port));
	                g_pLog->WriteLog(0,"/tmp/data_xor 11111:%ld", atol(data_xor));
	                sprintf(MessageBuf,"###ap_confirmdisplay###{\"datetime\":\"%s\",\"data\":{\"action\":\"%s\",\"id\":\"%s\", \"apIp\":\"%s\",\"mac\":\"%s\", \"vmId\":\"%s\", \"connected\":%d, \"dsPort\":%ld, \"dsXor\":%ld}}", str_time.toStdString().c_str(), ActionBuf, g_strTerminalID, m_strIP, m_strMac, sz_vmid, 1, atol(port), atol(data_xor));
	                g_Pproduce->send(MessageBuf, strlen(MessageBuf));
	                g_pLog->WriteLog(0,"zhaosenhua send msg response display: %s", MessageBuf);
	            }
			}
            //qDebug("display response end.\n");
        }//display
        if (strcmp(ActionBuf,"classover") == 0)
        {
            //qDebug("classover enter.");
            pthread_mutex_lock(&m_stateMutex);
            g_currclass_state = 0;
            pthread_mutex_unlock(&m_stateMutex);
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
        if (strcmp(ActionBuf,"start_demonstrate") == 0)
        {
            //qDebug("start_demonstrate enter.");
            pthread_mutex_lock(&m_stateMutex);
            g_currclass_state = 0;
            pthread_mutex_unlock(&m_stateMutex);
            memset(MessageBuf,0,1024);
            g_loginWnd->SetEnable(false);
            char szCommand[1024] = {0};
            g_pJson->ReadJson_v(szCommand, "data", "command");
            //qDebug("start_demonstrate command : %s.\n", szCommand);
            g_pLog->WriteLog(0,"start_demonstrate command : %s", szCommand);
            //system("sudo /usr/local/shencloud/disable_input.sh &");
            strcat(szCommand, " >/usr/local/shencloud/log/Eclass.log 2>&1");
            pthread_t pid;
           int nRet = detect_process("eclass_client_udp");
		    //test
		   char sztmp[512] = {0};
		   sprintf(sztmp, "start_demonstrate, detect_process(), nRet = %d.", nRet);
		   g_pLog->WriteLog(0,sztmp);
		   //test
            if (nRet == 0)
            {
	            if(pthread_create(&pid, NULL,ThreadForSystem, szCommand))
	            {
	                printf("create Thread Error");
	            }
//			   nRet = detect_process("eclass_client");
//			   if (nRet == 1)
//			   {
//	                sprintf(MessageBuf,"###ap_confirmstartdemonstrate###{\"datetime\":\"%s\",\"data\":{\"action\":\"%s\",\"id\":\"%s\"}}", str_time.toStdString().c_str(), ActionBuf, g_strTerminalID);
//	                g_Pproduce->send(MessageBuf, strlen(MessageBuf));
//	                g_pLog->WriteLog(0,"zhaosenhua send msg response startdemonstrate: %s", MessageBuf);
//			   }
            }
			//170505
           nRet = detect_process("eclass_client_udp");
		   if (nRet == 1)
		   {
                sprintf(MessageBuf,"###ap_confirmstartdemonstrate###{\"datetime\":\"%s\",\"data\":{\"action\":\"%s\",\"id\":\"%s\"}}", str_time.toStdString().c_str(), ActionBuf, g_strTerminalID);
                g_Pproduce->send(MessageBuf, strlen(MessageBuf));
                g_pLog->WriteLog(0,"zhaosenhua send msg response startdemonstrate: %s", MessageBuf);
		   }
		   //170505 end
           // qDebug("start_demonstrate end.");
        }//start_demonstrate
        if (strcmp(ActionBuf,"stop_demonstrate") == 0)
        {
            pthread_mutex_lock(&m_stateMutex);
            g_currclass_state = 0;
            pthread_mutex_unlock(&m_stateMutex);
            memset(MessageBuf,0,1024);
            //qDebug("stop_demonstrate enter.");
            //system("sudo /usr/local/shencloud/enable_input.sh &");
            memset(MessageBuf,0,1024);
            _kill_spicy_eclass(NULL, "eclass_client_udp");
            nRet = detect_process("eclass_client_udp");
            g_loginWnd->SetEnable(true);
		   //test
		   char sztmp[512] = {0};
		   sprintf(sztmp, "stop_demonstrate, detect_process(), nRet = %d.", nRet);
		   g_pLog->WriteLog(0,sztmp);
		   //test
//            if (nRet == 0)
//            {
//                g_pLog->WriteLog(0,"stop_demonstrate, amq response.");
//                sprintf(MessageBuf,"###ap_confirmstopdemonstrate###{\"datetime\":\"%s\",\"data\":{\"action\":\"%s\",\"id\":\"%s\"}}", str_time.toStdString().c_str(), ActionBuf, g_strTerminalID);
//                g_Pproduce->send(MessageBuf,strlen(MessageBuf));
//                g_pLog->WriteLog(0,"zhaosenhua send msg response stopdemonstrate: %s", MessageBuf);
//            }
//            QString strRet(g_szRetVm);
//            if (!strRet.isEmpty() || strRet.length() > 0 && (detect_process("spicy") == 0))
//            {
//                _demo_display_vm(g_szRetVm);
//                qDebug("stop_demonstrate, reset desktop.");
//            }
             //170505
		    QString strRet(g_szRetVm);
			if (nRet == 0 && (strRet.isEmpty() || strRet.length() == 0))
			{
			   g_pLog->WriteLog(0,"stop_demonstrate, amq response, g_szRetVm == ""and don't connect vm.");
                sprintf(MessageBuf,"###ap_confirmstopdemonstrate###{\"datetime\":\"%s\",\"data\":{\"action\":\"%s\",\"id\":\"%s\"}}", str_time.toStdString().c_str(), ActionBuf, g_strTerminalID);
                g_Pproduce->send(MessageBuf,strlen(MessageBuf));
                g_pLog->WriteLog(0,"zhaosenhua send msg response stopdemonstrate: %s", MessageBuf);
			}
			if (nRet == 0 && (!strRet.isEmpty() || strRet.length() > 0))
			{
				 _demo_display_vm(g_szRetVm);
				 nRet = detect_process("spicy");
				 if (nRet == 1)
				 {
                     pthread_mutex_lock(&m_stateMutex);
                     g_currclass_state = 1;
                     pthread_mutex_unlock(&m_stateMutex);
					g_pLog->WriteLog(0,"stop_demonstrate, amq response, g_szRetVm != null, connect vm.");
					sprintf(MessageBuf,"###ap_confirmstopdemonstrate###{\"datetime\":\"%s\",\"data\":{\"action\":\"%s\",\"id\":\"%s\"}}", str_time.toStdString().c_str(), ActionBuf, g_strTerminalID);
					g_Pproduce->send(MessageBuf,strlen(MessageBuf));
					g_pLog->WriteLog(0,"zhaosenhua send msg response stopdemonstrate: %s", MessageBuf);
				 }
			}
			//170505 end 
            //qDebug("stop_demonstrate end.");
        }//stop_demonstrate
        if (strcmp(ActionBuf,"freeStudy") == 0)
        {
            pthread_mutex_lock(&m_stateMutex);
            g_currclass_state = 0;
            pthread_mutex_unlock(&m_stateMutex);
            ReportMsg reportmsg;
            reportmsg.action = USER_WAITINGDLG_EXIT;
            call_msg_back(msg_respose, reportmsg);
            myHttp http;
            http.SetUrlIP(g_strServerIP);
            memset(MessageBuf,0,1024);
            sprintf(MessageBuf,"###ap_confirmfreestudy###{\"datetime\":\"%s\",\"data\":{\"action\":\"%s\",\"id\":\"%s\"}}", str_time.toStdString().c_str(), ActionBuf, g_strTerminalID);
            char JsonBuf[102400];
            char ClassName[MAXCLASS][100];
            char ClassID[MAXCLASS][100];
            g_bExit_freeStuy_flag = false;
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
                _kill_spicy_eclass("spicy", "eclass_client_udp");
                memset(g_szRetVm, 0, sizeof(g_szRetVm));
                g_loginWnd->m_pClassNameConfig->m_iClassNum = 0;
                g_loginWnd->show();
                g_loginWnd->SetChecked();
                g_Pproduce->send(MessageBuf,strlen(MessageBuf));
                g_pLog->WriteLog(0,"zhaosenhua send msg response freeStudy: %s", MessageBuf);
                int iRecode = g_pJson->ReadJson(ClassName,"data","list","name",MAXCLASS);
                iRecode = g_pJson->ReadJson(ClassID,"data","list","id",MAXCLASS);
                g_pLog->WriteLog(0,"Study:%d",iRecode);
                for(int i = 0;i < iRecode;i++)
                {
                    g_loginWnd->m_pClassNameConfig->AddClass(ClassName[i],ClassID[i]);
                }
                g_loginWnd->SetEnable(true);
                g_loginWnd->m_pClassNameConfig->ChooseOne();
                g_loginWnd->m_pClassNameConfig->SetLabelName();
            }
        }
        if (strcmp(ActionBuf,"free_display") == 0)
		{
            pthread_mutex_lock(&m_stateMutex);
            g_currclass_state = 0;
            pthread_mutex_unlock(&m_stateMutex);
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
            system("sudo rm -f /tmp/data_*");
#ifdef _DEBUG_PRINT
            qDebug("begin class IP:%s Port:%s VmID:%s", sz_host, sz_port, sz_vmid);
#endif
            g_pLog->WriteLog(0,"begin class IP:%s Port:%s VmID:%s", sz_host, sz_port, sz_vmid);
            sprintf(g_szCmd, "sudo spicy -h %s -p %s -f > %s 2>&1", sz_host, sz_port, "/usr/local/shencloud/log/spicy.log");
           _kill_spicy_eclass("spicy", "eclass_client_udp");
		    _display_vm(g_szCmd);
	        nRet = detect_process("spicy");
            if (nRet == 1)
            {
                pthread_mutex_lock(&m_stateMutex);
                g_currclass_state = 1;
                pthread_mutex_unlock(&m_stateMutex);
				pthread_mutex_lock(&g_freestudyMutex);
				g_bExit_freeStuy_flag = true;
				pthread_mutex_unlock(&g_freestudyMutex);
                sprintf(MessageBuf,"###ap_confirmfreedisplay###{\"datetime\":\"%s\",\"data\":{\"action\":\"%s\",\"id\":\"%s\", \"apIp\":\"%s\",\"mac\":\"%s\", \"vmId\":\"%s\", \"connected\":%d, \"dsPort\":%ld, \"dsXor\":%ld}}", str_time.toStdString().c_str(), ActionBuf, g_strTerminalID, m_strIP, m_strMac, sz_vmid, 1, atol(port), atol(data_xor));

                g_Pproduce->send(MessageBuf, strlen(MessageBuf));
				g_pLog->WriteLog(0,MessageBuf);
				
            }else
            {
				pthread_mutex_lock(&g_freestudyMutex);
				g_bExit_freeStuy_flag = false;
				pthread_mutex_unlock(&g_freestudyMutex);
				g_pLog->WriteLog(0,"zhaosenhua freeDisplay connect vm failed.");
		   }
		}
	    g_myBuffer.clear();
        sleep(1);
    }
    //qDebug("MqMsgProcess deal amq msg process exit ! \n");
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
        pthread_mutex_lock(&m_stateMutex);
        if (g_currclass_state == 1)
        {
            //connected vm
            nRet = detect_process("spicy");
            if (nRet == 0)
            {
                if (g_bSetupAmq)
                {
                   int ret = ping_net(g_strServerIP);
                   if (ret == 1)
                   {
                      QString strRet(g_szRetVm);
                      if (!strRet.isEmpty() || strRet.length() > 0)
                      {
                          if (g_currclass_state == 1)
                                _display_vm(g_szRetVm);
                          g_pLog->WriteLog(0,"zhaosenhua MqMsgProcess::_spicyProcess reconnect vm.");
                      }
                   }
                }
            }else
            {
               int ret = ping_net(g_strServerIP);
               if (ret == 0 || !g_bSetupAmq)
               {
                   if (detect_process("spicy") == 1)
                   {
                       _kill_spicy_eclass("spicy", NULL);
                   }
                   if (detect_process("eclass_client_udp") == 1)
                   {
                       g_pLog->WriteLog(0,"zhaosenhua MqMsgProcess::__kill eclass_client_udp!");
                       //_kill_spicy_eclass("eclass_client_udp", NULL);
                   }
               }
            }
        }
        pthread_mutex_unlock(&m_stateMutex);
        //qDebug() << "MqMsgProcess::_spicyProcess loop.";
        sleep(2);
    }
    g_pLog->WriteLog(0,"zhaosenhua MqMsgProcess::_spicyProcess exit !!!");
}

#define   DEMOFLAG   "/tmp/DemoFlag"
void *ThreadForSystemMsgProcess(void *param)
{
    g_pLog->WriteLog(0, "ThreadForSystem, _system !!!!!, command: %s", g_szCommand);
    _system(g_szCommand);
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
        iflag = 1;
    if (strcmp(ActionBuf, "stop_broadcast") == 0)
        iflag = 2;

    bool bTeacherFlag = false;
    if (strcmp(type, "teacher") == 0)
        bTeacherFlag = true;
    else
        bTeacherFlag = false;
    switch (iflag)
    {
        case 1: //start broadcast
            if (detect_process("eclass_client_udp") == 1)
            {
                 _kill_spicy_eclass(NULL, "eclass_client_udp");
            }
            if (bTeacherFlag)
            {
                if (detect_process("spicy") == 1)
                {
                    _kill_spicy_eclass("spicy", NULL);
                }
                sprintf(g_szCommand, "sudo eclass_client_udp -vh %s -vp %d -ah %s -ap %d -t 0 >/usr/local/shencloud/log/Eclass_udp.log 2>&1", g_strMultiVideo, g_nMultiVideo, g_strMultiAudio, g_nMultiAudio);
                g_pLog->WriteLog(0, "on_processBroadCastMsg, teacher start_broadcast command : %s.", g_szCommand);
                if(pthread_create(&pid, NULL,ThreadForSystemMsgProcess, NULL))
                {
                     g_pLog->WriteLog(0, "on_processBroadCastMsg, teacher create Thread Error");
                     return;
                }
            }else
            {
                sprintf(g_szCommand, "sudo eclass_client_udp -vh %s -vp %d -ah %s -ap %d -t 1 >/usr/local/shencloud/log/Eclass_udp.log 2>&1", g_strMultiStudentV, g_nMultiStudentVP, g_strMultiStudentA, g_nMultiStudentAP);
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
                    file->close();
                }else
                {
                    if (detect_process("spicy") == 1)
                    {
                        _kill_spicy_eclass("spicy", NULL);
                    }
                    g_pLog->WriteLog(0, "on_processBroadCastMsg, student start_broadcast command : %s.", g_szCommand);
                    if(pthread_create(&pid, NULL,ThreadForSystemMsgProcess, NULL))
                    {
                         g_pLog->WriteLog(0, "on_processBroadCastMsg, student create Thread Error");
                         return;
                    }
                }
            }
            break;
        case 2: //stop broadcast
            QString strRet(g_szRetVm);
            if (detect_process("eclass_client_udp") == 1)
            {
                _kill_spicy_eclass(NULL, "eclass_client_udp");
            }
            if (!bTeacherFlag)
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
                    if (!strRet.isEmpty() || strRet.length() > 0)
                    {
                         _demo_display_vm(g_szRetVm);
                    }
                }
            }else
            {
                if (!strRet.isEmpty() || strRet.length() > 0)
                {
                     _demo_display_vm(g_szRetVm);
                }
            }
            break;
    }
}

