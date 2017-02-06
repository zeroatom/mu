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

#include "config.h"

#include <list>
#include <vector>
#include <iostream>
using namespace std;

namespace mu
{
// 任务函数原型
typedef void (*TASK_FUNC)(void*);

// 任务接口
class TaskI
{
public:
    virtual ~TaskI()
    {
    }
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
     * @param _taskI
     */
    Task(TaskI* _taskI);
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

class TaskQueueI
{
public:
    typedef list<Task> TaskList;
public:
    virtual ~TaskQueueI(){};
    /**
     * @brief Close 关闭任务队列
     */
    virtual void Close() = 0;
    /**
     * @brief Produce 生产任务
     *
     * @param _task
     */
    virtual void Produce(const Task& _task) = 0;
    /**
     * @brief MultiProduce 添加多个任务
     *
     * @param _taskList
     */
    virtual void MultiProduce(const TaskList _taskList) = 0;
    /**
     * @brief Consume 消费任务
     *
     * @param _task
     *
     * @return 0- 成功 其他- 失败
     */
    virtual int Consume(Task& _task) = 0;
    virtual int ConsumeAll(TaskList&) = 0;
    virtual int Run() = 0;
    virtual int BatchRun() = 0;
    
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
    static Task Gen(void (*_func)(void*), void* _p)
    {
        cout << "D1" << endl;
        return Task(_func,_p);
    }

    /**
     * @brief Gen 
     *
     * @param _func
     *
     * @return 
     */
    template<typename RET>
    static Task Gen(RET (*_func)(void))
    {
        struct Lambda
        {
            static void TaskFunc(void* _p)
            {
                (*(RET(*)(void))_p)();
            };
        };
        cout << "D2" << endl;
        return Task(Lambda::TaskFunc, (void*)_func);
    }

    /**
     * @brief Gen 
     *
     * @param _func 函数
     * @param _arg1 参数
     *
     * @return 
     */
    template<typename FUNCT, typename ARG1>
    static Task Gen(FUNCT _func, ARG1 _arg1)
    {
        struct Lambda : public TaskI
        {
            FUNCT destFunc;
            ARG1 arg1;
            Lambda(FUNCT _func, const ARG1& _arg1):
                destFunc(_func),
                arg1(_arg1)
            {
            }
            virtual void Run()
            {
                (*destFunc)(arg1);
            }

            virtual TaskI* Clone()
            {
                return new Lambda(destFunc,arg1);
            }
        };
        return Task(new Lambda(_func, _arg1));
    }

    /**
     * @brief Gen 
     *
     * @param _func 函数
     * @param _arg1 参数
     * @param _arg2
     *
     * @return 
     */
    template<typename FUNCT, typename ARG1,typename ARG2>
    static Task Gen(FUNCT _func, ARG1 _arg1, ARG2 _arg2)
    {
        struct Lambda : public TaskI
        {
            FUNCT destFunc;
            ARG1 arg1;
            ARG2 arg2;
            Lambda(FUNCT _func, const ARG1& _arg1, const ARG2& _arg2):
                destFunc(_func),
                arg1(_arg1),
                arg2(_arg2)
            {
            }
            virtual void Run()
            {
                (*destFunc)(arg1,arg2);
            }

            virtual TaskI* Clone()
            {
                return new Lambda(destFunc,arg1, arg2);
            }
        };
        return Task(new Lambda(_func, _arg1,_arg2));
    }

    /**
     * @brief Gen 
     *
     * @param _func 函数
     * @param _arg1 参数
     * @param _arg2
     * @param _arg3
     *
     * @return 
     */
    template<typename FUNCT, typename ARG1,typename ARG2, typename ARG3>
    static Task Gen(FUNCT _func, ARG1 _arg1, ARG2 _arg2, ARG3 _arg3)
    {
        struct Lambda : public TaskI
        {
            FUNCT destFunc;
            ARG1 arg1;
            ARG2 arg2;
            ARG3 arg3;
            Lambda(FUNCT _func, const ARG1& _arg1, const ARG2& _arg2, const ARG3& _arg3):
                destFunc(_func),
                arg1(_arg1),
                arg2(_arg2),
                arg3(_arg3)
            {
            }
            virtual void Run()
            {
                (*destFunc)(arg1,arg2,arg3);
            }

            virtual TaskI* Clone()
            {
                return new Lambda(destFunc,arg1, arg2, arg3);
            }
        };
        cout << "D5" << endl;
        return Task(new Lambda(_func, _arg1,_arg2, _arg3));
    }

    /**
     * @brief Gen 
     *
     * @param _func 函数
     * @param _arg1 参数
     * @param _arg2
     * @param _arg3
     * @param _arg4
     *
     * @return 
     */
    template<typename FUNCT, typename ARG1,typename ARG2, typename ARG3, typename ARG4>
    static Task Gen(FUNCT _func, ARG1 _arg1, ARG2 _arg2, ARG3 _arg3, ARG4 _arg4)
    {
        struct Lambda : public TaskI
        {
            FUNCT destFunc;
            ARG1 arg1;
            ARG2 arg2;
            ARG3 arg3;
            ARG4 arg4;
            Lambda(FUNCT _func,
                    const ARG1& _arg1, const ARG2& _arg2, const ARG3& _arg3, const ARG4& _arg4):
                destFunc(_func),
                arg1(_arg1),
                arg2(_arg2),
                arg3(_arg3),
                arg4(_arg4)
            {
            }
            virtual void Run()
            {
                (*destFunc)(arg1,arg2,arg3,arg4);
            }

            virtual TaskI* Clone()
            {
                return new Lambda(destFunc,arg1, arg2, arg3, arg4);
            }
        };
        cout << "D6" << endl;
        return Task(new Lambda(_func, _arg1,_arg2, _arg3, _arg4));
    }

    /**
     * @brief Gen 
     *
     * @param _func 函数
     * @param _arg1 参数
     * @param _arg2
     * @param _arg3
     * @param _arg4
     * @param _arg5
     *
     * @return 
     */
    template<typename FUNCT,
        typename ARG1,typename ARG2, typename ARG3, typename ARG4,
        typename ARG5>
    static Task Gen(FUNCT _func,
            ARG1 _arg1, ARG2 _arg2, ARG3 _arg3, ARG4 _arg4,
            ARG5 _arg5)
    {
        struct Lambda : public TaskI
        {
            FUNCT destFunc;
            ARG1 arg1;
            ARG2 arg2;
            ARG3 arg3;
            ARG4 arg4;
            ARG5 arg5;
            Lambda(FUNCT _func,
                    const ARG1& _arg1, const ARG2& _arg2, const ARG3& _arg3, const ARG4& _arg4,
                    const ARG5& _arg5):
                destFunc(_func),
                arg1(_arg1),
                arg2(_arg2),
                arg3(_arg3),
                arg4(_arg4),
                arg5(_arg5)
            {
            }
            virtual void Run()
            {
                (*destFunc)(arg1,arg2,arg3,arg4,arg5);
            }

            virtual TaskI* Clone()
            {
                return new Lambda(destFunc,arg1, arg2, arg3, arg4,arg5);
            }
        };
        cout << "D7" << endl;
        return Task(new Lambda(_func, _arg1,_arg2, _arg3, _arg4, _arg5));
    }

    /**
     * @brief Gen 
     *
     * @param _func 函数
     * @param _arg1 参数
     * @param _arg2
     * @param _arg3
     * @param _arg4
     * @param _arg5
     * @param _arg6
     *
     * @return 
     */
    template<typename FUNCT,
        typename ARG1,typename ARG2, typename ARG3, typename ARG4,
        typename ARG5,typename ARG6>
    static Task Gen(FUNCT _func,
            ARG1 _arg1, ARG2 _arg2, ARG3 _arg3, ARG4 _arg4,
            ARG5 _arg5, ARG6 _arg6)
    {
        struct Lambda : public TaskI
        {
            FUNCT destFunc;
            ARG1 arg1;
            ARG2 arg2;
            ARG3 arg3;
            ARG4 arg4;
            ARG5 arg5;
            ARG6 arg6;
            Lambda(FUNCT _func,
                    const ARG1& _arg1, const ARG2& _arg2, const ARG3& _arg3, const ARG4& _arg4,
                    const ARG5& _arg5, const ARG6& _arg6):
                destFunc(_func),
                arg1(_arg1),
                arg2(_arg2),
                arg3(_arg3),
                arg4(_arg4),
                arg5(_arg5),
                arg6(_arg6)
            {
            }
            virtual void Run()
            {
                (*destFunc)(arg1,arg2,arg3,arg4,arg5,arg6);
            }

            virtual TaskI* Clone()
            {
                return new Lambda(destFunc,arg1, arg2, arg3, arg4,arg5,arg6);
            }
        };
        cout << "D8" << endl;
        return Task(new Lambda(_func, _arg1,_arg2, _arg3, _arg4, _arg5,_arg6));
    }

    // C++系列任务
    template<typename T, typename RET>
    static Task Gen(RET (T::*_func)(void), T* _obj)
    {
        struct Lambda: public TaskI
        {
            RET (T::*destFunc)(void);
            T* obj;
            Lambda(RET (T::*_func)(void), T* _obj):
                destFunc(_func),
                obj(_obj)
            {
            }

            virtual void Run()
            {
                (obj->*destFunc)();
            }

            virtual TaskI* Clone()
            {
                return new Lambda(destFunc, obj);
            }
        };
        return Task(new Lambda(_func, _obj));
    }

    template<typename T, typename RET, typename FARG1, typename ARG1>
    static Task Gen(RET (T::*_func)(FARG1), T* _obj, ARG1 _arg1)
    {
        struct Lambda: public TaskI
        {
            RET (T::*destFunc)(FARG1);
            T* obj;
            ARG1 arg1;
            Lambda(RET (T::*_func)(FARG1), T* _obj, const ARG1& _arg1):
                destFunc(_func),
                obj(_obj),
                arg1(_arg1)
            {
            }

            virtual void Run()
            {
                (obj->*destFunc)(arg1);
            }

            virtual TaskI* Clone()
            {
                return new Lambda(destFunc, obj, arg1);
            }
        };
        return Task(new Lambda(_func, _obj, _arg1));
    }

    template<typename T, typename RET,
        typename FARG1, typename FARG2,
        typename ARG1, typename ARG2>
    static Task Gen(RET (T::*_func)(FARG1,FARG2), T* _obj,
            ARG1 _arg1,
            ARG2 _arg2)
    {
        struct Lambda: public TaskI
        {
            RET (T::*destFunc)(FARG1,FARG2);
            T* obj;
            ARG1 arg1;
            ARG2 arg2;
            Lambda(RET (T::*_func)(FARG1,FARG2), T* _obj,
                    const ARG1& _arg1,
                    const ARG2& _arg2):
                destFunc(_func),
                obj(_obj),
                arg1(_arg1),
                arg2(_arg2)
            {
            }

            virtual void Run()
            {
                (obj->*destFunc)(arg1,arg2);
            }

            virtual TaskI* Clone()
            {
                return new Lambda(destFunc, obj, arg1,arg2);
            }
        };
        return Task(new Lambda(_func, _obj, _arg1, _arg2));
    }

    template<typename T, typename RET,
        typename FARG1, typename FARG2, typename FARG3,
        typename ARG1, typename ARG2, typename ARG3>
    static Task Gen(RET (T::*_func)(FARG1,FARG2,FARG3), T* _obj,
            ARG1 _arg1,
            ARG2 _arg2,
            ARG3 _arg3)
    {
        struct Lambda: public TaskI
        {
            RET (T::*destFunc)(FARG1,FARG2,FARG3);
            T* obj;
            ARG1 arg1;
            ARG2 arg2;
            ARG3 arg3;
            Lambda(RET (T::*_func)(FARG1,FARG2,FARG3), T* _obj,
                    const ARG1& _arg1,
                    const ARG2& _arg2,
                    const ARG3& _arg3):
                destFunc(_func),
                obj(_obj),
                arg1(_arg1),
                arg2(_arg2),
                arg3(_arg3)
            {
            }

            virtual void Run()
            {
                (obj->*destFunc)(arg1,arg2,arg3);
            }

            virtual TaskI* Clone()
            {
                return new Lambda(destFunc, obj, arg1,arg2,arg3);
            }
        };
        return Task(new Lambda(_func, _obj, _arg1, _arg2,_arg3));
    }

    template<typename T, typename RET,
        typename FARG1, typename FARG2, typename FARG3, typename FARG4,
        typename ARG1, typename ARG2, typename ARG3, typename ARG4>
    static Task Gen(RET (T::*_func)(FARG1,FARG2,FARG3,FARG4), T* _obj,
            ARG1 _arg1,
            ARG2 _arg2,
            ARG3 _arg3,
            ARG4 _arg4)
    {
        struct Lambda: public TaskI
        {
            RET (T::*destFunc)(FARG1,FARG2,FARG3,FARG4);
            T* obj;
            ARG1 arg1;
            ARG2 arg2;
            ARG3 arg3;
            ARG4 arg4;
            Lambda(RET (T::*_func)(FARG1,FARG2,FARG3,FARG4), T* _obj,
                    const ARG1& _arg1,
                    const ARG2& _arg2,
                    const ARG3& _arg3,
                    const ARG4& _arg4):
                destFunc(_func),
                obj(_obj),
                arg1(_arg1),
                arg2(_arg2),
                arg3(_arg3),
                arg4(_arg4)
            {
            }

            virtual void Run()
            {
                (obj->*destFunc)(arg1,arg2,arg3,arg4);
            }

            virtual TaskI* Clone()
            {
                return new Lambda(destFunc, obj, arg1,arg2,arg3,arg4);
            }
        };
        return Task(new Lambda(_func, _obj, _arg1, _arg2,_arg3,_arg4));
    }

    template<typename T, typename RET,
        typename FARG1, typename FARG2, typename FARG3, typename FARG4, typename FARG5,
        typename ARG1, typename ARG2, typename ARG3, typename ARG4, typename ARG5>
    static Task Gen(RET (T::*_func)(FARG1,FARG2,FARG3,FARG4,FARG5), T* _obj,
            ARG1 _arg1,
            ARG2 _arg2,
            ARG3 _arg3,
            ARG4 _arg4,
            ARG5 _arg5)
    {
        struct Lambda: public TaskI
        {
            RET (T::*destFunc)(FARG1,FARG2,FARG3,FARG4,FARG5);
            T* obj;
            ARG1 arg1;
            ARG2 arg2;
            ARG3 arg3;
            ARG4 arg4;
            ARG5 arg5;
            Lambda(RET (T::*_func)(FARG1,FARG2,FARG3,FARG4,FARG5), T* _obj,
                    const ARG1& _arg1,
                    const ARG2& _arg2,
                    const ARG3& _arg3,
                    const ARG4& _arg4,
                    const ARG5& _arg5):
                destFunc(_func),
                obj(_obj),
                arg1(_arg1),
                arg2(_arg2),
                arg3(_arg3),
                arg4(_arg4),
                arg5(_arg5)
            {
            }

            virtual void Run()
            {
                (obj->*destFunc)(arg1,arg2,arg3,arg4,arg5);
            }

            virtual TaskI* Clone()
            {
                return new Lambda(destFunc, obj, arg1,arg2,arg3,arg4,arg5);
            }
        };
        return Task(new Lambda(_func, _obj, _arg1, _arg2,_arg3,_arg4,_arg5));
    }

    template<typename T, typename RET,
        typename FARG1, typename FARG2, typename FARG3, typename FARG4, typename FARG5,typename FARG6,
        typename ARG1, typename ARG2, typename ARG3, typename ARG4, typename ARG5, typename ARG6>
    static Task Gen(RET (T::*_func)(FARG1,FARG2,FARG3,FARG4,FARG5,FARG6), T* _obj,
            ARG1 _arg1,
            ARG2 _arg2,
            ARG3 _arg3,
            ARG4 _arg4,
            ARG5 _arg5,
            ARG6 _arg6)
    {
        struct Lambda: public TaskI
        {
            RET (T::*destFunc)(FARG1,FARG2,FARG3,FARG4,FARG5,FARG6);
            T* obj;
            ARG1 arg1;
            ARG2 arg2;
            ARG3 arg3;
            ARG4 arg4;
            ARG5 arg5;
            ARG6 arg6;
            Lambda(RET (T::*_func)(FARG1,FARG2,FARG3,FARG4,FARG5,FARG6), T* _obj,
                    const ARG1& _arg1,
                    const ARG2& _arg2,
                    const ARG3& _arg3,
                    const ARG4& _arg4,
                    const ARG5& _arg5,
                    const ARG6& _arg6):
                destFunc(_func),
                obj(_obj),
                arg1(_arg1),
                arg2(_arg2),
                arg3(_arg3),
                arg4(_arg4),
                arg5(_arg5),
                arg6(_arg6)
            {
            }

            virtual void Run()
            {
                (obj->*destFunc)(arg1,arg2,arg3,arg4,arg5,arg6);
            }

            virtual TaskI* Clone()
            {
                return new Lambda(destFunc, obj, arg1,arg2,arg3,arg4,arg5,arg6);
            }
        };
        return Task(new Lambda(_func, _obj, _arg1, _arg2,_arg3,_arg4,_arg5,_arg6));
    }
};

}; // namespace mu

#endif
