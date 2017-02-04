#ifndef _NET_COMMON_SOCKET_CONTROLLER_H_
#define _NET_COMMON_SOCKET_CONTROLLER_H_

#include <string>
using namespace std;

#include "socket_controller_i.h"
#include "message.h"
#include "msg_handler_i.h"

namespace mu
{

class CommonSocketController : public SocketControllerI
{
public:
    CommonSocketController(MsgHandlerPtr _msgHandler);
    ~CommonSocketController();
    /**
     * @brief HandleError 处理错误
     *
     * @param SocketI
     *
     * @return 
     */
    virtual int HandleError(SocketI*);
    /**
     * @brief HandleRead 处理读数据
     *
     * @param SocketI
     * @param buff
     * @param len
     *
     * @return 
     */
    virtual int HandleRead(SocketI*, char* buff, size_t len);
    /**
     * @brief HandleWriteCompleted 写完数据回调
     *
     * @param SocketI
     *
     * @return 
     */
    virtual int HandleWriteCompleted(SocketI*);

    /**
     * @brief CheckPreSend 发送前处理
     *
     * @param SocketI
     * @param _buff
     *
     * @return 
     */
    virtual int CheckPreSend(SocketI*, string& _buff);
private:
    /**
     * @brief 消息处理器
     */
    MsgHandlerPtr m_msgHandler;
    /**
     * @brief 已接受大小
     */
    size_t m_haveRecvSize;
    /**
     * @brief 正在接受的消息
     */
    Message m_message;
    
};
}; // namespace mu
#endif
