/**
 * @file signal_utility.h
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

 * @brief 信号包装
 * @author lee, china2011long@163.com
 * @version 1.0.0
 * @date 2017-02-05
 */

#ifndef _BASE_SIGNAL_UTILITY_H_
#define _BASE_SIGNAL_UTILITY_H_

#include <signal.h>

namespace mu
{

class SignalUtility
{
public:
    /**
     * @brief Wait 等待信号
     *
     * @return 
     */
    static int Wait()
    {
        sigset_t maskSig;
        sigemptyset(&maskSig);

        sigaddset(&maskSig, SIGINT);
        sigaddset(&maskSig, SIGQUIT);
        sigaddset(&maskSig, SIGTERM);

        int sigNum = 0;
        pthread_sigmask(SIG_BLOCK, &maskSig, 0);

        ::sigwait(&maskSig, &sigNum);
        return sigNum;
    }
};
}; // namespace mu

#endif
