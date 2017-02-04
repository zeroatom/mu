#include <iostream>
using namespace std;

#include "thirdparty/mu/net/net_factory.h"
#include "thirdparty/mu/net/msg_sender.h"
using namespace mu;

class EchoClient : public MsgHandlerI
{
    /**
     * @brief HandleBroken socket断开处理
     *
     * @param _socket_
     *
     * @return 
     */
    virtual int HandleBroken(SocketPtr _socket_)
    {
        _socket_->SafeDelete();
        return 0;
    }

    /**
     * @brief HandleMsg 处理消息
     *
     * @param _msg 消息内容
     * @param _socket_t socket指针
     *
     * @return 
     */
    virtual int HandleMsg(const Message& _msg, SocketPtr _socket_)
    {
        cout<< "收到消息 :" << _msg.GetBody()<<endl;
        return 0;
    }

};


int main(int argc, char* argv[])
{
    EchoClient es ;
    SocketPtr sp = NetFactory::Connect("tcp://127.0.0.1:10001",&es);
    MsgSender::Send(sp, 1, "hello");
    sleep(100);
    NetFactory::Stop();
}
