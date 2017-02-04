#include "thread.h"

using namespace mu;

void* Thread::ThreadFunc(void* _p)
{
    Task* t = (Task*)_p;
    t->Run();
    SAFE_DELETE(t);
    return 0;
}

int Thread::CreateThread(Task _func, int num)
{
    for(int i = 0; i < num; ++i)
    {
        pthread_t ntid;
        Task* t = new Task(_func);
        if(0 == ::pthread_create(&ntid, NULL, ThreadFunc, t))
        {
            m_threadList.push_back(ntid);
        }
    }
    return 0;
}

int Thread::Join()
{
    list<pthread_t>::iterator it = m_threadList.begin();
    for(; it != m_threadList.end(); ++it)
    {
        pthread_join((*it), NULL);
    }
    m_threadList.clear();
    return 0;
}
