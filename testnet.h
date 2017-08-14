#ifndef TESTNET_H
#define TESTNET_H

#include <linux/sockios.h>	
#include <sys/socket.h>  
#include <sys/ioctl.h>	
#include <linux/if.h>  
#include <string.h>  
#include <stdio.h>	
#include <stdlib.h>  
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include "log.h"
#include <QDebug>

extern Log *g_pLog;
extern volatile bool g_bSetupAmq;
struct ethtool_value {
   __uint32_t cmd;
   __uint32_t data;
};
#define ETHTOOL_GLINK		 0x0000000a /* Get link status (ethtool_value) */
typedef enum { IFSTATUS_UP, IFSTATUS_DOWN, IFSTATUS_ERR } interface_status_t;  
static char hw_name[10] = {'\0'};
static pthread_t g_nettid = NULL;
static pthread_t g_amqpid = NULL;

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static interface_status_t interface_detect_beat_ethtool(int fd, char *iface)
{  
    ethtool_value edata;
    edata.cmd = ETHTOOL_GLINK;
	struct ifreq ifr;   
	 
    memset(&ifr, 0, sizeof(ifr));
    ifr.ifr_data = (caddr_t)&edata;

    strncpy(ifr.ifr_name, iface, sizeof(ifr.ifr_name)-1);

    if (ioctl(fd, /*SIOCGIFFLAGS*/SIOCETHTOOL, &ifr) == -1)
    {
        perror("ETHTOOL_GLINK failed ");
        g_pLog->WriteLog(0, "detect_beat_ethtool ethernet interface %s is not valid .\n");
        return IFSTATUS_ERR;
    }
    return edata.data ? IFSTATUS_UP : IFSTATUS_DOWN;
//    if (ifr.ifr_flags & IFF_RUNNING)
//    {
//        return IFSTATUS_UP;
//    }else
//    {
//        return IFSTATUS_DOWN;
//    }
//    return IFSTATUS_UP;
}  

static void gethw_name()
{
	FILE *fp;  
	char buf[512] = {'\0'};  
	char *token = NULL;  
  
	/* 获取网卡名称 */	
	if ((fp = fopen("/proc/net/dev", "r")) != NULL)  
	{  
		while (fgets(buf, sizeof(buf), fp) != NULL)  
		{  
			if(strstr(buf, "eth") != NULL)	
			{		  
				token = strtok(buf, ":");  
				while (*token == ' ') ++token;	
				strncpy(hw_name, token, strlen(token));  
			}  
		}  
	}  
	fclose(fp);  
}

//extern bool g_bpause_net_check;
static void* thrd_net_setup(void * param)
{
	for (;;)
	{
//        if (g_bpause_net_check)
//        {
//            qDebug() << "thrd_net_setup  net checking .....";
//            sleep(2);
//            continue;
//        }
		int fd;
		interface_status_t status;
		if((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)  
		{  
            perror("socket ");
            g_pLog->WriteLog(0, "thrd_net_setup socket error, thrd_net_setup exit .\n");
            //qDebug() << " thrd_net_setup socket error, thrd_net_setup exit. \n";
			break; 
		}
        //g_pLog->WriteLog(0, "thrd_net_setup 333333333333333333 .\n");
		status = interface_detect_beat_ethtool(fd, hw_name);  
		close(fd);
		if (status == IFSTATUS_UP)
		{
            //g_pLog->WriteLog(0, "thrd_net_setup 22222222222222222222  normal .\n");
            //qDebug() << " thrd_net_setup 22222222222222222222  normal. ";
            //pthread_mutex_lock(&mutex);
            g_bSetupAmq = true;
            //pthread_mutex_unlock(&mutex);
		}
		if (status == IFSTATUS_DOWN)
		{
            g_pLog->WriteLog(0, "thrd_net_setup network not ping. \n");
            //pthread_mutex_lock(&mutex);
            g_bSetupAmq = false;
            //pthread_mutex_unlock(&mutex);
		}
//        if (g_amqpid != NULL)
//        {
//            int pthread_kill_err = pthread_kill(g_amqpid, 0);
//            if (pthread_kill_err == ESRCH)
//            {
//                //g_pLog->WriteLog(0, "thrd_net_setup 22222222222222222222 amq thread not exist or exit.\n");
//            }else if (pthread_kill_err == EINVAL)
//            {
//                //g_pLog->WriteLog(0, "thrd_net_setup 22222222222222222222 amq thread send einval signal.\n");
//            }else
//            {
//                //g_pLog->WriteLog(0, "thrd_net_setup 22222222222222222222 amq thread is alive.\n");
//            }
//        }
		sleep(5);
	}
}

static void wait_net_setup()
{
	gethw_name();
    if ( pthread_create(&g_nettid, NULL, thrd_net_setup, NULL) !=0 )
	{
        g_pLog->WriteLog(0, "zhaosenhua wait_net_setup create thread failed.");
		printf("Create thread error!\n");
	};
    //pthread_join(tid, NULL);
}

#endif // TESTNET_H
