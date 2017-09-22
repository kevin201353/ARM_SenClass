#ifndef __NETLINK__
#define __NETLINK__

#include <linux/sockios.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <linux/mii.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include "log.h"
#include <QDebug>
#include "include.h"

static char hw_name[10] = {'\0'};
static char carrier_path[512] = {'\0'};
static int detect_ethtool(int skfd, char *ifname);
static int detect_mii(int skfd, char *ifname);
struct ethtool_value {
   __uint32_t cmd;
   __uint32_t data;
};
static pthread_t g_nettid = NULL;
extern volatile bool g_bSetupAmq;


#define ETHTOOL_GLINK		 0x0000000a /* Get link status (ethtool_value) */
/*--------------------------------------------------------------------------*/
/**
  @brief    Detect eth link status from ETHTOOL API and MII reg.
  @param    ifname -- interface name.
  @return   Return true if link is up, return false if link is down,
            or return -1 if errors occur.
  This function will first try to get eth link status from ETHTOOL API.
  If it fails, it will try to get eth link status from MII reg.
 */
/*--------------------------------------------------------------------------*/
static int get_netlink_status(char *ifname)
{
    int skfd = -1;
    int link_status;
    if (ifname == NULL) {
        ifname = "eth0";
    }
   
    if ((skfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("socket error\n");
        return -1;
    }
    link_status = detect_ethtool(skfd, ifname);
    if (link_status < 0)
        link_status = detect_mii(skfd, ifname);
    close(skfd);
    return link_status;
}

/*--------------------------------------------------------------------------*/
/**
  @brief    Detect eth link status from ETHTOOL API.
  @param    skfd   -- socket handler.
  @param    ifname -- interface name.
  @return   Return true if link is up, return false if link is down,
            or return -1 if errors occur.
 */
/*--------------------------------------------------------------------------*/
static int detect_ethtool(int skfd, char *ifname)
{
    struct ifreq ifr;
    struct ethtool_value edata;
    memset(&ifr, 0, sizeof(ifr));
    edata.cmd = ETHTOOL_GLINK;
    strncpy(ifr.ifr_name, ifname, sizeof(ifr.ifr_name)-1);
    ifr.ifr_data = (char *) &edata;
    if (ioctl(skfd, SIOCETHTOOL, &ifr) == -1) {
        printf("ETHTOOL_GLINK failed: %s\n", strerror(errno));
        return -1;
    }
    return (edata.data ? 1 : 0);
}

/*--------------------------------------------------------------------------*/
/**
  @brief    Detect eth link status from MII status reg.
  @param    skfd   -- socket handler.
  @param    ifname -- interface name.
  @return   Return true if link is up, return false if link is down,
            or return -1 if errors occur.
 */
/*--------------------------------------------------------------------------*/
static int detect_mii(int skfd, char *ifname)
{
    struct ifreq           ifr;
    struct mii_ioctl_data *mii_val;
   
    strncpy(ifr.ifr_name, ifname, IFNAMSIZ);
    if (ioctl(skfd, SIOCGMIIPHY, &ifr) < 0) {
        fprintf(stderr, "SIOCGMIIPHY on %s failed: %s\n", ifname,
                strerror(errno));
        (void) close(skfd);
        return -1;
    }
    mii_val = (struct mii_ioctl_data *)(&ifr.ifr_data);
    mii_val->reg_num = MII_BMSR;    
    if (ioctl(skfd, SIOCGMIIREG, &ifr) < 0) {
        printf("SIOCGMIIREG on %s failed: %s\n", ifr.ifr_name,
               strerror(errno));
        return -1;
    }
    if ((!(mii_val->val_out & BMSR_RFAULT)) &&
          (mii_val->val_out & BMSR_LSTATUS) &&
        (!(mii_val->val_out & BMSR_JCD))) {
        return 1;
    } else {
        return 0;
    }
}

static void getnetname()
{
    FILE *fp = NULL;
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
    snprintf(carrier_path, sizeof(carrier_path), "/sys/class/net/%s/carrier", hw_name);
    g_pLog->WriteLog(0, "net_detect, hw_name: %s, path :%s .", hw_name, carrier_path);
    fclose(fp);
}

static void net_detect()
{
    char buf[512] = {'\0'};
    FILE *fp = NULL;
    fp = fopen(carrier_path, "r");
    if (NULL != fp)
    {
        while(NULL != fgets(buf, sizeof(buf), fp))
        {
            if (buf[0] == '0')
            {
                g_bSetupAmq = false;
            }else
            {
                g_bSetupAmq = true;
            }
        }
    }else
    {
        g_pLog->WriteLog(0, "net_detect, open /sys/class/net/%s/carrier failed.", hw_name);
    }
    fclose(fp);
}

static void* thrd_net_setup(void * param)
{
    while(1)
    {
        int nRet = get_netlink_status("eth0");
        if (nRet == 1)
        {
            g_bSetupAmq = true;
        }
        if (nRet == 0)
        {
            g_bSetupAmq = false;
        }
//        net_detect();
        usleep(1000);
    }
    pthread_detach(pthread_self());
    return NULL;
}

static void wait_net_setup2()
{
    getnetname();
    if ( pthread_create(&g_nettid, NULL, thrd_net_setup, NULL) !=0 )
    {
        g_pLog->WriteLog(0, "zhaosenhua wait_net_setup create thread failed.");
        printf("Create thread error!\n");
    };
}
#endif //__NETLINK__

