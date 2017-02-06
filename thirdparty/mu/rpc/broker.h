#ifndef _RPC_BROKER_H_
#define _RPC_BROKER_H_

#include <assert.h>
#include <string>
#include <map>
#include <set>
using namespace std;

#include "thirdparty/mu/base/arg_config.h"
#include "thirdparty/mu/net/msg_handler_i.h"
#include "thirdparty/mu/base/thread.h"
#include "thirdparty/mu/base/task_queue_impl.h"
#include "thirdparty/mu/net/net_factory.h"
#include "thirdparty/mu/base/timer_service.h"

namespace mu
{

class Broker : public MsgHandlerI
{
public:
    Broker();
    virtual ~Broker();

    /**
     * @brief HandleBroken 连接断开回调
     *
     * @param _sock
     *
     * @return 
     */
    int HandleBroken(SocketPtr _sock);

    /**
     * @brief HandleMsg 消息到达回调
     *
     * @param _msg
     * @param _sock
     *
     * @return 
     */
    int HandleMsg(const Message& _msg, SocketPtr _sock);

public:
    /**
     * @brief Open 开启
     *
     * @param _opt
     *
     * @return 
     */
    int Open(ArgConfig& _opt);
    /**
     * @brief Close 关闭
     *
     * @return 
     */
    int Close();
    /**
     * @brief GetTq 获取任务队列
     *
     * @return 
     */
    TaskQueue& GetTq();
    /**
     * @brief GetTimer 获取时间服务
     *
     * @return 
     */
//    TimerService& GetTimer();
protected:
    int HandleBrokenImpl(SocketPtr _sock);
    int HandleMsgImpl(const Message& _msg, SocketPtr _sock);
private:
    /**
     * @brief 接收器
     */
    AcceptorI* mAcceptor;
    /**
     * @brief 
     */
    string mListenHost;
    /**
     * @brief 工作队列
     */
    TaskQueue mTq;
    /**
     * @brief 线程管理
     */
    Thread mThread;
    /**
     * @brief 时间服务
     */
//    TimerService mTimer;
};

}; // namespace mu
#endif
