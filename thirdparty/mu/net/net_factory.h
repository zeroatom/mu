/**
 * @file net_factory.h
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

 * @brief 网络工厂类
 * @author lee, china2011long@163.com
 * @version 1.0.0
 * @date 2017-02-03
 */

#ifndef _NET_NET_FACTORY_H_
#define _NET_NET_FACTORY_H_

#include <assert.h>

#include "thirdparty/mu/base/thread.h"
#include "thirdparty/mu/base/singleton.h"
#include "thirdparty/mu/base/task_queue_impl.h"
#include "msg_handler_i.h"
#include "acceptor_impl.h"
#include "epoll_impl.h"
#include "connector.h"

namespace mu
{

class NetFactory
{
public:
    struct GlobalData
    {
        GlobalData():
            epoll()
        {
            startedFlag = false;
            tg = NULL;
        }

        ~GlobalData()
        {
            Stop();
        }

        /**
         * @brief 开启标识
         */
        volatile bool startedFlag;
        /**
         * @brief 线程管理
         */
        Thread thread;
        /**
         * @brief epoll类
         */
        EpollImpl epoll;
        /**
         * @brief 所有接受者
         */
        vector<AcceptorI*> all_acceptor;
        /**
         * @brief 工作任务池
         */
        TaskQueuePool* tg;

        /**
         * @brief Epoll事件循环
         *
         * @param _e
         */
        static void RunEpoll(void* _e)
        {
            GlobalData* p = (GlobalData*)_e;
            p->epoll.EventLoop();
        }

        /**
         * @brief Start 启动
         *
         * @param 任务线程数
         */
        void Start(int _threadNum = 2)
        {
            if(false == startedFlag)
            {
                assert(_threadNum > 0);
                startedFlag = true;
                tg = new TaskQueuePool(_threadNum);
                thread.CreateThread(Task(&RunEpoll, this), 1);
                thread.CreateThread(TaskQueuePool::GenTask(tg),_threadNum);
            }
        }
        
        /**
         * @brief 停止
         */
        void Stop()
        {
            if(true == startedFlag)
            {
                cout<< "net_factory" <<endl;
                for(size_t i = 0; i < all_acceptor.size(); ++i)
                {
                    all_acceptor[i]->Close();
                }

                for(size_t i = 0; i < all_acceptor.size(); ++i)
                {
                    delete all_acceptor[i];
                }
                all_acceptor.clear();
                tg->Close();
                epoll.Close();
                thread.Join();
                delete tg;
                tg = NULL;
            }
        }

    };

    /**
     * @brief Start 启动
     *
     * @param _threadNum
     *
     * @return 
     */
    static int Start(int _threadNum)
    {
        Singleton<GlobalData>::Instance().Start(_threadNum);
        return 0;
    }

    /**
     * @brief Stop 停止
     */
    static void Stop()
    {
        Singleton<GlobalData>::Instance().Stop();
    }

    /**
     * @brief Listen 监听端口
     *
     * @param _host
     * @param _msgHandler
     *
     * @return 
     */
    static AcceptorI* Listen(const string& _host, MsgHandlerI* _msgHandler)
    {
        Singleton<GlobalData>::Instance().Start();
        AcceptorImpl* ret = new AcceptorImpl(&(Singleton<GlobalData>::Instance().epoll),
                _msgHandler,
                (Singleton<GlobalData>::Instance().tg));

        if(ret->Open(_host))
        {
            delete ret;
            return NULL;
        }
        Singleton<GlobalData>::Instance().all_acceptor.push_back(ret);
        return ret;
    }

    /**
     * @brief Connect 连接远程
     *
     * @param _host
     * @param _msgHandler
     *
     * @return 
     */
    static SocketPtr Connect(const string& _host, MsgHandlerI* _msgHandler)
    {
        Singleton<GlobalData>::Instance().Start();
        return Connector::Connect(_host, &(Singleton<GlobalData>::Instance().epoll), _msgHandler,
                (Singleton<GlobalData>::Instance().tg->RandAlloc()));
    }
};

}; // namespace mu
#endif
