#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include "TCPStream.h"

class TCPClient
{
    public:
    TCPStream* connect(int port, const char* server);

    private:
    int resolveHostName(const char* host, struct in_addr* addr);
};

#endif /* TCPCLIENT_H */
