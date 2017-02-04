/**
 * @file netbase.h
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

 * @brief 网络基础
 * @author lee, china2011long@163.com
 * @version 1.0.0
 * @date 2017-01-17
 */
#ifndef _NET_NETBASE_H_
#define _NET_NETBASE_H_
namespace mu
{

// socket
typedef int Socket;

class FdI
{
public:
    virtual ~FdI(){};

    /**
     * @brief Socket 获取socket
     *
     * @return 
     */
    virtual Socket GetSocket() = 0;
    /**
     * @brief HandleEpollRead 处理读事件
     *
     * @return 
     */
    virtual int HandleEpollRead() = 0;
    /**
     * @brief HandleEpollWrite 处理写事件
     *
     * @return 
     */
    virtual int HandleEpollWrite() = 0;
    /**
     * @brief HandleEpollDel 处理删除事件
     *
     * @return 
     */
    virtual int HandleEpollDel() = 0;
    /**
     * @brief Close 关闭
     */
    virtual void Close() = 0;
    
};

class IoDemultiplexerI
{
public:
    virtual ~IoDemultiplexerI(){};

    /**
     * @brief EventLoop 主循环| 分发事件
     *
     * @return 
     */
    virtual int EventLoop() = 0;
    /**
     * @brief Close 关闭分发器
     *
     * @return 
     */
    virtual int Close() = 0;
    /**
     * @brief RegisterFd 注册fd
     *
     * @param 
     *
     * @return 
     */
    virtual int RegisterFd(FdI *) = 0;
    /**
     * @brief UnRegisterFd 解除fd
     *
     * @param 
     *
     * @return 
     */
    virtual int UnRegisterFd(FdI *) = 0;
    /**
     * @brief ModFd 修改fd
     *
     * @param 
     *
     * @return 
     */
    virtual int ModFd(FdI *) = 0;
};
}; // namespace mu
#endif
