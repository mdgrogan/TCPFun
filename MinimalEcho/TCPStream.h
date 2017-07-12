#ifndef TCPSTREAM_H
#define TCPSTREAM_H

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>


class TCPStream
{
    public:
    /* make visible to using classes */
    friend class TCPServer;
    friend class TCPClient;
    
    ~TCPStream();

    /* wrap send/recv */
    ssize_t send(const char* buffer, size_t len, int flags);
    ssize_t recv(char* buffer, size_t len, int flags);

    std::string	getPeerIP();
    int		getPeerPort();
    
    private:
    TCPStream(int sockfd, struct sockaddr_in* addr);

    int		_sockfd;	/* socket descriptor */
    std::string _peerIP;	/* IP address of opposite socket */
    int		_peerPort;	/* port no. of opposite socket */
};

#endif /* TCPSTREAM_H */
