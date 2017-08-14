#include "global.h"
#include <stdlib.h>
#include "include.h"
#include "testnet.h"
#include <QDebug>
#include "loginwidget.h"
#include "waitstu2.h"

extern LoginWidget * g_loginWnd;
extern void *InitThread(void *param);
bool    g_exitMonitoramq;
pthread_t g_monitoramq;
volatile bool g_resetamq;
volatile bool g_bSetupAmq;
void call_msg_back(MsgCallBackFun fun, ReportMsg msg)
{
    fun(msg);
}

static pthread_t g_waitid = 0;
static pthread_t g_amqpid2 = 0;
bool g_bshowwaitstu = false;

extern pthread_t g_contid;
extern void *thrd_connect(void *);
extern pthread_mutex_t g_freestudyMutex;
extern bool  g_bExit_freeStuy_flag;

static void *thrd_waitexec(void *param)
{
    g_pLog->WriteLog(0,"zhaosenhua, msg_respose waitstu show. \n");
    //qDebug() << "xxxxx show waitstu";
    FILE *fp;
    if ((fp = popen("/usr/local/shencloud/waitstu", "r")) == NULL)
    {
        g_pLog->WriteLog(0, "msg_respose  USER_WAITINGDLG_SHOW  failed.");
    }
    pclose(fp);
    //qDebug() << "xxxxx show waitstu finished.";
    return NULL;
}

static void *thrd_amq(void *param)
{
    while(true)
    {
        if (g_exitMonitoramq)
            break;
        if (g_resetamq && g_bSetupAmq)
        {
            ReportMsg reportmsg;
            reportmsg.action = USER_WAITINGDLG_RESET;
            call_msg_back(msg_respose, reportmsg);
            g_resetamq = false;
        }
        sleep(2);
    }
}

void amq_monitor()
{
    if(pthread_create(&g_monitoramq,NULL,thrd_amq, NULL))
    {
        printf("create Thread Error");
    }
}

void msg_respose(ReportMsg msg)
{
    switch (msg.action)
    {
        case USER_WAITINGDLG_SHOW:
            {
                //show waiting dlg show
                if (!g_bshowwaitstu)
                {
                    //PIPE_WAIT("0");
//                    if(pthread_create(&g_waitid,NULL,thrd_waitexec, NULL))
//                    {
//                        printf("create Thread Error");
//                    }
                    if (g_loginWnd != NULL)
                    {
                        //g_loginWnd->move(-1920, -1080);
                        g_loginWnd->m_waitstu->waitstushow();
                    }
                    g_bshowwaitstu = true;
                }
            }
            break;
        case USER_WAITINGDLG_EXIT:
            {
                //qDebug() << "xxxxx starting exit waitstu";
                g_pLog->WriteLog(0,"zhaosenhua, msg_respose waitstu exit. \n");
                g_bshowwaitstu = false;
                //PIPE_WAIT("1");
                if (g_loginWnd != NULL)
                {
                    //g_loginWnd->m_waitstuDialog->WaitStuHide();
                    // g_loginWnd->move(0, 0);
                    g_loginWnd->show();
                    g_loginWnd->m_waitstu->waitstuhide();
                    g_loginWnd->SetEnable(true);
                    pthread_mutex_lock(&g_freestudyMutex);
                    g_bExit_freeStuy_flag = false;
                    pthread_mutex_unlock(&g_freestudyMutex);
                }
                //qDebug() << "xxxxx exit waitstu";
            }
            break;
        case USER_WAITINGDLG_RESET:
        {
//            g_pLog->WriteLog(0,"zhaosenhua, msg_respose reset amq prcess. \n");
//            qDebug() << "zhaosenhua, msg_respose reset amq prcess. \n";
//            if (g_Pconsume != NULL)
//            {
//                g_Pconsume->runConsumer();
//                //g_resetamq = false;
//            }
        }
        break;
        case USER_AMQ_RESET:
        {
//            g_pLog->WriteLog(0,"zhaosenhua, msg_respose reset amq prcess, network is unreachable. \n");
//            if (NULL != g_Pconsume)
//            {
//                g_Pconsume->cleanup();
//            }
//            if (NULL != g_Pproduce)
//            {
//                g_Pproduce->cleanup();
//            }
//            if(pthread_create(&g_amqpid2, NULL,InitThread, g_loginWnd))
//            {

//            }
//            qDebug() << "zhaosenhua, msg_respose reset amq prcess. network is unreachable.\n";
        }
        break;
	   case USET_MSG_PROCESSMSG:
	   	{
			
		}
	   break;
    case USER_MSG_FREESTUDY:
        {
			g_pLog->WriteLog(0,"zhaosenhua, classmould  double  free study. \n");
			if(pthread_create(&g_contid,NULL,thrd_connect, NULL))
			{
			        printf("create Thread Error");
			}
        }
        break;
    case USER_MSG_AMQPRODUCE:
        {
//            if (NULL != g_Pproduce)
//            {
//                g_Pproduce->cleanup();
//                g_Pproduce = NULL;
//            }
//            if(g_Pproduce == NULL)
//            {
//                g_Pproduce = new ActiveMQProduce();
//                g_Pproduce->start(g_strProduceAdd,20,g_strProduceQueue,false,false);
//            }
        }
        break;
     case USER_MSG_HIDEWINDOW:
        {
            if (g_loginWnd != NULL)
            {
                //g_loginWnd->move(0, 0);
                g_loginWnd->hide();
                g_loginWnd->m_waitstu->waitstuhide();
            }
        }
        break;
        default:
            break;
    }
}
