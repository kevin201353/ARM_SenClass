#ifndef __INTERFACE_H_
#define __INTERFACE_H_

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdarg.h>
#include <errno.h>
#include <time.h>
#include <netdb.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include <openssl/ssl.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
//#include <net/if.h>
#include <net/route.h>
#include <dirent.h>

#define INT8        char
#define UIN8        unsigned char
#define INT32       int
#define UINT32      unsigned int
#define BOOL        int
#define TRUE        1
#define FALSE       0


#define LOGSIZE_MAX     516
#define ARGVSIZE_MAX    200 //32

#define RDPEXE_PATH        "/home/pengjian/freeRDP/standFreeRDP/FreeRDP-stable-1.1/client/X11/xfreerdp"
#define VMWAREEXE_PATH      "/usr/bin/vmware-view"
#define FIREFOX_PATH			"/usr/bin/firefox"
#define SPICY_PATH          "/usr/bin/spicy"

#define LOGNAME              "Interface.log"
#define LOGBACKNAME          "Interface.log.bak"
#define LOGSIZE         10*1024*1024           // byte

//#define WAIT_PID
/*******************************
 *
 * ***************************/
typedef struct
{
    INT8 strUsrName[100];         //User name
    INT8 strPasswd[64];             //Passwd
    INT8 strIP[16];                 // IP
    INT8 strExeName[64];            //ExeName
    INT8 *Argv[ARGVSIZE_MAX];
}RDP;
typedef struct
{
    INT8 strUsrName[100];         //User name
    INT8 strPasswd[64];             //Passwd
    INT8 strIP[16];                 // IP
    INT8 strExeName[64];            //ExeName
    INT8 strDeskTop[64];            //DeskTop Name
    INT8 *Argv[ARGVSIZE_MAX];
}VMware;
typedef struct 
{
    INT8 strDomain[100];                 // XenDeskTop IP
    INT8 strExeName[64];            //ExeName
    INT8 *Argv[ARGVSIZE_MAX];
}Citrix;
typedef struct
{
    INT8 strIP[20];
    UINT32 iPort;
    INT8 strTicket[100];
    INT8 strExeName[64];
    INT8 *Argv[ARGVSIZE_MAX];
}Spicy;
typedef struct
{
    RDP     sRDP;
    VMware  sVMware;
    Citrix	sCitrix;
    Spicy   sSpicy;
    INT32 iLogFlag;                 //Log switch
    INT32 fd;                       //Log fd
}param;

struct rdp_tls
{
    INT8    *strIP;
    UINT32  iPort;
    SSL *ssl;
};

typedef struct rdp_tls rdpTls;
void SetLogFlag(INT32 flag);

INT32 Run_FreeRDP(const INT8 *name,const INT8 *passwd,const INT8 *IP);
INT32 Run_VmwareView(INT8 *name,INT8 *passwd,INT8 *IP,INT8 *desktop);
INT32 Run_Citrix(const INT8 *IP);
INT32 Run_Spicy(const INT8 *IP,UINT32 iPort,const INT8* strTicket);

INT32 Select(INT32 fd,UINT32 timeOut);

INT32 InitLog(void);
INT32 CloseLog(void);
INT32 WriteLog(const INT8* sFormat, ... );
INT32 GetExeName(const INT8 *exe_path,INT32 Mode);
INT32 TestIP(const INT8 *IP,const INT8 *Port,UINT32 TimeOut);
INT32 SetStaticIP(const INT8 *IFName,const INT8 *IP);
INT32 SetNetMask(const INT8 *IFName,const INT8 *IP);
INT32 SetGetWay(const INT8 *IFName,const INT8 *IP);

INT8 GetNetIP();
#endif
