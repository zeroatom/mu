/**
 * @file msg_handler_i.h
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

 * @brief 消息处理接口
 * @author lee, china2011long@163.com
 * @version 1.0.0
 * @date 2017-01-17
 */
#ifndef _NET_MSG_HANDLER_I_H_
#define _NET_MSG_HANDLER_I_H_

#include "message.h"
#include "socket_i.h"

namespace mu
{

class MsgHandlerI
{
public:
    virtual ~MsgHandlerI(){};

    /**
     * @brief HandleBroken socket断开回调
     *
     * @param _sock 断开socket
     *
     * @return 0- 成功 其他- 失败
     */
    virtual int HandleBroken(SocketPtr _sock) = 0;
    /**
     * @brief HandleMsg 处理消息
     *
     * @param _msg 消息内容
     * @param _sock_ 触发socket
     *
     * @return 0- 成功 其他- 失败
     */
    virtual int HandleMsg(const Message& _msg, SocketPtr _sock_) = 0;

private:
    
};

/**
 * @brief 消息处理器指针
 */
typedef MsgHandlerI* MsgHandlerPtr;
}; // namespace mu
#endif
