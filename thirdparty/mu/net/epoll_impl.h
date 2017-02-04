/**
 * @file epoll_impl.h
 * Copyright (C) 

 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

 * @brief epoll实现
 * @author lee, china2011long@163.com
 * @version 1.0.0
 * @date 2017-01-18
 */
#ifndef _NET_EPOLL_IMPL_H_
#define _NET_EPOLL_IMPL_H_

#include <list>
using namespace std;

#include "epoll_i.h"
#include "thirdparty/mu/base/task_queue_i.h"
#include "thirdparty/mu/base/lock.h"

namespace mu
{
#define CREATE_EPOLL_SIZE  4096
#define EPOLL_EVENTS_SIZE  128
#define EPOLL_WAIT_TIME    100 

class EpollImpl : public EpollI
{
public:
    EpollImpl();
    ~EpollImpl();

    virtual int EventLoop();
    virtual int Close();
    virtual int RegisterFd(FdI*);
    virtual int UnRegisterFd(FdI*);
    virtual int ModFd(FdI*);

    /**
     * @brief InteruptLoop 中断循环
     *
     * @return 
     */
    int InteruptLoop();

protected:
    void FdDelCallback();
private:
    /**
     * @brief 运行标识
     */
    volatile bool m_running;    
    /**
     * @brief 循环fd
     */
    int m_efd;
    /**
     * @brief 任务队列
     */
    TaskQueueI* m_taskQueue;
    /**
     * @brief 中断socket
     */
    int m_interuptSockets[2];
    /**
     * @brief 待销毁 错误 socket
     */
    list<FdI*> m_errorFdSet;
    /**
     * @brief 锁
     */
    Mutex m_mutex;
};

}; // namespace mu
#endif
