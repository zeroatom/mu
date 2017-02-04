#ifndef _NET_SOCKET_IMPL_H_
#define _NET_SOCKET_IMPL_H_

#include <list>
#include <string>
using namespace std;

#include "socket_i.h"

namespace mu
{

class EpollI;
class SocketControllerI;
class TaskQueueI;

#define  RECV_BUFFER_SIZE 8096

class SocketImpl : public SocketI
{
public:
    /**
     * @brief 发送缓存
     */
    typedef list<string> SendBuffer;
public:
    SocketImpl(EpollI*, SocketControllerI*, int fd, TaskQueueI* _tq);
    ~SocketImpl();

    virtual Socket GetSocket()
    {
        return m_fd;
    }

    /**
     * @brief Close 关闭
     */
    virtual void Close();
    /**
     * @brief Open 打开
     */
    virtual void Open();

    virtual int HandleEpollRead();
    virtual int HandleEpollWrite();
    virtual int HandleEpollDel();
    
    virtual void AsyncSend(const string& _buff);
    virtual void AsyncRecv();
    virtual void SafeDelete();

    int HandleEpollReadImpl();
    int HandleEpollWriteImpl();
    int HandleEpollErrorImpl();
    void SendImpl(const string& _buff);
    void CloseImpl();

    SocketControllerI* GetSc()
    {
        return m_sc;
    }
private:
    bool IsOpen()
    {
        return m_fd > 0;
    }

    int DoSend(const string& msg, string& _left);
private:
    /**
     * @brief 分发器
     */
    EpollI* m_epoll;
    /**
     * @brief socket上层控制器
     */
    SocketControllerI* m_sc;
    /**
     * @brief 底层socket
     */
    int m_fd;
    /**
     * @brief 任务队列
     */
    TaskQueueI* m_tq;
    /**
     * @brief 发送缓冲区
     */
    SendBuffer m_sendBuffer;
};
}; // namespace mu
#endif
