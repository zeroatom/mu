#ifndef _BASE_TIMER_SERVICE_H_
#define _BASE_TIMER_SERVICE_H_

#include <sys/epoll.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <assert.h>
#include <stdint.h>
#include <unistd.h>
#include <list>
#include <map>
using namespace std;

#include "thread.h"
#include "lock.h"
#include "time_utility.h"

namespace mu
{

class TimerService
{
    struct InteruptInfo
    {
        int pairFds[2];
        InteruptInfo()
        {
            assert(0 == ::socketpair(AF_LOCAL, SOCK_STREAM, 0, pairFds));
            ::write(pairFds[1], "0", 1);
        }
        ~InteruptInfo()
        {
            ::close(pairFds[0]);
            ::close(pairFds[1]);
        }
        int ReadFd()
        {
            return pairFds[0];
        }
        int WriteFd()
        {
            return pairFds[1];
        }
    };
    struct RegisteredInfo
    {
        RegisteredInfo(uint64_t _ms, uint64_t _destTm, const Task& _t, bool _isLoop):
            timeout(_ms),
            destTm(_destTm),
            callback(_t),
            isLoop(_isLoop)
        {
        }
        /**
         * @brief IsTimeout 是否超时
         *
         * @param _curTm
         *
         * @return 
         */
        bool IsTimeout(uint64_t _curTm)
        {
            return destTm < _curTm;
        }
        /**
         * @brief 超时时间
         */
        uint64_t timeout;
        /**
         * @brief 目标时间
         */
        uint64_t destTm;
        /**
         * @brief 回调
         */
        Task callback;
        /**
         * @brief 是否循环
         */
        bool isLoop;
    };
    typedef list<RegisteredInfo> RegisteredInfoList;
    typedef multimap<long, RegisteredInfo> RegisteredInfoMap;
public:
    TimerService(long tick = 100)
    {
        mRunning = false;
        mEfd = -1;
        mMinTimeout = tick;
    }
    ~TimerService()
    {
        Stop();
    }

    void Start()
    {
        if(false == mRunning)
        {
            mRunning = true;
            mEfd = ::epoll_create(16);

            struct Lambda
            {
                static void Run(void* _p)
                {
                    ((TimerService*)_p)->Run();
                }
            };
            mThread.CreateThread(Task(&Lambda::Run, this),1);
        }
    }

    void Stop()
    {
        if(true == mRunning)
        {
            mRunning =false;

            Interupt();

            mThread.Join();
            ::close(mEfd);
        }
    }

    /**
     * @brief LoopTimer 循环定时任务
     *
     * @param _ms
     * @param _func
     */
    void LoopTimer(uint64_t _ms, Task _func)
    {
        uint64_t destTm = TimeUtility::GetCurrentMs() + _ms;
        LockGuard lock(mMutex);
        mTmpRegisteredList.push_back(RegisteredInfo(_ms,destTm, _func, true));
    }
    /**
     * @brief OnceTimer 单次定时任务
     *
     * @param _ms
     * @param _func
     */
    void OnceTimer(uint64_t _ms, Task _func)
    {
        uint64_t destTm = TimeUtility::GetCurrentMs() + _ms;
        LockGuard lock(mMutex);
        mTmpRegisteredList.push_back(RegisteredInfo(_ms,destTm, _func, false));
    }

    /**
     * @brief Run 循环
     */
    void Run()
    {
        struct epoll_event ev_set[64];

        do
        {
            ::epoll_wait(mEfd, ev_set, 64, mMinTimeout);

            if(false == mRunning)
            {
                break;
            }

            uint64_t curTm = TimeUtility::GetCurrentMs();

            AddNewTimer();
            ProcessTimerCallback(curTm);
        }while(true);
    }
private:
    /**
     * @brief AddNewTimer 注册未注册的定时器
     */
    void AddNewTimer()
    {
        LockGuard lock(mMutex);
        RegisteredInfoList::iterator it = mTmpRegisteredList.begin();
        for(; it != mTmpRegisteredList.end(); ++it)
        {
            mRegisteredMap.insert(make_pair(it->destTm, *it));
        }
        mTmpRegisteredList.clear();
    }

    /**
     * @brief Interupt 中断
     */
    void Interupt()
    {
        epoll_event ev = { 0, { 0  }  };
        ev.events = EPOLLIN | EPOLLPRI | EPOLLOUT | EPOLLHUP;
        ::epoll_ctl(mEfd, EPOLL_CTL_ADD, mInteruptInfo.ReadFd(), &ev);
    }

    /**
     * @brief ProcessTimerCallback 处理时间回调
     *
     * @param _now
     */
    void ProcessTimerCallback(uint64_t _now)
    {
        RegisteredInfoMap::iterator itBegin = mRegisteredMap.begin();
        RegisteredInfoMap::iterator it = itBegin;

        for(; it != mRegisteredMap.end(); ++it)
        {
            RegisteredInfo& last = it->second;
            if(false == last.IsTimeout(_now))
            {
                break;
            }
            last.callback.Run();
            if(last.isLoop)
            {
                LoopTimer(last.timeout, last.callback);
            }
        }
        if(it != itBegin)
        {
            mRegisteredMap.erase(itBegin, it);
        }
    }
private:
    /**
     * @brief 运行标识
     */
    volatile bool mRunning;
    /**
     * @brief epoll socket
     */
    int mEfd;
    /**
     * @brief 最小定时时间
     */
    volatile long mMinTimeout;
    /**
     * @brief 待加入定时的时间任务
     */
    RegisteredInfoList mTmpRegisteredList;
    /**
     * @brief 已注册的定时任务
     */
    RegisteredInfoMap mRegisteredMap;
    /**
     * @brief 中断循环信息
     */
    InteruptInfo mInteruptInfo;
    /**
     * @brief 线程管理
     */
    Thread mThread;
    /**
     * @brief 锁
     */
    Mutex mMutex;
};
}; // namespace mu
#endif
