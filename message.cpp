#include "global.h"
#include <stdlib.h>
#include "include.h"
#include "testnet.h"
#include <QDebug>
#include "loginwidget.h"
#include "waitstu2.h"

extern LoginWidget * g_loginWnd;
extern waitstu2  *m_waitstu;
extern void *InitThread(void *param);
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

void msg_respose(ReportMsg msg)
{
    switch (msg.action)
    {
        case USER_WAITINGDLG_SHOW:
            {
                //show waiting dlg show
                if (!g_bshowwaitstu)
                {
                    if (g_loginWnd != NULL)
                    {
                        //g_loginWnd->move(-1920, -1080);
                        g_loginWnd->hide();
                        m_waitstu->waitstushow();
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
                if (g_loginWnd != NULL)
                {
                    g_loginWnd->showFullScreen();
                    m_waitstu->waitstuhide();
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
        }
        break;
        case USER_AMQ_RESET:
        {
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
        }
        break;
     case USER_MSG_HIDEWINDOW:
        {
            if (g_loginWnd != NULL)
            {
                //g_loginWnd->move(0, 0);
                g_loginWnd->hide();
                m_waitstu->waitstuhide();
            }
        }
        break;
        default:
            break;
    }
}
