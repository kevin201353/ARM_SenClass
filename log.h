#ifndef LOG_H_
#define LOG_H_

#include "listbuf.h"
#include <error.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdarg.h>
#include "type.h"


#define LOGPATH            "/usr/local/shencloud/log"
#define LOGFILENAME        "Student.log"
#define LOGSIZE            1024*1024*20
#define MESSAGELEN         102400

#ifndef ARM
#define PRINTSCREEN
#endif

typedef struct MessageLog
{
    char s_cType;
    char s_cLogLevel;
    char s_strMessage[MESSAGELEN];
}sMessage;

class Log
{
public:
    Log();
    ~Log();
    int InitLog();
    void ThreadFun();
    void WriteLog(char Loglevel ,const char *Format,...);
private:
    int OpenLog();
    int Mkdir(const char *path, int mode=S_IRWXU | S_IRWXG | S_IRWXO);
    ListBuf *m_pListBuf;
    char m_cInitFlag;
    FILE *m_pf;
    pthread_t m_pid;
};

#endif // LOG_H
