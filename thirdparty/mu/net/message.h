/**
 * @file message.h
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

 * @brief 消息结构
 * @author lee, china2011long@163.com
 * @version 1.0.0
 * @date 2017-01-17
 */

#ifndef _NET_MESSAGE_H_
#define _NET_MESSAGE_H_

#include <stdint.h>
#include <string>
#include <string.h>
using namespace std;

#include <arpa/inet.h>

namespace mu
{
    /**
     * @brief 消息头
     */
struct MessageHead
{
    MessageHead():
        size(0),
        cmd(0),
        flag(0)
    {}
    explicit MessageHead(uint16_t _cmd):
        size(0),
        cmd(_cmd),
        flag(0)
    {}

    void hton()
    {
        size = htonl(size);
        cmd = htons(cmd);
        flag = htons(flag);
    }

    void ntoh()
    {
        size = ntohl(size);
        cmd = ntohs(cmd);
        flag = ntohs(flag);
    }

    uint32_t size;
    uint16_t cmd;
    uint16_t flag;
};

class Message
{
public:
    Message()
    {
    }

    /**
     * @brief GetCmd 获取消息id
     *
     * @return 
     */
    uint16_t GetCmd() const
    {
        return m_head.cmd;
    }

    /**
     * @brief GetBody 消息内容
     *
     * @return 
     */
    const string& GetBody() const
    {
        return m_body;
    }
    /**
     * @brief Size 消息大小
     *
     * @return 
     */
    size_t Size() const
    {
        return m_head.size;
    }
    /**
     * @brief GetFlag 标识位
     *
     * @return 
     */
    uint16_t GetFlag() const
    {
        return m_head.flag;
    }

    /**
     * @brief AppendHead 添加头
     *
     * @param _index
     * @param _buff
     * @param len
     *
     * @return 添加数据大小
     */
    size_t AppendHead(size_t _index, char* _buff, size_t len)
    {
        size_t willAppend = sizeof(m_head) - _index;
        if(willAppend > len)
        {
            willAppend = len;
            ::memcpy((char*)&m_head + _index, _buff, willAppend);
        }
        else
        {
            ::memcpy((char*)&m_head + _index, _buff, willAppend);
            m_head.ntoh();
        }

        return willAppend;
    }

    /**
     * @brief AppendMsg 添加消息
     *          添加消息大小受头中大小控制
     *
     * @param _buff 字符串地址
     * @param len 大小
     *
     * @return 实际添加大小
     */
    size_t AppendMsg(char* _buff, size_t len)
    {
        size_t willAppend = m_head.size - m_body.size();
        if(willAppend > len)
        {
            willAppend = len;
        }
        m_body.append(_buff, willAppend);
        return willAppend;
    }

    /**
     * @brief Clear 清理
     */
    void Clear()
    {
        ::memset(&m_head, 0, sizeof(m_head));
        m_body.clear();
    }

    /**
     * @brief AppendToBody 添加到消息体中
     *
     * @param _buff 消息地址
     * @param len 消息大小
     */
    void AppendToBody(const char* _buff, size_t len)
    {
        m_body.append(_buff, len);
    }

    /**
     * @brief HaveRecvHead 是否包含头
     *
     * @param _haveRecvSize 已接受大小
     *
     * @return true- 包含头 false- 不包含
     */
    bool HaveRecvHead(size_t _haveRecvSize)
    {
        return _haveRecvSize >= sizeof(MessageHead);
    }
private:
    MessageHead m_head;    
    string m_body;
};
}; // namespace mu
#endif
