#ifndef _BASE_TASK_QUEUE_IMPL_H_
#define _BASE_TASK_QUEUE_IMPL_H_
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
    int BatchRun();
private:
    volatile bool m_flag;
    TaskList m_taskList;
    Mutex m_mutex;
    ConditionVar m_cond;
};

};
#endif
