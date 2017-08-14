#ifndef LISTBUF_H
#define LISTBUF_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXPANDMODE     0
#define COVERMODE      1

class ListBuf
{
public:
    ListBuf();
    ~ListBuf();
    int InitBuf(int bufsize,int bufnum,char mode);
    int PutOne(const char *buf);
    int GetOne(char *buf);
    int Put(const char *buf, int nNum);
    int Get(char *buf,int nNum);
private:
    pthread_mutex_t  m_Locak;
    char             *m_pBufPointer;
    int                 m_iHead;
    int                 m_iTail;
    int                 m_iBufNum;
    int                 m_iBufSize;
    char                m_cInitFlag;
    char                m_cMode;
};

#endif // LISTBUF_H
