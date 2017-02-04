/**
 * @file acceptor_impl.h
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

 * @brief 通用接受器
 * @author lee, china2011long@163.com
 * @version 1.0.0
 * @date 2017-02-03
 */

#ifndef _NET_ACCEPTOR_IMPL_H_
#define _NET_ACCEPTOR_IMPL_H_

#include "acceptor_i.h"

namespace mu
{

#define LISTEN_BACKLOG 5

class EpollI;
class SocketI;
class MsgHandlerI;
class TaskQueuePool;

class AcceptorImpl : public AcceptorI
{
public:
    AcceptorImpl(EpollI* _epoll, MsgHandlerI* _msgHandler, TaskQueuePool* _tq);
    ~AcceptorImpl();
    /**
     * @brief Open 打开端口
     *
     * @param _address
     *
     * @return 
     */
    int Open(const string& _address);
    /**
     * @brief Close 关闭
     */
    void Close();
    /**
     * @brief Socket 获取监听socket
     *
     * @return 
     */
    Socket GetSocket() {return m_listenFd;};
    /**
     * @brief HandleEpollRead 读事件
     *
     * @return 
     */
    int HandleEpollRead();
    /**
     * @brief HandleEpollDel 删除事件
     *
     * @return 
     */
    int HandleEpollDel();
protected:
    /**
     * @brief CreateSocket 创建socket
     *
     * @param Socket
     *
     * @return 
     */
    virtual SocketI* CreateSocket(Socket);
private:
    /**
     * @brief 监听socket
     */
    Socket m_listenFd;
    /**
     * @brief socket事件分发器
     */
    EpollI* m_epoll;
    /**
     * @brief 消息处理器
     */
    MsgHandlerI* m_msgHandler;
    /**
     * @brief 工作任务池
     */
    TaskQueuePool* m_tq;
};
}; // namespace mu
#endif
