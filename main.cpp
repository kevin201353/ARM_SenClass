#include "loginwidget.h"
#include <QApplication>
#include "ryanConfig.h"
#include "include.h"
#include "waitstu2.h"
#include "stwidget.h"
#include <QFontDatabase>
#include <QFont>
#include <QString>
#include "netlink.h"
static pthread_t g_heartid = NULL;
extern volatile bool g_bSetupAmq;
extern void *WhileFun(void *param);
void MqInit();
void shenProc();
static void *HeartNetThread(void *param);

volatile bool   g_bgetserTime = false;
QString loadFontFromFile(QString path)
{
    static QString font;
    static bool loaded = false;
    if(!loaded)
    {
        loaded = true;
        int loadedFontID = QFontDatabase::addApplicationFont(path);
        QStringList loadedFontFamilies = QFontDatabase::applicationFontFamilies(loadedFontID);
        if(!loadedFontFamilies.empty())
            font = loadedFontFamilies.at(0);
    }
    return font;
}

int main(int argc, char *argv[])
{
    int iRecode = 0;
    while((iRecode = getopt(argc,argv,"vV")) != EOF)
    {

        switch (iRecode) {
            case 'v':
            case 'V':
            {
                printf(VISION_VISON,__DATE__,__TIME__);
                return 0;
            }
        }
    }
    shenProc();
    QApplication a(argc, argv);
    waitstu2    waitstu;
    waitstu.showFullScreen();
    LoginWidget w;
    w.showFullScreen();
    w.hide();
    QString fontName = loadFontFromFile("/usr/local/share/fonts/msyh.ttc");
	QFont font(fontName);
	QApplication::setFont(font);
    return a.exec();
}

void initConfig()
{
    NetConfig config;
    config.GetMacAdd(g_strTerminalID,false);
    strcat(g_strConsumerQueue, "edu_");
    strcat(g_strConsumerQueue, g_strTerminalID);
    GetConfigString(CONFIGNAME,"MQ","ProduceQueue","edu_Queue",g_strProduceQueue,50);
    GetConfigString(CONFIGNAME,"ROOM","ServiceIP","192.168.120.36",g_strServerIP,25);
#ifdef _DEBUG_PRINT
    qDebug("Service:%s",g_strServerIP);
#endif
    strcpy(g_strConsumerAdd,"failover:(tcp://");
    strcat(g_strConsumerAdd,g_strServerIP);
    strcat(g_strConsumerAdd,":61616)?initialReconnectDelay=2000");
    //strcat(g_strConsumerAdd, ")?connectionTimeout=5000&timeout=5000");
    strcpy(g_strProduceAdd, g_strConsumerAdd);
#ifdef _DEBUG_PRINT
    qDebug("ActiveMQ Server:%s Recv Queue:%s Send Queue:%s",g_strConsumerAdd, g_strConsumerQueue,g_strProduceQueue);
    qDebug(g_strProduceAdd);
#endif
    GetConfigString(CONFIGNAME,"ROOM","ClassName","Default",g_strRoomNum,100);
    GetConfigString(CONFIGNAME,"ROOM","SeatName","A1",g_strSeatNum,20);
#ifdef _DEBUG_PRINT
    qDebug("%s %s",g_strRoomNum, g_strSeatNum);
#endif
}

static void *HeartNetThread(void *param)
{
    int count = 0;
    while(true)
    {
        if (g_bSetupAmq)
        {
           //int ret = ping_net(g_strServerIP);
           //if (ret == 1)
           {
               if (count >= 6000)
               {
                   break;
               }
               count++;
           }
        }
        usleep(1000);
    }
    g_pLog->WriteLog(0,"HeartNetThread mqinit()");
    MqInit();
    system("/opt/spice/bin/start-pa.sh");
    g_bgetserTime = true;
    pthread_detach(pthread_self());
    return NULL;
}

void shenProc()
{
    g_bgetserTime = false;
    if(g_pLog == NULL)
    {
        g_pLog = new Log();
        g_pLog->InitLog();
    }
    initConfig();
#if 1
    wait_net_setup2();
#endif
#if 1
    if(pthread_create(&g_heartid,NULL,HeartNetThread,NULL))
    {
    }
#endif
}
void MqInit()
{
#if 1
    activemq::library::ActiveMQCPP::initializeLibrary();
    g_Pconsume = NULL;
    g_Pproduce = NULL;
    if(g_Pconsume == NULL)
    {
        g_Pconsume = new ActiveMQConsumer();
        g_Pconsume->start(g_strConsumerAdd,g_strConsumerQueue,false,false);
    }
    if(g_Pproduce == NULL)
    {
        g_Pproduce = new ActiveMQProduce();
    }
    if(g_Pconsume != NULL)
    {
        g_Pconsume->runConsumer();
    }
    if (g_Pproduce != NULL)
    {
        g_Pproduce->start(g_strProduceAdd,20,g_strProduceQueue,false,false);
    }
    g_pLog->WriteLog(0, "thread MQ InitThread, ConsumerAdd=%s, ConsumerQueue=%s,ProduceAdd=%s,ProduceQueue=%s.",g_strConsumerAdd,g_strConsumerQueue,g_strProduceAdd,g_strProduceQueue);
#endif
}
