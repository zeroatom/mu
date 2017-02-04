#ifndef _NET_SOCKET_CONTROLLER_I_H_
#define _NET_SOCKET_CONTROLLER_I_H_

#include <string>
using namespace std;

namespace mu
{

class SocketI;

class SocketControllerI
{
public:
    virtual ~SocketControllerI()
    {
    }

    virtual int HandleOpen(SocketI*) {return 0;}
    virtual int CheckPreSend(SocketI*, string& _buff) {return 0;};

    virtual int HandleError(SocketI*) = 0;
    virtual int HandleRead(SocketI*, char* buff, size_t len) = 0;
    virtual int HandleWriteCompleted(SocketI*) = 0;
private:
    
};
}; // namespace mu
#endif
