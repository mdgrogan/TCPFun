#include "TCPStream.h"

/* TCPStream constructor */
TCPStream::TCPStream(int sockfd, struct sockaddr_in* addr):_sockfd(sockfd) {
    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, (struct in_addr*)&(addr->sin_addr.s_addr), ip, INET_ADDRSTRLEN);
    ip[INET_ADDRSTRLEN] = 0; /* ensure null termination */
    _peerIP = ip;
    _peerPort = ntohs(addr->sin_port);
}

/* TCPStream destructor */
TCPStream::~TCPStream() {
    close(_sockfd);
}

/* Basically wrap send to hide socket descriptor */
ssize_t TCPStream::send(const char* buffer, size_t len, int flags) {
    return ::send(_sockfd, buffer, len, flags);
}

/* Basically wrap recv to hide socket descriptor */
ssize_t TCPStream::recv(char* buffer, size_t len, int flags) {
    return ::recv(_sockfd, buffer, len, flags);
}

/* Get IP and port information */
std::string TCPStream::getPeerIP() {
    return _peerIP;
}

int TCPStream::getPeerPort() {
    return _peerPort;
}
    
