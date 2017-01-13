#include "lock.h"

using namespace mu;

Mutex::Mutex()
{
    pthread_mutex_init(&m_mutex, NULL);
}

Mutex::~Mutex()
{
    pthread_mutex_destroy(&m_mutex);
}

bool Mutex::Lock()
{
    if (pthread_mutex_lock(&m_mutex))
    {
        return false;
    }
    return true;
}

bool Mutex::TimeLock(int _us)
{
    timespec ts = {_us/(1000000), _us*(1000)};
    if (pthread_mutex_timedlock(&m_mutex, &ts))
    {
        return false;
    }
    return true;
}

bool Mutex::Unlock()
{
    if (pthread_mutex_unlock(&m_mutex))
    {
        return false;
    }
    return true;
}

bool Mutex::TryLock()
{
    if (pthread_mutex_trylock(&m_mutex))
    {
        return false;
    }
    return true;
}

pthread_mutex_t& Mutex::GetMutex() 
{
    return m_mutex;
}

ConditionVar::ConditionVar(Mutex& _mutex):
    m_mutex(_mutex)
{
    pthread_cond_init(&m_cond, NULL);
}

ConditionVar::~ConditionVar()
{
    pthread_cond_destroy(&m_cond);
}

bool ConditionVar::Wait()
{
    return 0 == pthread_cond_wait(&m_cond, &m_mutex.GetMutex());
}

bool ConditionVar::TimeWait(int us_)
{
    timespec ts = {us_/(1000000), us_*(1000)};
    return 0 == pthread_cond_timedwait(&m_cond, &m_mutex.GetMutex(), &ts);
}

bool ConditionVar::Signal()
{
    return 0 == pthread_cond_signal(&m_cond);
}

bool ConditionVar::Broadcast()
{
    return 0 == pthread_cond_broadcast(&m_cond);
}

LockGuard::LockGuard(Mutex& mutex_):
    m_flag(false),
    m_mutex(mutex_)
{
    m_mutex.Lock();
    m_flag = true;
}
LockGuard::~LockGuard()
{
    if (IsLocked())
    {
        Unlock();
    }
}

bool LockGuard::Unlock()
{
    m_flag = false;
    return m_mutex.Unlock();
}

bool LockGuard::IsLocked() const 
{ 
    return m_flag; 
}
