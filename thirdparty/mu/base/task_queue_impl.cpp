#include "task_queue_impl.h"

using namespace mu;

TaskQueue::TaskQueue():
    m_flag(true),
    m_cond(m_mutex)
{
}
TaskQueue::~TaskQueue()
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
        cout<< "tongzhi"<<endl;
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
            cout << "线程结束"<<endl;
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
        cout<<"处理"<<endl;
    }
    cout<<"线程结束 2"<<endl;
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

void TaskQueuePool::TaskFunc(void* _pd)
{
    TaskQueuePool* t = (TaskQueuePool*)_pd;
    t->Run();
}

Task TaskQueuePool::GenTask(TaskQueuePool* p)
{
    return Task(&TaskFunc, p);
}

TaskQueuePool::TaskQueuePool(int n):
    m_index(0)
{
    for(int i = 0; i < n; ++i)
    {
        TaskQueue* p = new TaskQueue();
        m_tqs.push_back(p);
    }
}

TaskQueuePool::~TaskQueuePool()
{
    TaskQueueVt::iterator it = m_tqs.begin();
    for(; it != m_tqs.end(); ++it)
    {
        delete (*it);
    }
    m_tqs.clear();
}

void TaskQueuePool::Run()
{
    TaskQueue* p = NULL;
    {
        LockGuard lock(m_mutex);
        if(m_index >= (int)m_tqs.size())
        {
            throw runtime_error("too more thiread");
        }
        p = m_tqs[m_index++];
    }
    p->BatchRun();
}

void TaskQueuePool::Close()
{
    TaskQueueVt::iterator it = m_tqs.begin();
    for(; it != m_tqs.end(); ++it)
    {
        (*it)->Close();
    }
}

size_t TaskQueuePool::Size() const
{
    return m_tqs.size();
}

TaskQueueI* TaskQueuePool::Alloc(long _id)
{
    return m_tqs[_id % m_tqs.size()];
}

TaskQueueI* TaskQueuePool::RandAlloc()
{
    static unsigned long _id = 0;
    return m_tqs[++_id % m_tqs.size()];
}
