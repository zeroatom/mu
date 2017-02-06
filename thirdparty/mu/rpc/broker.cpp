#include "broker.h"
#include "../base/log.h"

#include <iostream>
using namespace std;

using namespace mu;

Broker::Broker():
    mAcceptor(NULL)
{
}

Broker::~Broker()
{
}

int Broker::HandleBroken(SocketPtr _sock)
{
    mTq.Produce(TaskBinder::Gen(&Broker::HandleBrokenImpl, this, _sock));
    return 0;
}

int Broker::HandleMsg(const Message& _msg, SocketPtr _sock)
{
    cout<< "收到消息"<<endl;
    mTq.Produce(TaskBinder::Gen(&Broker::HandleMsgImpl, this, _msg, _sock));
    return 0;
}

int Broker::HandleBrokenImpl(SocketPtr _sock)
{
    _sock->SafeDelete();
    LOGTRACE("Broker::HandleBrokenImpl");
    return 0;
}

int Broker::HandleMsgImpl(const Message& _msg, SocketPtr _sock)
{
    LOGTRACE("收到消息 %s", _msg.GetBody().c_str());
    return 0;
}

int Broker::Open(ArgConfig& _opt)
{
    NetFactory::Start(1);
    mListenHost = _opt.Get("-broker","");

    mAcceptor = NetFactory::Listen(mListenHost, this);
    if(NULL == mAcceptor)
    {
        LOGERROR("Broker::Open failed <%s>", mListenHost);
        return -1;
    }

    // 任务队列
    mThread.CreateThread(TaskBinder::Gen(&TaskQueue::Run, &mTq), 1);
    return 0;
}

int Broker::Close()
{
    if(!mAcceptor)
    {
        return 0;
    }
    mAcceptor->Close();
    mAcceptor = NULL;
    mTq.Close();
    mThread.Join();
    return 0;
}

TaskQueue& Broker::GetTq()
{
    return mTq;
}
/*
TimerService& Broker::GetTimer()
{
    return mTimer;
}
*/
