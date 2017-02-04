/**
 * @file acceptor_i.h
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

 * @brief 端口接收器
 * @author lee, china2011long@163.com
 * @version 1.0.0
 * @date 2017-02-03
 */

#ifndef _NET_ACCEPTOR_I_H_
#define _NET_ACCEPTOR_I_H_

#include <string>
#include <unistd.h>
using namespace std;

#include "netbase.h"

namespace mu
{

class AcceptorI : public FdI
{
public:
    virtual ~AcceptorI();
    /**
     * @brief Open 打开地址
     *
     * @param _address xx://xx:xx
     *
     * @return 0- 成功 其他- 失败
     */
    virtual int Open(const string& _address) = 0;

    /**
     * @brief HandleEpollWrite 写入
     *
     * @return 
     */
    int HandleEpollWrite();
};
}; // namespace mu
#endif
