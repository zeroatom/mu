/**
 * @file msg_sender.h
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

 * @brief 消息发送器
 * @author lee, china2011long@163.com
 * @version 1.0.0
 * @date 2017-02-04
 */

#ifndef _NET_MSG_SENDER_H_
#define _NET_MSG_SENDER_H_

#include "message.h"
#include "socket_i.h"

namespace mu
{

class MsgSender
{
public:
    /**
     * @brief Send 发送消息
     *
     * @param _socketPtr
     * @param _cmd
     * @param _str
     */
    static void Send(SocketPtr _socketPtr, uint16_t _cmd, const string& _str)
    {
        if(_socketPtr)
        {
            MessageHead h(_cmd);
            h.size = _str.size();
            h.hton();
            string dest((const char*)&h, sizeof(h));
            dest += _str;
            _socketPtr->AsyncSend(dest);
        }
    }
};

}; // namespace mu





#endif
