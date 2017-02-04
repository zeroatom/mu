#include <iostream>
using namespace std;

#include "thirdparty/mu/net/net_factory.h"
#include "thirdparty/mu/net/msg_sender.h"
using namespace mu;

class EchoService : public MsgHandlerI
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
        cout<< "受到消息:"<< _msg.GetBody()<<endl;
        MsgSender::Send(_socket_, 2, "ok");
        return 0;
    }

};


int main(int argc, char* argv[])
{
    EchoService es ;
    NetFactory::Listen("tcp://127.0.0.1:10001",&es);
    sleep(100);
    NetFactory::Stop();
}
