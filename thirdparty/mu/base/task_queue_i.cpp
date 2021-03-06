#include "task_queue_i.h"
#include "config.h"

using namespace mu;

TaskImpl::TaskImpl(TASK_FUNC _func, void* _arg):
    m_func(_func),
    m_arg(_arg)
{
}

void TaskImpl::Run()
{
    m_func(m_arg);
}

TaskI* TaskImpl::Clone()
{

    return new TaskImpl(m_func, m_arg);
}

void Task::Dumy(void* _arg)
{
}

Task::Task()
{
    m_taskImpl = new TaskImpl(&Task::Dumy, NULL);
}

Task::Task(TASK_FUNC _f, void* _arg):
    m_taskImpl(new TaskImpl(_f,_arg))
{
}

Task::Task(TaskI* _taskI):
    m_taskImpl(_taskI)
{
}
Task::Task(const Task& _src):
    m_taskImpl(_src.m_taskImpl->Clone())
{
}

Task::~Task()
{
    SAFE_DELETE(m_taskImpl);
}

Task& Task::operator=(const Task& _src)
{
    SAFE_DELETE(m_taskImpl); 

    m_taskImpl = _src.m_taskImpl->Clone();
    return *this;
}

void Task::Run()
{
    m_taskImpl->Run();
}
