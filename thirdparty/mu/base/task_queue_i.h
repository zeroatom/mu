/**
 * @file task_queue_i.h
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

 * @brief 任务接口
 * @author lee, china2011long@163.com
 * @version 1.0.0
 * @date 2017-01-13
 */
#ifndef _TASK_QUEUE_I_H_
#define _TASK_QUEUE_I_H_

#include <list>
#include <vector>
using namespace std;

namespace mu
{
// 任务函数原型
typedef void (*TASK_FUNC)(void*);

// 任务接口
class TaskI
{
public:
    virtual ~TaskI();
    /**
     * @brief Run 运行任务
     */
    virtual void Run() = 0;
    /**
     * @brief 克隆任务
     *
     * @return 
     */
    virtual TaskI* Clone() = 0;
    
};

// 任务实现
class TaskImpl: public TaskI
{
public:
    /**
     * @brief TaskImpl 
     *
     * @param _func 函数指针
     * @param _arg 函数参数
     */
    TaskImpl(TASK_FUNC _func, void* _arg);

    virtual void Run();

    virtual TaskI* Clone();
protected:
    /**
     * @brief 函数指针
     */
    TASK_FUNC m_func;
    /**
     * @brief 参数
     */
    void* m_arg;
};

// 任务
class Task
{
public:
    /**
     * @brief Dumy 空处理函数
     *
     * @param _arg 参数
     */
    static void Dumy(void* _arg);

    /**
     * @brief Task 
     */
    Task();
    /**
     * @brief Task 
     *
     * @param _f 处理函数
     * @param _arg 参数
     */
    Task(TASK_FUNC _f, void* _arg);
    /**
     * @brief Task 
     *
     * @param _src
     */
    Task(const Task& _src);
    /**
     * @brief ~Task 
     */
    ~Task();
    /**
     * @brief operator= 
     *
     * @param _src 任务源
     *
     * @return 
     */
    Task& operator=(const Task& _src);

    /**
     * @brief Run 运行
     */
    void Run();

protected:
    /**
     * @brief 任务实现
     */
    TaskI* m_taskImpl;
};

// 任务绑定器
class TaskBinder
{
public:
    // c系列任务
    /**
     * @brief 无返回值任务
     *
     * @param _func 函数指针
     * @param _p 参数
     *
     * @return 任务对象
     */
    static Task Gen(void (*_func)(void*), void* _p);

    // C++系列任务
};

}; // namespace mu

#endif
