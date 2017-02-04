#ifndef _BASE_TASK_QUEUE_IMPL_H_
#define _BASE_TASK_QUEUE_IMPL_H_

#include <pthread.h>
#include <list>
#include <stdexcept>
using namespace std;

#include "task_queue_i.h"
#include "lock.h"

namespace mu
{

class TaskQueue : public TaskQueueI
{
public:
    TaskQueue();
    ~TaskQueue();
    /**
     * @brief Close 关闭任务队列
     */
    void Close();
    /**
     * @brief Produce 生产任务
     *
     * @param _task
     */
    void Produce(const Task& _task);
    /**
     * @brief MultiProduce 添加多个任务
     *
     * @param _taskList
     */
    void MultiProduce(const TaskList _taskList);
    /**
     * @brief Consume 消费任务
     *
     * @param _task
     *
     * @return 0- 成功 其他- 失败
     */
    int Consume(Task& _task_);
    /**
     * @brief ConsumeAll 消费所有任务
     *
     * @param TaskList
     *
     * @return 0- 成功 其他- 失败
     */
    int ConsumeAll(TaskList&);
    /**
     * @brief Run 一个一个处理任务
     *
     * @return 
     */
    int Run();
    /**
     * @brief BatchRun 批量运行所有任务
     *
     * @return 
     */
    int BatchRun();
private:
    volatile bool m_flag;
    TaskList m_taskList;
    Mutex m_mutex;
    ConditionVar m_cond;
};


/**
 * @brief 工作任务池
 */
class TaskQueuePool
{
    /**
     * @brief 任务池
     */
    typedef vector<TaskQueue*> TaskQueueVt;
    /**
     * @brief 线程任务
     *
     * @param _pd
     */
    static void TaskFunc(void* _pd);
public:
    /**
     * @brief 生成一个线程任务
     *
     * @param p
     *
     * @return 
     */
    static Task GenTask(TaskQueuePool* p);
public:
    TaskQueuePool(int n);
    ~TaskQueuePool();
    /**
     * @brief 运行一个任务队列
     */
    void Run();
    /**
     * @brief Close 关闭所有任务队列
     */
    void Close();
    /**
     * @brief Size 任务池大小
     *
     * @return 
     */
    size_t Size() const;
    /**
     * @brief 分配索引队列
     *
     * @param _id
     *
     * @return 
     */
    TaskQueueI* Alloc(long _id);
    /**
     * @brief 随机分配工作队列
     *
     * @return 
     */
    TaskQueueI* RandAlloc();
private:
    /**
     * @brief 锁
     */
    Mutex m_mutex;
    /**
     * @brief 任务池
     */
    TaskQueueVt m_tqs;
    /**
     * @brief 当前索引
     */
    int m_index;
};

};
#endif
