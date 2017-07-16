#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include "TCPServer.h"

TCPServer::TCPServer(int port, const char* addr)
    :_listenerSockfd(0), _port(port), _addr(addr), _isListening(false) {}

TCPServer::~TCPServer() {
    if (_listenerSockfd)
	close(_listenerSockfd);
}

/* initialize a socket and begin listening for connections */
int TCPServer::start() {
    int ret;
    struct sockaddr_in address;
    
    /* make sure not already listening */
    if (_isListening) 
	return 0;
    
    /* setup socket */
    _listenerSockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (_listenerSockfd < 0) {
	fprintf(stderr, "socket() failed: %d\n", _listenerSockfd);
	return _listenerSockfd;
    }

    /* setup addressing */
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(_port);
    if (sizeof _addr)
	inet_pton(AF_INET, _addr, &(address.sin_addr));
    address.sin_addr.s_addr = INADDR_ANY;

    /* hook them up and listen for incoming connections*/
    ret = bind(_listenerSockfd, (struct sockaddr*)&address, sizeof(address));
    if (ret) {
	fprintf(stderr, "bind() failed: %d\n", ret);
	return ret;
    }

    ret = listen(_listenerSockfd, 10);
    if (ret) {
	fprintf(stderr, "listen() failed: %d\n", ret);
	return ret;
    }

    _isListening = true;
    return ret;
}

/* accept incoming connection and assign it to a stream */
TCPStream* TCPServer::accept() {
    int sockfd;
    struct sockaddr_in address;
    socklen_t len = sizeof(address);
    
    /* check for sanity */
    if (!_isListening)
	return NULL;
    
    /* accept incoming connection from address */
    memset(&address, 0, sizeof(address));
    sockfd = ::accept(_listenerSockfd, (struct sockaddr*)&address, &len);
    if (sockfd < 0) {
	fprintf(stderr, "accept() failed: %d\n", sockfd);
	return NULL;
    }

    /* assign connection to a stream */
    return new TCPStream(sockfd, &address);
}
