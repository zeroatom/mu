#include <errno.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <assert.h>

#include "epoll_impl.h"

#include <iostream>
using namespace std;

using namespace mu;

EpollImpl::EpollImpl():
    m_running(true),
    m_efd(-1)
{
    m_efd = ::epoll_create(CREATE_EPOLL_SIZE);
    m_interuptSockets[0] = -1;
    m_interuptSockets[1] = -1;
    assert( 0 == ::socketpair(AF_LOCAL, SOCK_STREAM, 0, m_interuptSockets) );
    struct epoll_event ee = { 0, { 0  }  };
    ee.data.ptr  = this;
    ee.events    = EPOLLIN | EPOLLPRI | EPOLLOUT | EPOLLHUP | EPOLLET;;
    ::epoll_ctl(m_efd, EPOLL_CTL_ADD, m_interuptSockets[0], &ee);
}

EpollImpl::~EpollImpl()
{
    ::close(m_interuptSockets[0]);
    ::close(m_interuptSockets[1]);
    ::close(m_efd);
    m_efd = -1;
}

int EpollImpl::EventLoop()
{
    int i = 0, nfds = 0;
    struct epoll_event ev_set[EPOLL_EVENTS_SIZE];

    do
    {
        nfds  = ::epoll_wait(m_efd, ev_set, EPOLL_EVENTS_SIZE, EPOLL_WAIT_TIME);

        if (nfds < 0 && EINTR == errno)
        {
            nfds = 0;
            continue;
        }

        for(i =0; i < nfds; ++i)
        {
            epoll_event& cur_ev = ev_set[i];
            FdI* fd_ptr        = (FdI*)cur_ev.data.ptr;
            if (cur_ev.data.ptr == this)
            {
                if(false == m_running)
                {
                    return 0;
                }

                FdDelCallback();
                continue;
            }

            if (cur_ev.events & (EPOLLIN | EPOLLPRI))
            {
                fd_ptr->HandleEpollRead();
            }

            if(cur_ev.events & EPOLLOUT)
            {
                fd_ptr->HandleEpollWrite();
            }
            
            if (cur_ev.events & (EPOLLERR | EPOLLHUP))
            {
                fd_ptr->Close();
            }
        }
    }while(nfds >= 0);
    return 0;
}

int EpollImpl::Close()
{
    m_running = false;
    InteruptLoop();

    return 0;
}

int EpollImpl::RegisterFd(FdI* _fd_ptr)
{
    struct epoll_event ee = { 0, { 0  }  };
    ee.data.ptr  = _fd_ptr;
    ee.events    = EPOLLIN | EPOLLPRI | EPOLLOUT | EPOLLHUP | EPOLLET;

    return ::epoll_ctl(m_efd, EPOLL_CTL_ADD, _fd_ptr->GetSocket(), &ee);
}

int EpollImpl::UnRegisterFd(FdI* _fd_ptr)
{
    int ret = 0;
    if (_fd_ptr->GetSocket() > 0)
    {
        struct epoll_event ee;

        ee.data.ptr  = (void*)0;
        ret = ::epoll_ctl(m_efd, EPOLL_CTL_DEL, _fd_ptr->GetSocket(), &ee);
    }

    {
        LockGuard lock(m_mutex);
        m_errorFdSet.push_back(_fd_ptr);
    }

    InteruptLoop();
    return ret;
}

int EpollImpl::ModFd(FdI* _fd_ptr)
{
    struct epoll_event ee = { 0, { 0  }  };

    ee.data.ptr  = _fd_ptr;
    ee.events    = EPOLLIN | EPOLLPRI | EPOLLOUT | EPOLLHUP | EPOLLET;

    return ::epoll_ctl(m_efd, EPOLL_CTL_MOD, _fd_ptr->GetSocket(), &ee);
}

void EpollImpl::FdDelCallback()
{
    LockGuard lock(m_mutex);
    list<FdI*>::iterator it = m_errorFdSet.begin();
    for(; it != m_errorFdSet.end(); ++it)
    {
        (*it)->HandleEpollDel();
    }
    m_errorFdSet.clear();
}

int EpollImpl::InteruptLoop()
{
    struct epoll_event ee = { 0, { 0  }  };
    ee.data.ptr  = this;
    ee.events    = EPOLLIN | EPOLLPRI | EPOLLOUT | EPOLLHUP | EPOLLET;

    return ::epoll_ctl(m_efd, EPOLL_CTL_MOD, m_interuptSockets[0], &ee);
}
