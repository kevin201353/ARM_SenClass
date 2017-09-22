#include "activemq.h"
#include "include.h"
#include "testnet.h"
#include "global.h"
#include "myqueue.h"
#include "mqmsgprocess.h"

extern LoginWidget * g_loginWnd;
MyQueue  g_MsgQueue;

extern MqMsgProcess  g_mqMsgProcess;
MyBuffer g_myBuffer;

ActiveMQProduce::ActiveMQProduce()
{
}

ActiveMQProduce::~ActiveMQProduce()
{
    delete bytesMessage;
    cleanup();
}

void ActiveMQProduce::start( const std::string& brokerURI, unsigned int numMessages, const std::string& destURI, bool useTopic, bool clientAck)
{
    this->connection = NULL;
    this->session = NULL;
    this->destination = NULL;
    this->producer = NULL;
    this->numMessages = numMessages;
    this->useTopic = useTopic;
    this->brokerURI = brokerURI;
    this->destURI = destURI;
    this->clientAck = clientAck;
    initialize();
}

void ActiveMQProduce::initialize()
{
    try {
        // Create a ConnectionFactory
        auto_ptr<ActiveMQConnectionFactory> connectionFactory(new ActiveMQConnectionFactory(brokerURI));

        // Create a Connection
        try{
            connection = connectionFactory->createConnection();
            if (NULL != connection)
                connection->start();
        } catch( CMSException& e )
        {
            e.printStackTrace();
            //throw e;
        }

        if (NULL != connection)
        {
            // Create a Session
            if( clientAck ) {
                session = connection->createSession( Session::CLIENT_ACKNOWLEDGE );
            } else {
                session = connection->createSession( Session::AUTO_ACKNOWLEDGE );
            }
            // Create the destination (Topic or Queue)
            if (NULL != session)
            {
                if( useTopic ) {
                    destination = session->createTopic( destURI );
                } else {
                    destination = session->createQueue( destURI );
                }

                // Create a MessageProducer from the Session to the Topic or Queue
                producer = session->createProducer( destination );
                if (NULL != producer)
                    producer->setDeliveryMode( DeliveryMode::NON_PERSISTENT );
            }
        }
    }catch ( CMSException& e )
    {
        QString str(e.getMessage().c_str());
#ifdef _DEBUG_PRINT
        qDebug() << "ActiveMQProduce::initialize createConnection: " + str;
#endif
        e.printStackTrace();
        g_pLog->WriteLog(0,"zhaosenhua amq ActiveMQProduce::initialize onException @@@@ , error: %s\n", e.getMessage().c_str());
    }
}


void ActiveMQProduce::send(const char* Message,int nSize)
{
     if( producer != NULL )
     {
        try {
             if (NULL != session)
             {
                bytesMessage = session->createBytesMessage((unsigned char*)Message,nSize);
             }
         }catch(CMSException& e)
         {
             e.printStackTrace();
             g_pLog->WriteLog(0,"zhaosenhua amq ActiveMQProduce::createBytesMessage onException @@@@ , error: %s\n", e.getMessage().c_str());
         }
        try{
            if (NULL != bytesMessage)
            {
                producer->send(bytesMessage);
            }
        }catch(CMSException& e)
        {
            e.printStackTrace();
            g_pLog->WriteLog(0,"zhaosenhua amq ActiveMQProduce::send message onException @@@@ , error: %s\n", e.getMessage().c_str());
            QString str(e.getMessage().c_str());
            if (str == "ActiveMQProducerKernel - Producer Already Closed")
            {
//                ReportMsg reportmsg;
//                reportmsg.action = USER_MSG_AMQPRODUCE;
//                call_msg_back(msg_respose, reportmsg);
            }
        }
     }
}


void ActiveMQProduce::cleanup()
{
    // Destroy resources.
    try{
        if( destination != NULL )
            delete destination;
    }catch ( CMSException& e )
    {
        e.printStackTrace();
    }
    destination = NULL;

    try{
        if( producer != NULL )
            delete producer;
    }catch ( CMSException& e )
    {
        e.printStackTrace();
    }
    producer = NULL;

    // Close open resources.
    try{
        if( session != NULL )
            session->close();
        if( connection != NULL )
            connection->close();
    }catch ( CMSException& e )
    {
        e.printStackTrace();
    }

    try{
        if( session != NULL )
            delete session;
    }catch ( CMSException& e )
    {
        e.printStackTrace();
    }
    session = NULL;

    try{
        if( connection != NULL )
            delete connection;
    }catch ( CMSException& e )
    {
        e.printStackTrace();
    }
    connection = NULL;
}



//////////////////////////////////////////////////////////////////////////////////////

ActiveMQConsumer::ActiveMQConsumer()
{

}
ActiveMQConsumer::~ActiveMQConsumer()
{
    cleanup();
}

void ActiveMQConsumer::close()
{
    cleanup();
}

void ActiveMQConsumer::runConsumer()
{
    try {
        // Create a ConnectionFactory
        ActiveMQConnectionFactory* connectionFactory = new ActiveMQConnectionFactory(brokerURI);

        if (NULL == connectionFactory)
            return;
        // Create a Connection
        connection = connectionFactory->createConnection();
        //delete connectionFactory;
        if (NULL != connection)
        {
            connection->start();
            connection->setExceptionListener(this);
            // Create a Session
            if( clientAck )
            {
                session = connection->createSession( Session::CLIENT_ACKNOWLEDGE );
            }
            else
            {
                session = connection->createSession( Session::AUTO_ACKNOWLEDGE );
            }
            if (NULL != session)
            {
                if( useTopic )
                {
                    destination = session->createTopic( destURI );
                }
                else
                {
                    destination = session->createQueue( destURI );
                }
                // Create a MessageConsumer from the Session to the Topic or Queue
                consumer = session->createConsumer( destination );
                producer = session->createProducer(destination);
                consumer->setMessageListener( this );
            }
        }
    } catch (CMSException& e)
    {
        QString str(e.getMessage().c_str());
        //qDebug() << "runConsumer: " + str;
        e.printStackTrace();
        if (str == "Network is unreachable")
        {
//            ReportMsg reportmsg;
//            reportmsg.action = USER_AMQ_RESET;
//            call_msg_back(msg_respose, reportmsg);
        }
    }
}

// Called from the consumer since this class is a registered MessageListener.
void ActiveMQConsumer::onMessage( const Message* message )
{
    static int count = 0;
    try
    {
        count++;
        const BytesMessage* bytesMessage = dynamic_cast< const BytesMessage* >( message );
        string text;
        if( bytesMessage != NULL )
        {
            text.assign((char *)(bytesMessage->getBodyBytes()),(bytesMessage->getBodyLength()));
            g_pLog->WriteLog(0,"zhaosenhua amq received Message data len : %d.\n", bytesMessage->getBodyLength());
        }
        else
        {
            text = "NOT A BYTESMESSAGE!";
        }

        if( clientAck )
        {
            char MessageBuf[1024];
            memset(MessageBuf,0,1024);
            sprintf(MessageBuf,"###ap_confirmclassstatus###{\"data\":{\"id\":\"%s\"}}",g_strTerminalID);

            BytesMessage *bytesMessage;
            bytesMessage = session->createBytesMessage((unsigned char*)MessageBuf,strlen(MessageBuf));
            // 发送消息
            // printf( "Sent message  from thread %s\n", threadIdStr.c_str() );
            producer->send(bytesMessage);
            g_pLog->WriteLog(0,"zsh send message: %s\n", MessageBuf);
           // message->acknowledge();
        }
        g_pLog->WriteLog(0,"Message #%d Received: %s\n", count, text.c_str());
        /*
        QueMsg msg;
        strcpy(msg.Data, text.c_str());
        g_MsgQueue.AddQueMsg(msg);
        */
        g_myBuffer.add((char *)text.c_str());
		
        /*
        const TextMessage *txtMsg = dynamic_cast<const TextMessage*>(message);
        if (txtMsg != NULL) {
            std::string body = txtMsg->getText();
            char sztmp[1024] = {0};
            sprintf(sztmp, "Message #%d Received: %s\n", count, body.c_str());
            qDebug() << QString(sztmp);
            QueMsg msg;
            strcpy(msg.Data, body.c_str());
            g_MsgQueue.AddQueMsg(msg);
        }*/
    }
    catch (CMSException& e)
    {
        e.printStackTrace();
        g_pLog->WriteLog(0,"zhaosenhua amq received message exception, error: %s\n", e.getMessage().c_str());
    }
}

void ActiveMQConsumer::onException( const CMSException& ex AMQCPP_UNUSED )
{
    //printf("CMS Exception occurred.  Shutting down client.\n");
    g_pLog->WriteLog(0,"zhaosenhua amq received message onException @@@@ , error: %s\n", ex.getMessage().c_str());
}

void ActiveMQConsumer::transportInterrupted()
{
    std::cout << "The Connection's Transport has been Interrupted." << std::endl;
    g_pLog->WriteLog(0,"zhaosenhua amq received message, The Connection's Transport has been Interrupted. \n");
}

void ActiveMQConsumer::transportResumed()
{
    std::cout << "The Connection's Transport has been Restored." << std::endl;
    g_pLog->WriteLog(0,"zhaosenhua amq received message, The Connection's Transport has been Restored.\n");
}

void ActiveMQConsumer::cleanup(){

    //*************************************************
    // Always close destination, consumers and producers before
    // you destroy their sessions and connection.
    //*************************************************

    // Destroy resources.
    try{
        if( destination != NULL )
            delete destination;
    }catch (CMSException& e)
    {

    }
    destination = NULL;

    try{
        if( consumer != NULL )
            delete consumer;
    }catch (CMSException& e)
    {

    }
    consumer = NULL;

    // Close open resources.
    try{
        if( session != NULL )
            session->close();
        if( connection != NULL )
            connection->close();
    }catch (CMSException& e)
    {

    }

    // Now Destroy them
    try{
        if( session != NULL )
            delete session;
    }catch (CMSException& e)
    {

    }
    session = NULL;

    try{
        if( connection != NULL )
            delete connection;
    }catch (CMSException& e)
    {

    }
    connection = NULL;
}

void ActiveMQConsumer::mycleanup()
{
}

void ActiveMQConsumer::start( const std::string& brokerURI,const std::string& destURI,bool useTopic = false,bool clientAck = false )
{
    this->connection = NULL;
    this->session = NULL;
    this->destination = NULL;
    this->consumer = NULL;
    this->useTopic = useTopic;
    this->brokerURI = brokerURI;
    this->destURI = destURI;
    this->clientAck = clientAck;
}

/********************************************************
 *
 * *********************************************************/
static pthread_t g_mqpid = NULL;
MyMqConsumer::MyMqConsumer()
{
    m_stop = false;
    m_pconsumer = NULL;
    m_pconnect = NULL;
    m_psession = NULL;
    m_pdest = NULL;
}

MyMqConsumer::~MyMqConsumer()
{
    m_stop = true;
    m_pconnect->close();
    m_pconsumer = NULL;
    m_pconnect = NULL;
    m_psession = NULL;
    m_pdest = NULL;
}

void MyMqConsumer::init(const std::string& brokerURI, const std::string& destURI)
{
    ActiveMQConnectionFactory factory;
    g_pLog->WriteLog(0,"ThrdMqMessage, init brokerURI:%s, destURI:%s.\n", brokerURI.c_str(), destURI.c_str());
    factory.setBrokerURI(brokerURI);
    m_pconnect = factory.createConnection();
    m_psession = m_pconnect->createSession(Session::AUTO_ACKNOWLEDGE);
    m_pdest = m_psession->createQueue(destURI);
    m_pconsumer = m_psession->createConsumer(m_pdest);
    m_pconnect->start();
}

void *ThrdMqMessage(void *param)
{
    MyMqConsumer* pMq = (MyMqConsumer*)param;
    if (NULL == pMq)
        return NULL;
    while(!pMq->m_stop)
    {
        std::auto_ptr<Message> message(pMq->m_pconsumer->receive(1000*10));
        const BytesMessage* bytesMessage = dynamic_cast< const BytesMessage* >(message.get());
        if( bytesMessage != NULL )
        {
            string text;
            text.assign((char *)(bytesMessage->getBodyBytes()),(bytesMessage->getBodyLength()));
            g_pLog->WriteLog(0,"message len: %d, Received: %s\n", bytesMessage->getBodyLength(), text.c_str());
            g_myBuffer.add((char *)text.c_str());
        }
        usleep(1000);
    }
    pthread_detach(pthread_self());
    return NULL;
}

void MyMqConsumer::start()
{
    if(pthread_create(&g_mqpid,NULL,ThrdMqMessage,this))
    {
        g_pLog->WriteLog(0,"MyMqConsumer::start(), pthread_create failed. \n");
    }
}


