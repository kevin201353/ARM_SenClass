#include "listbuf.h"
#include "global.h"

ListBuf::ListBuf()
{
    pthread_mutex_init(&m_Locak,NULL);
    m_pBufPointer = NULL;
    m_iHead = 0;
    m_iTail = 0;
    m_iBufNum = 0;
    m_iBufSize = 0;
    m_cInitFlag = 0;
    m_cMode = 0;
}

ListBuf::~ListBuf()
{
    if(m_pBufPointer)
        free(m_pBufPointer);
    m_cInitFlag = 0;
    m_pBufPointer = NULL;
    pthread_mutex_destroy(&m_Locak);
}

/*************************************************
 * bufsize:Node size
 * bufnum:Node Num
 * **********************************************/
int ListBuf::InitBuf(int bufsize,int bufnum,char mode)
{
    if(bufsize <= 0 || bufnum <= 0)
    {
#ifdef _DEBUG_PRINT
        printf("Init Buf Error[bufsize = %d][bufnum = %d]\n",bufsize,bufnum);
#endif
        return -1;
    }
    m_iBufNum = bufnum;
    m_iBufSize = bufsize;
    m_cMode = mode;
    if(!m_pBufPointer)
    {
        m_pBufPointer = (char *)malloc(sizeof(char)*m_iBufSize*m_iBufNum);
        if(m_pBufPointer == NULL)
        {
#ifdef _DEBUG_PRINT
            printf("Malloc m_pBufPointer Error!\n");
#endif
            return -3;
        }
        else
        {
            memset(m_pBufPointer,0,sizeof(char)*m_iBufSize*m_iBufNum);
            m_cInitFlag = 1;
        }
    }
    else
    {
#ifdef _DEBUG_PRINT
        printf("m_pBufPointer is Not NULL,InitBuf Error\n");
#endif
        return -2;
    }
    return 0;
}
int ListBuf::PutOne(const char *buf)
{
    int iTemp = 0;
    iTemp = m_iTail;
    m_iTail = (m_iTail+1)%m_iBufNum;
#ifdef _DEBUG_PRINT
    printf("Tail=%d\n",m_iTail);
#endif
    if(m_iTail == m_iHead)     //Buf is FULL
    {
#ifdef _DEBUG_PRINT
        printf("m_iTail == m_iHead");
#endif
        if(m_cMode == COVERMODE)
        {
            m_iHead = (m_iHead+1)%m_iBufNum;
            memcpy(m_pBufPointer+(m_iBufSize*iTemp),buf,m_iBufSize);
        }else if(m_cMode == EXPANDMODE)
        {
            m_iTail = iTemp;
            return -1;
        }
    }
    else
    {
        memcpy(m_pBufPointer+(m_iBufSize*iTemp),buf,m_iBufSize);
    }
    return 0;
}
int ListBuf::Put(const char *buf, int nNum)
{
    int i = 0;
    if(m_cInitFlag == 0)
    {
        //printf("Not Init\n");
        return -1;
    }
    if(buf == NULL)
    {
        //printf("Push NULL\n");
        return -2;
    }
    pthread_mutex_lock(&m_Locak);
   for(i = 0;i < nNum;i++)
   {
        if(PutOne(buf+(i*m_iBufSize))<0)
        {
#ifdef _DEBUG_PRINT
            printf("Put Data[%d] Error!\n",i);
#endif
            pthread_mutex_unlock(&m_Locak);
            return -3;
        }
   }
   pthread_mutex_unlock(&m_Locak);
   return i;
}
int ListBuf::GetOne(char *buf)
{
    int iTemp = 0;
    if(m_iHead == m_iTail)
    {
//        printf("No Data To Get!\n");
        return -1;
    }
    else
    {
        iTemp = m_iHead;
        m_iHead = (m_iHead+1)%m_iBufNum;
        memcpy(buf,m_pBufPointer+(iTemp*m_iBufSize),m_iBufSize);
    }
    return 0;
}
int ListBuf::Get(char *buf, int nNum)
{
    int i = 0;
    char *pTemp = buf;
    if(m_cInitFlag == 0)
    {
        //printf("Not Init\n");
        return -1;
    }
    if(buf == NULL)
    {
        //printf("Get Buf is NULL\n");
        return -2;
    }
    pthread_mutex_lock(&m_Locak);
    for(i = 0;i < nNum;i++)
    {
        if(GetOne(pTemp+(i*m_iBufSize)) <0)
        {
 //           printf("Get Data[%d] Error!\n",i);
            pthread_mutex_unlock(&m_Locak);
            return -3;
        }
    }
    pthread_mutex_unlock(&m_Locak);
    return i;
}
