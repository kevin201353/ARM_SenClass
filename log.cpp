#include "log.h"
#include "include.h"

void *WriteLogThread(void *param);

Log::Log()
{
    m_pListBuf = NULL;
    m_cInitFlag = 0;
    m_pf = NULL;
    m_pid = 0;
}
Log::~Log()
{
    if(m_pListBuf)
    {
        delete m_pListBuf;
    }
    if(m_pf)
    {
        fclose(m_pf);
    }
    m_pListBuf = NULL;
    m_pf = NULL;
    m_cInitFlag = 0;
    m_pid = 0;
}
int Log::InitLog()
{
    int iRecode = -1;
    if(m_cInitFlag)
    {
#ifdef _DEBUG_PRINT
        printf("Log::InitLog Init already!\n");
#endif
       return -1;
    }
    if(m_pListBuf)
    {
#ifdef _DEBUG_PRINT
        printf("m_pListBuf is not NULL\n");
#endif
        return -2;
    }
    m_pListBuf = new ListBuf();
    iRecode = m_pListBuf->InitBuf(sizeof(sMessage),50,EXPANDMODE);
    if(iRecode < 0)
    {
#ifdef _DEBUG_PRINT
        printf("Init Buf Error\n");
#endif
        return -3;
    }
    if(m_pf)
    {
#ifdef _DEBUG_PRINT
        printf("m_pf is not NULL\n");
#endif
        return -4;
    }
    iRecode = OpenLog();
    if(iRecode < 0)
    {
#ifdef _DEBUG_PRINT
        printf("OpenLog File Error!\n");
#endif
        return -5;
    }
    m_cInitFlag = 1;
    if(pthread_create(&m_pid,NULL,WriteLogThread,this))
    {
#ifdef _DEBUG_PRINT
        printf("Create Thread Error!\n");
#endif
        return -6;
    }
    return 0;
}
void *WriteLogThread(void *param)
{
    Log *p = (Log*)param;
    p->ThreadFun();
    return NULL;
}
void Log::ThreadFun()
{
    sMessage WriteInfo;
    int iRecode = 0;
    int iWriteNum = 0;
    long lFilesize = 0;
    while(m_cInitFlag)
    {
        memset(&WriteInfo,0,sizeof(WriteInfo));
        iRecode = m_pListBuf->Get((char*)&WriteInfo,1);
        if(iRecode < 0)
        {
            sleep(1);
            continue;
        }
        if(m_pf)
        {
            fprintf(m_pf,"%s\n",WriteInfo.s_strMessage);
            iRecode = fflush(m_pf);
            if(iRecode != 0)
            {
#ifdef _DEBUG_PRINT
                printf("File fflush Error[%d] [%d]\n",iRecode,errno);
                perror("Error:");
#endif
            }
            iWriteNum++;
        }
        if(!iWriteNum%80)
        {
            lFilesize = ftell(m_pf);
            if(lFilesize >= LOGSIZE)
            {
                char strCmd[1024];
                memset(strCmd,0,1024);
                sprintf(strCmd,"mv %s/%s %s/%s.bak",LOGPATH,LOGFILENAME,LOGPATH,LOGFILENAME);
                system(strCmd);
                fclose(m_pf);
                OpenLog();
            }
            iWriteNum = 0;
        }
        usleep(500);
    }
}

void Log::WriteLog(char Loglevel, const char *Format,...)
{
    sMessage PutMessage;
    char strTime[100];
    char strMessage[MESSAGELEN];
    va_list arg;
    struct tm *pTm;
    time_t Time;
    Time = time(NULL);
    pTm = localtime(&Time);
    memset(strTime,0,100);
    memset(strMessage,0,MESSAGELEN);
    sprintf(strTime,"[%04d-%02d-%02d %02d:%02d:%02d] ",1900+pTm->tm_year, pTm->tm_mon+1, pTm->tm_mday, pTm->tm_hour, pTm->tm_min, pTm->tm_sec);
    va_start(arg,Format);
    vsnprintf(strMessage,MESSAGELEN-2,Format,arg);
    va_end(arg);

    PutMessage.s_cLogLevel = Loglevel;
    PutMessage.s_cType = 0;
    sprintf(PutMessage.s_strMessage,"%s%s",strTime,strMessage);

#ifdef PRINTSCREEN
    printf("%s\n",PutMessage.s_strMessage);
#endif

    if(m_pListBuf)
    {
        m_pListBuf->Put((char*)((void *)&PutMessage),1);
    }
}
int Log::OpenLog()
{
    char strLogfileName[100];
    memset(strLogfileName,0,100);
    if(m_pf)
    {
        fclose(m_pf);
    }
    Mkdir(LOGPATH);
    sprintf(strLogfileName,"%s/%s",LOGPATH,LOGFILENAME);
    m_pf = fopen(strLogfileName,"a");
    if(m_pf == NULL)
    {
#ifdef _DEBUG_PRINT
        printf("fopen Error:%s!\n",strerror(errno));
#endif
        return -1;
    }
    return 0;
}

int Log::Mkdir(const char *path,int mode)
{
    char strTemp[64];
    int ipoint = 0;
    memset(strTemp,0,64);
    int iRecode = 0;
    if(path == NULL || path[1] == 0)
        return -1;
    ipoint = strlen(path)-1;
    strcpy(strTemp,path);
    iRecode = mkdir(path,mode);
    if(iRecode == 0)
    {
        return 0;
    }
    else if(iRecode < 0)
    {
#ifdef _DEBUG_PRINT
        printf("Mkdir Error:%s\n",strerror(errno));
#endif
        if(errno == EEXIST)
            return 0;
        else
        {
            while(strTemp[ipoint] != '/')
            {
                strTemp[ipoint] = 0;
                ipoint--;
            }
            Mkdir(strTemp,mode);
        }
    }
    mkdir(path,mode);
    return 0;
}
