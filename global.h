#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <QWidget>
#include "type.h"

#define USER_WAITINGDLG_SHOW   100
#define USER_WAITINGDLG_EXIT   101
#define USER_WAITINGDLG_RESET  102
#define USER_AMQ_RESET         103
#define USER_MSG_NETDOWN       104
#define USER_MSG_NETDLG_HIDE   105
#define QUE_MSG_LEN 1024  
#define USET_MSG_PROCESSMSG    107

#define USER_MSG_FREESTUDY     108
#define USER_MSG_AMQPRODUCE    109
#define USER_MSG_HIDEWINDOW    110

#define MAX_IP_SIZE            20

#define FIFO_PIPE   "/usr/local/shencloud/student_fifo"
#define SPICY_LOG_PATH  "/usr/local/shencloud/log/Spicy.log"

#define PIPE_SPICYLOG(x) ({ FILE *fp = fopen("/usr/local/shencloud/student_spicy", "a+"); \
    if (fp != NULL){ \
        fwrite(x, 1, strlen(x), fp);\
        fclose(fp); \
    }})


//#define PIPE_WAIT(x) ({ int fd = open(FIFO_PIPE, O_WRONLY); \
//    if (fd != -1){ \
//        write(fd, x, strlen(x)); \
//        close(fd); \
//    }})

struct ReportMsg {
    unsigned int action;
};

struct QueMsg {
    char Data[QUE_MSG_LEN];
};

typedef void (*MsgCallBackFun)(struct ReportMsg msg);
void msg_respose(struct ReportMsg msg);
void call_msg_back(MsgCallBackFun fun, struct ReportMsg msg);
extern void create_msg_queue();
extern void msg_send(char* dataTmp);
extern void msg_queue_del();
extern void msg_recv(char* dataT);
extern int ping_net(char *ip);
#ifdef ARM
extern long long  __GetTime();
#else
extern long __GetTime();
#endif
extern void InitMyMutex();
extern void MyMutex_lock();
extern void MyMutex_unlock();
extern void MyMutex_destroy();
extern bool bMyGetConnectVm();
extern void MySetConnectVm(bool bval);
extern float g_scr_old_width;
extern float g_scr_old_height;
extern bool  g_processThread;
extern int my_resize(QWidget* widget, float factorX, float factorY);
extern QString loadFontFromFile(QString path);
#define PIPE_WAIT(x) ({msg_send(x);})
extern char   g_szLocalIP[MAX_IP_SIZE];
#endif // GLOBAL_H

