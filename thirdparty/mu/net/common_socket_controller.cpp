#include "common_socket_controller.h"

#include <iostream>
using namespace std;

using namespace mu;


CommonSocketController::CommonSocketController(MsgHandlerPtr _msgHandler):
    m_msgHandler(_msgHandler),
    m_haveRecvSize(0)
{
}

CommonSocketController::~CommonSocketController()
{
}

int CommonSocketController::HandleError(SocketI* _sp_)
{
    m_msgHandler->HandleBroken(_sp_);
    return 0;
}

int CommonSocketController::HandleRead(SocketI* _sp, char* buff, size_t len)
{
    size_t left_len = len;
    size_t tmp      = 0;

    while(left_len > 0)
    {
        if(false == m_message.HaveRecvHead(m_haveRecvSize))
        {
            tmp = m_message.AppendHead(m_haveRecvSize, buff, left_len);

            m_haveRecvSize += tmp;
            left_len -= tmp;
            buff += tmp;
        }

        tmp = m_message.AppendMsg(buff, left_len);
        m_haveRecvSize += tmp;
        left_len -= tmp;
        buff += tmp;
        if(m_message.GetBody().size() == m_message.Size())
        {
            m_msgHandler->HandleMsg(m_message, _sp);
            m_haveRecvSize = 0;
            m_message.Clear();
        }
    }
    return 0;
}

int CommonSocketController::HandleWriteCompleted(SocketI* _sp)
{
    return 0;
}

int CommonSocketController::CheckPreSend(SocketI* _sp, string& _buff)
{
    if(_sp->GetSocket() < 0)
    {
        return -1;
    }
    return 0;
}
