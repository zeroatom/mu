#include <errno.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "socket_impl.h"
#include "socket_op.h"
#include "socket_controller_i.h"
#include "epoll_i.h"
#include "thirdparty/mu/base/task_queue_i.h"

#include <iostream>
using namespace std;

using namespace mu;


SocketImpl::SocketImpl(EpollI* _e, SocketControllerI* _sc, int _fd, TaskQueueI* _tq):
    m_epoll(_e),
    m_sc(_sc),
    m_fd(_fd),
    m_tq(_tq)
{
}

SocketImpl::~SocketImpl()
{
    delete m_sc;
}

void SocketImpl::Open()
{
    SocketOp::SetNonBlock(m_fd);
    m_sc->HandleOpen(this);
    AsyncRecv();
}

void SocketImpl::Close()
{
    m_tq->Produce(TaskBinder::Gen(&SocketImpl::CloseImpl,this));
}

void SocketImpl::CloseImpl()
{
    if(m_fd > 0)
    {
        m_epoll->UnRegisterFd(this);
        ::close(m_fd);
        m_fd = -1;
    }
}

int SocketImpl::HandleEpollRead()
{
    m_tq->Produce(TaskBinder::Gen(&SocketImpl::HandleEpollReadImpl,this));
    return 0;
}

int SocketImpl::HandleEpollReadImpl()
{
    if(IsOpen())
    {
        int nread = 0;
        char recv_buffer[RECV_BUFFER_SIZE];
        do
        {
            nread = ::read(m_fd, recv_buffer, sizeof(recv_buffer) - 1);
            if(nread > 0)
            {
                recv_buffer[nread] = '\0';
                m_sc->HandleRead(this, recv_buffer, size_t(nread));
                if (nread < int(sizeof(recv_buffer) - 1))
                {
                    break;
                }
            }
            else if (0 == nread)
            {
                this->Close();
                return -1;
            }
            else
            {
                if (errno == EINTR)
                {
                    continue;
                }
                else if (errno == EWOULDBLOCK)
                {
                    break;
                }
                else
                {
                    this->Close();
                    return -1;
                }
            }
        }while(1);
    }
    return 0;
}

int SocketImpl::HandleEpollDel()
{
    m_tq->Produce(TaskBinder::Gen(&SocketControllerI::HandleError, this->GetSc(), this));
    return 0;
}

int SocketImpl::HandleEpollWrite()
{
    m_tq->Produce(TaskBinder::Gen(&SocketImpl::HandleEpollWriteImpl, this));
    return 0;
}

int SocketImpl::HandleEpollWriteImpl()
{
    int ret = 0;
    string left_buff;

    if (false == IsOpen() || true == m_sendBuffer.empty())
    {
        return 0;
    }

    do
    {
        const string& msg = m_sendBuffer.front();
        ret = DoSend(msg, left_buff);
        
        if(ret < 0)
        {
            this->Close();
            return -1;
        }
        else if(ret > 0)
        {
            m_sendBuffer.pop_front();
            m_sendBuffer.push_front(left_buff);
            return 0;
        }
        else
        {
            m_sendBuffer.pop_front();
        }
    }while(false == m_sendBuffer.empty());

    m_sc->HandleWriteCompleted(this);
    return 0;
}

void SocketImpl::AsyncSend(const string& _msg)
{
    m_tq->Produce(TaskBinder::Gen(&SocketImpl::SendImpl, this, _msg));
}

void SocketImpl::SendImpl(const string& _srcBuff)
{
    string buff = _srcBuff;

    if(false == IsOpen() || m_sc->CheckPreSend(this,buff))
    {
        return;
    }

    if(false == m_sendBuffer.empty())
    {
        m_sendBuffer.push_back(buff);
        return;
    }

    string left_buff;

    int ret = DoSend(buff, left_buff);

    if(ret < 0 )
    {
        this->Close();
    }
    else if(ret > 0)
    {
        m_sendBuffer.push_back(left_buff);
    }
    else
    {
        m_sc->HandleWriteCompleted(this);
    }
}

int SocketImpl::DoSend(const string& _buff, string& left_buff_)
{
    size_t nleft = _buff.size();
    const char* buffer = _buff.data();
    ssize_t nwritten;

    while(nleft > 0)
    {
        if((nwritten = ::send(m_fd, buffer, nleft, MSG_NOSIGNAL)) <= 0)
        {
            if(EINTR == errno)
            {
                nwritten = 0;
            }
            else if (EWOULDBLOCK == errno)
            {
                left_buff_.assign(_buff.c_str() + (_buff.size() - nleft), nleft);
                return 1;
            }
            else
            {
                this->Close();
                return -1;
            }
        }

        nleft -= nwritten;
        buffer += nwritten;
    }
    return 0;
}

void SocketImpl::AsyncRecv()
{
    m_epoll->RegisterFd(this);
}

void SocketImpl::SafeDelete()
{
    struct lambda
    {
        static void Exe(void* _p)
        {
            delete ((SocketImpl*)_p);
        }
    };

    m_tq->Produce(Task(&lambda::Exe, this));
}
