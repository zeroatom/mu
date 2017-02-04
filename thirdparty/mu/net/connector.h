/**
 * @file connector.h
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

 * @brief 连接器
 * @author lee, china2011long@163.com
 * @version 1.0.0
 * @date 2017-02-04
 */

#ifndef _NET_CONNECTOR_H_
#define _NET_CONNECTOR_H_

#include <assert.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>

#include "socket_impl.h"
#include "msg_handler_i.h"
#include "common_socket_controller.h"
#include "thirdparty/mu/base/string_utility.h"

namespace mu
{

class Connector
{
public:
    /**
     * @brief Connect 连接到远端
     *
     * @param _host
     * @param _e
     * @param _msgHandler
     * @param _tq
     *
     * @return 
     */
    static SocketPtr Connect(const string& _host, EpollI* _e, MsgHandlerI* _msgHandler, TaskQueueI* _tq)
    {
        SocketPtr ret = NULL;
        // tcp://127.0.0.1:1001
        vector<string> vt;
        StringUtility::Split(_host, vt, "://");
        if(vt.size() != 2)
        {
            return NULL;
        }

        vector<string> vt2;
        StringUtility::Split(vt[1], vt2, ":");
        if(vt2.size() != 2)
        {
            return NULL;
        }
        if(vt2[0] == "*")
        {
            vt2[0] = "127.0.0.1";
        }

        int s;
        struct sockaddr_in addr;

        if((s = socket(AF_INET,SOCK_STREAM,0)) < 0)
        {
            perror("socket");
            return ret;
        }

        bzero(&addr,sizeof(addr));
        addr.sin_family      = AF_INET;
        addr.sin_port        = htons(atoi(vt2[1].c_str()));
        addr.sin_addr.s_addr = inet_addr(vt2[0].c_str());
        if(::connect(s, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        {
            perror("connect");
            return ret;
        }

        ret = new SocketImpl(_e, new CommonSocketController(_msgHandler), s, _tq);
        ret->Open();
        return ret;
    }
};
}; // namespace mu
#endif
