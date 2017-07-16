#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "TCPStream.h"

class TCPServer
{
    public:
    TCPServer(int port, const char* addr = "");
    ~TCPServer();

    int		start();
    TCPStream*	accept();
    
    private:
    int		_listenerSockfd;
    int		_port;
    const char* _addr;
    bool	_isListening;
};

#endif /* TCPSERVER_H */
