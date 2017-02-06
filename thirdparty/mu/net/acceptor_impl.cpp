#include <sys/socket.h>
#include <assert.h>
#include <stdio.h>
#include <sys/types.h>
#include <netdb.h>

#include <verto.h>
using namespace std;

#include "epoll_i.h"
#include "thirdparty/mu/base/string_utility.h"
#include "acceptor_impl.h"
#include "socket_op.h"
#include "thirdparty/mu/base/task_queue_impl.h"
#include "socket_impl.h"
#include "common_socket_controller.h"

using namespace mu;

AcceptorImpl::AcceptorImpl(EpollI* _epoll, MsgHandlerI* _msgHandler, TaskQueuePool* _tq):
    m_listenFd(-1),
    m_epoll(_epoll),
    m_msgHandler(_msgHandler),
    m_tq(_tq)
{
}

AcceptorImpl::~AcceptorImpl()
{
    this->Close();
}


int AcceptorImpl::Open(const string& _address)
{
    //! tcp://127.0.0.1:8080
    vector<string> vt;
    StringUtility::Split(_address, vt, "://");
    assert(2 == vt.size());

    vector<string> vt2;
    StringUtility::Split(vt[1], vt2, ":");
    assert(2 == vt2.size());

    struct addrinfo hints;
    bzero(&hints, sizeof(struct addrinfo) );
    hints.ai_family      = AF_UNSPEC;
    hints.ai_socktype  = SOCK_STREAM;
    hints.ai_flags        = AI_PASSIVE;

    int ret =0, yes = 1;
    struct addrinfo *res;
    const char* host = NULL;
    if(vt2[0] != "*")
    {
        host = vt2[0].c_str();
    }

    if ((ret = getaddrinfo(host, vt2[1].c_str(), &hints, &res)) != 0) 
    {
        fprintf(stderr, "AcceptorImpl::Open getaddrinfo: %s, address_=<%s>\n", gai_strerror(ret), _address.c_str());
        return -1;
    }

    if ((m_listenFd = ::socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1)
    {
        perror("AcceptorImpl::Open when socket");
        return -1;
    }

    if (::setsockopt(m_listenFd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
    {
        perror("AcceptorImpl::Open when setsockopt");
        return -1;
    }

    if (::bind(m_listenFd, res->ai_addr, res->ai_addrlen) == -1)
    {
        fprintf(stderr, "AcceptorImpl::Open when bind: %s, address_=<%s>\n", strerror(errno), _address.c_str());
        return -1;
    }

    SocketOp::SetNonBlock(m_listenFd);
    if (::listen(m_listenFd, LISTEN_BACKLOG) == -1)
    {
        perror("acceptor_impl_t::Open when listen");
        return -1;
    }

    ::freeaddrinfo(res);

    m_epoll->RegisterFd(this);
    return 0;
}

void AcceptorImpl::Close()
{
    if(m_listenFd > 0)
    {
        assert(0 == m_epoll->UnRegisterFd(this));
        ::close(m_listenFd);
        m_listenFd = -1;
    }
}

int AcceptorImpl::HandleEpollRead()
{
    struct sockaddr_storage addr;
    socklen_t addrlen = sizeof(addr);

    Socket newFd = -1;

    do
    {
        if((newFd = ::accept(m_listenFd, (struct sockaddr *)&addr, &addrlen)) == -1)
        {
            if (errno == EWOULDBLOCK)
            {
                return 0;
            }
            else if (errno == EINTR || errno == EMFILE || errno == ECONNABORTED || errno == ENFILE ||
                    errno == EPERM || errno == ENOBUFS || errno == ENOMEM)
            {
                perror("accept");
                m_epoll->ModFd(this);
                return 0;
            }
            perror("accept");
            return -1;
        }
        cout<< "x1"<<endl;

        SocketI* socket = CreateSocket(newFd);
        socket->Open();
    }while(true);

    return 0;
}

int AcceptorImpl::HandleEpollDel()
{
    return 0;
}

SocketI* AcceptorImpl::CreateSocket(Socket _newFd)
{
    return new SocketImpl(m_epoll, new CommonSocketController(m_msgHandler), _newFd, m_tq->Alloc(_newFd));
}
