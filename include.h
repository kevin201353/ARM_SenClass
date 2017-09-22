#ifndef INCLUDE_H
#define INCLUDE_H

#include "log.h"
//#include "tcp.h"
#include "myjson.h"
#include "process.h"
#include <signal.h>
#include "activemq.h"
#include "myhttp.h"
#include "classnameconfig.h"
#include "interface.h"
#include "ryanConfig.h"
#include "loadingframe.h"
#include <sys/syscall.h>

extern myHttp *g_pMyHttp;

extern Log *g_pLog;
extern char cMainExitFlag;
//extern TcpServer *g_pTcp;
extern myJson *g_pJson;
extern process *g_pProcess;
extern ActiveMQConsumer *g_Pconsume;
extern ActiveMQProduce *g_Pproduce;
extern LoadingFrame *g_LoadingFrame;

extern char g_strProduceAdd[1024];
extern char g_strConsumerAdd[1024];
extern char g_strProduceQueue[50];
extern char g_strConsumerQueue[50];
extern char g_strServerIP[25];
extern char g_strRoomNum[100];
extern char g_strSeatNum[20];
extern char g_strTerminalID[30];
//multi
extern char g_strMultiCmd[20];
extern int  g_nMultiCmd;
extern char g_strMultiVideo[20];
extern int g_nMultiVideo;

extern char g_strMultiAudio[20];
extern int g_nMultiAudio;
extern char g_brodMsg[1024];

extern char g_strMultiStudentV[20];
extern int  g_nMultiStudentVP;

extern char g_strMultiStudentA[20];
extern int  g_nMultiStudentAP;
//multi
int DameonInit(void);
void MainExit(int sig);
void Regist();
extern volatile bool  g_bgetserTime;


#define PIPE_SIGNAL_UP    "1"
#define PIPE_SIGNAL_DOWN  "0"


#define CONFIGNAME  "/usr/local/shencloud/Terminal.con"
#define CONFIGDEMON  "/usr/local/shencloud/Demon.con"

#define VISION_VISON     "                        \n\t\
  _______________________________________________ \n\t\
 |                                               |\n\t\
      Shencloud CloudClassRoom_Student   Module          \n\t\
 |                       (Ver 1.00)              |\n\t\
 |                                               |\n\t\
 |    Build By: GCCH      %s %s  |\n\t\
 |    Copyright (C)   Shencloud   2012-2017      |\n\t\
 |    All Rights Reserved.                       |\n\t\
 |_______________________________________________|\n"

#define VISION_START     "                        \n\t\
  _______________________________________________ \n\t\
 |                                               |\n\t\
 |    Shencloud TerminalServer   Module          |\n\t\
 |                       (Ver 1.00)              |\n\t\
 |          START                                |\n\t\
 |    Build By: GCCH      %s %s  |\n\t\
 |    Copyright (C)   Shencloud   .  2012-2017   |\n\t\
 |    All Rights Reserved.                       |\n\t\
 |_______________________________________________|\n"

#define VISION_END     "                          \n\t\
  _______________________________________________ \n\t\
 |                                               |\n\t\
 |    Shencloud TerminalServer   Module          |\n\t\
 |                       (Ver 1.00)              |\n\t\
 |          END                                  |\n\t\
 |    Build By: GCCH       %s %s  |\n\t\
 |    Copyright (C)   Shencloud      2012-2017   |\n\t\
 |    All Rights Reserved.                       |\n\t\
 |_______________________________________________|\n"


#define HELP     "                          \n\t\
    _______________________________________________ \n\t\
   |                                               |\n\t\
   |    %s HELP             |\n\t\
   |                       (Ver 1.00)              |\n\t\
   |                                               |\n\t\
   |    USAGE:                                     |\n\t\
   |    <程序路径/程序名>  -[v|V|h|H|c|C|p|P|]       |\n\t\
   |    -[v|V] :          程序版本号                 |\n\t\
   |    -[h|H] :          程序帮助                  |\n\t\
   |    -[c|C] :          接收消息队列名称            |\n\t\
   |    -[p|P] :          发送消息队列名称            |\n\t\
   |    -[a|A] :          activeMQ服务器地址         |\n\t\
   |                 tcp://192.168.0.140:61616     |\n\t\
   |                                               |\n\t\
   |    Copyright (C)   Shencloud      2012-2017   |\n\t\
   |    All Rights Reserved.                       |\n\t\
   |_______________________________________________|\n"

#endif // INCLUDE_H
