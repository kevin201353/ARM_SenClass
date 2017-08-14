#include "multimsg.h"
#include <QByteArray>
#include "log.h"
#include "ryanConfig.h"
#include "include.h"

#define  MULTI_BIND
extern Log *g_pLog;
char g_strMultiCmd[20] = {"224.5.6.88"};
int  g_nMultiCmd = 11008;

char g_strMultiVideo[20] = {"224.5.6.85"};
int g_nMultiVideo = 11004;

char g_strMultiAudio[20] = {"224.5.6.84"};
int g_nMultiAudio = 11005;

char g_strMultiStudentV[20] = {"224.5.6.87"};
int  g_nMultiStudentVP = 11007;

char g_strMultiStudentA[20] = {"224.5.6.86"};
int  g_nMultiStudentAP = 11006;
char g_brodMsg[1024] = {0};
MultiMsg::MultiMsg()
{
    char szAddr[20] = {0};
    char szPort[20] = {0};
    char szVideoPort[20] = {0};
    char szAudioPort[20] = {0};
    GetConfigString(CONFIGNAME, "MUSERVER", "addr", "224.5.6.88", szAddr, 20);
    GetConfigString(CONFIGNAME, "MUSERVER", "port", "11008", szPort, 20);
    strcpy(g_strMultiCmd, szAddr);
    g_nMultiCmd = atoi(szPort);
    GetConfigString(CONFIGNAME, "VIDEO", "addr", "224.5.6.85", g_strMultiVideo, 20);
    GetConfigString(CONFIGNAME, "VIDEO", "port", "11004", szVideoPort, 20);
    g_nMultiVideo = atoi(szVideoPort);
    GetConfigString(CONFIGNAME, "AUDIO", "addr", "224.5.6.84", g_strMultiAudio, 20);
    GetConfigString(CONFIGNAME, "AUDIO", "port", "11005", szAudioPort, 20);
    g_nMultiAudio = atoi(szAudioPort);
    //student broadcast
    memset(szVideoPort, 0, sizeof(szVideoPort));
    GetConfigString(CONFIGNAME, "SPICY_VIDEO", "addr", "224.5.6.87", g_strMultiStudentV, 20);
    GetConfigString(CONFIGNAME, "SPICY_VIDEO", "port", "11007", szVideoPort, 20);
    g_nMultiStudentVP = atoi(szVideoPort);
    memset(szVideoPort, 0, sizeof(szVideoPort));
    GetConfigString(CONFIGNAME, "SPICY_AUDIO", "addr", "224.5.6.86", g_strMultiStudentA, 20);
    GetConfigString(CONFIGNAME, "SPICY_AUDIO", "port", "11006", szVideoPort, 20);
    g_nMultiStudentAP = atoi(szVideoPort);

    groupAddress = QHostAddress(szAddr);
    udpSocket = new QUdpSocket(this);
    int port = atoi(szPort);
    int nResult = udpSocket->bind(QHostAddress::AnyIPv4, port, QUdpSocket::ShareAddress);
    char szMsg[1024] = {0};
    sprintf(szMsg, "zhaosenhua MultiMsg, udp bind : %d.", nResult);
    g_pLog->WriteLog(0, szMsg);
    if (nResult)
    {
        nResult = udpSocket->joinMulticastGroup(groupAddress);
        sprintf(szMsg, "zhaosenhua MultiMsg, udp joinMulticastGroup: %d.", nResult);
        g_pLog->WriteLog(0, szMsg);
        if (nResult)
        {
            udpSocket->setSocketOption(QAbstractSocket::ReceiveBufferSizeSocketOption, 1024);
            udpSocket->setSocketOption(QAbstractSocket::MulticastLoopbackOption, 0);
            connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
        }
    }
}

void MultiMsg::uinit()
{
    udpSocket->disconnected();
    udpSocket->leaveMulticastGroup(groupAddress);
}

void MultiMsg::startSending()
{

}

void MultiMsg::sendDatagram()
{

}

void MultiMsg::processPendingDatagrams()
{
    char szMsg[1024] = {0};
    while(udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size());
        strcpy(g_brodMsg, datagram.data());
        sprintf(szMsg, "zhaosenhua MultiMsg, recv server cmd data : %s.", datagram.data());
        g_pLog->WriteLog(0, szMsg);
        emit broadcastMsg();
    }
}
