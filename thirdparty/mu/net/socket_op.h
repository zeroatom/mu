#ifndef _NET_SOCKET_OP_H_
#define _NET_SOCKET_OP_H_
#include <fcntl.h>
#include <errno.h>

#include "netbase.h"

namespace mu
{
struct SocketOp
{
    /**
     * @brief SetNonBlock 设置非阻塞
     *
     * @param _fd_
     *
     * @return 0- 正常 其他- 失败
     */
    static int SetNonBlock(Socket _fd_)
    {
        int flags;
        flags = fcntl(_fd_, F_GETFL, 0);
        if((flags == fcntl(_fd_, F_SETFL, flags | O_NONBLOCK)) < 0)
        {
            return -1;
        }
        return 0;
    }
};
}; // namespace mu
#endif
