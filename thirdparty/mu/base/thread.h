/**
 * @file thread.h
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

 * @brief 线程的封装
 * @author lee, china2011long@163.com
 * @version 1.0.0
 * @date 2017-01-17
 */
#ifndef _BASE_THREAD_H_
#define _BASE_THREAD_H_

#include <pthread.h>
#include <list>
using namespace std;

#include "task_queue_i.h"

namespace mu
{

class Thread
{
    /**
     * @brief ThreadFunc 执行任务
     *
     * @param _p 对象指针，由此功能释放
     *
     * @return 
     */
    static void* ThreadFunc(void* _p);
public:
    /**
     * @brief CreateThread 创建线程
     *
     * @param _func
     * @param num
     *
     * @return 
     */
    int CreateThread(Task _func, int num =1);
    /**
     * @brief Join 等待所有线程
     *
     * @return 
     */
    int Join();

private:
    list<pthread_t> m_threadList;
    
};
}; // namespace mu
#endif
