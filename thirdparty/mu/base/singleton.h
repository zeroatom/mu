/**
 * @file singleton.h
 * @brief 单例
 * @author lee
 * @version 1.0.0
 * @date 2017-01-11
 */
#ifndef _BASE_SINGLETON_H_
#define _BASE_SINGLETON_H_

#include <stdlib.h>
#include <pthread.h>

namespace mu
{

template<class T>
class Singleton
{
public:
/**
* @brief 获取单例
*
* @return 
*/
    static T& Instance()
    {
        pthread_once(&m_ponce, &Singleton::Init);
        return *m_instance;
    }

/**
* @brief 获取单例指针
*
* @return 
*/
    static T* Instance_ptr()
    {
        pthread_once(&m_ponce, &Singleton::Init);
        return m_instance;
    }
private:
/**
* @brief 初始化单例
*/
    static void Init()
    {
        m_instance = new T();
        atexit(destroy);
    }

/**
* @brief 销毁单例
*/
    static void Destroy()
    {
        if(m_instance)
        {
                delete m_instance;
                m_instance = 0;
        }
    }

private:
    static T* volatile m_instance;
    static pthread_once_t m_ponce;
};

template<class T>
T* volatile Singleton<T>::m_instance = NULL;

template<typename T>
pthread_once_t Singleton<T>::m_ponce = PTHREAD_ONCE_INIT;
};

#endif

