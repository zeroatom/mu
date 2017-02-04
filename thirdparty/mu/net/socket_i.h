/**
 * @file socket_i.h
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

 * @brief socket接口
 * @author lee, china2011long@163.com
 * @version 1.0.0
 * @date 2017-01-17
 */
#ifndef _NET_SOCKET_I_H_
#define _NET_SOCKET_I_H_

#include <string>
using namespace std;

#include "netbase.h"

namespace mu
{

class SocketI : public FdI
{
public:
    SocketI():
        m_data(NULL)
    {
    }
    virtual ~SocketI() {}
    /**
     * @brief Open 打开socket
     */
    virtual void Open() = 0;
    /**
     * @brief AsyncSend 异步发送数据
     *
     * @param _buff
     */
    virtual void AsyncSend(const string& _buff) = 0;
    /**
     * @brief AsyncRecv 异步接受数据
     */
    virtual void AsyncRecv() = 0;
    /**
     * @brief SafeDelete 安全删除
     */
    virtual void SafeDelete()
    {
        delete this;
    }
    /**
     * @brief SetData 设置用户数据
     *
     * @param p
     */
    virtual void SetData(void* p)
    {
        m_data = p;
    }
    template<typename T>
    T* GetData() const
    {
        return (T*)m_data;
    }
private:
    /**
     * @brief 用户绑定数据
     */
    void* m_data;    
};

typedef SocketI* SocketPtr;
}; // namespace mu
#endif
