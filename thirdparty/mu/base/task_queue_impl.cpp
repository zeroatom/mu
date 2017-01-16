#include "task_queue_impl.h"

using namespace mu;

TaskQueue::TaskQueue():
    m_flag(true),
    m_cond(m_mutex)
{

}

void TaskQueue::Close()
{
    LockGuard lock(m_mutex);
    m_flag = false;
    m_cond.Broadcast();
}

void TaskQueue::Produce(const Task& _task)
{
    LockGuard lock(m_mutex);
    bool needSig = m_taskList.empty();

    m_taskList.push_back(_task);

    if(needSig)
    {
        m_cond.Signal();
    }
}

void TaskQueue::MultiProduce(const TaskList _taskList)
{
    LockGuard lock(m_mutex);
    bool needSig = m_taskList.empty();

    for(TaskList::const_iterator it = _taskList.begin(); it != _taskList.end(); ++it)
    {
        m_taskList.push_back(*it);
    }

    if(needSig)
    {
        m_cond.Signal();
    }
}

int TaskQueue::Consume(Task& _task_)
{
    LockGuard lock(m_mutex);
    while(m_taskList.empty())
    {
        if(false == m_flag)
        {
            return -1;
        }
        m_cond.Wait();
    }

    _task_ = m_taskList.front();
    m_taskList.pop_front();
    return 0;
}

int TaskQueue::ConsumeAll(TaskList& _taskList_)
{
    LockGuard lock(m_mutex);
    while(m_taskList.empty())
    {
        if(false == m_flag)
        {
            return -1;
        }
        m_cond.Wait();
    }

    _taskList_ = m_taskList;

    m_taskList.clear();
    return 0;
}

int TaskQueue::Run()
{
    Task t;
    while(0 == Consume(t))
    {
        t.Run();
    }
    return 0;
}

int TaskQueue::BatchRun()
{
    TaskList tasks;
    int ret = ConsumeAll(tasks);
    while(0 == ret)
    {
        for(TaskList::iterator it = tasks.begin(); it != tasks.end(); ++it)
        {
            (*it).Run();
        }
        tasks.clear();
        ret = ConsumeAll(tasks);
    }
    return 0;
}
