/**
 * @file lock.h
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

 * @brief 所有锁的抽象
 * @author lee, china2011long@163.com
 * @version 1.0.0
 * @date 2017-01-13
 */

#ifndef _BASE_LOCK_H_
#define _BASE_LOCK_H_

#include<pthread.h>

namespace mu
{

/**
 * @brief  普通锁
 */
class Mutex
{
public:
    Mutex();
    ~Mutex();
    /**
     * @brief Lock 锁定
     *
     * @return true- 锁定成功 false- 锁定失败
     */
    bool Lock();
    /**
     * @brief TimeLock 时间锁定
     *
     * @param _us 时间
     *
     * @return true- 时间内锁定成功 false- 时间内锁定失败
     */
    bool TimeLock(int _us);
    /**
     * @brief Unlock 解锁
     *
     * @return true- 解锁成功 false- 解锁失败
     */
    bool Unlock();
    /**
     * @brief TryLock 尝试加锁
     *
     * @return true- 加锁成功 false- 加锁失败
     */
    bool TryLock();

    /**
     * @brief GetMutex 获取底层锁
     *
     * @return 
     */
    pthread_mutex_t& GetMutex();

private:
    /**
     * @brief 底层锁
     */
    pthread_mutex_t              m_mutex;
};

// 条件变量
class ConditionVar
{
public:
    ConditionVar(Mutex& _mutex);
    ~ConditionVar();
    /**
     * @brief Wait 等待信号量
     *
     * @return 
     */
    bool Wait();
    /**
     * @brief TimeWait 时间等待信号量
     *
     * @param us_ 等待时间
     *
     * @return true- 等到信号成功 false- 未等到信号
     */
    bool TimeWait(int us_);
    /**
     * @brief Signal 触发信号
     *
     * @return true- 触发信号成功 false- 触发信号失败
     */
    bool Signal();
    /**
     * @brief Broadcast 广播信号
     *
     * @return true- 广播信号成功 false- 广播信号失败
     */
    bool Broadcast();

private:
    /**
     * @brief 锁
     */
    Mutex&                         m_mutex;
    /**
     * @brief 底层信号对象
     */
    pthread_cond_t                m_cond;
};

//保卫锁
class LockGuard
{
public:
    LockGuard(Mutex& mutex_);
    ~LockGuard();

    /**
     * @brief Unlock 解锁
     *
     * @return true- 解锁成功 false- 解锁失败
     */
    bool Unlock();

    /**
     * @brief IsLocked 是否锁定
     *
     * @return true- 锁定 false- 未锁定
     */
    bool IsLocked() const;
private:
    /**
     * @brief 锁标识
     */
    bool    m_flag;
    /**
     * @brief 锁
     */
    Mutex&      m_mutex;
};

};
#endif
