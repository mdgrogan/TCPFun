#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include "TCPClient.h"

TCPStream* TCPClient::connect(int port, const char* server) {
    int sockfd;
    struct sockaddr_in address;

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    if (!resolveHostName(server, &(address.sin_addr)))
	inet_pton(AF_INET, server, &(address.sin_addr));

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
	fprintf(stderr, "socket() failed: %d\n", sockfd);
	return NULL;
    }

    if (::connect(sockfd, (struct sockaddr*)&address, sizeof(address))) {
	fprintf(stderr, "connect() failed\n");
	close(sockfd);
	return NULL;
    }

    return new TCPStream(sockfd, &address);
}

/* this is totally unnecessary for dicking around on local machine */
int TCPClient::resolveHostName(const char* server, struct in_addr* addr) {
    struct addrinfo* res;
    int ret = getaddrinfo(server, NULL, NULL, &res);
    
    if (ret == 0)
	memcpy(addr, &((struct sockaddr_in*)res->ai_addr)->sin_addr, sizeof(struct in_addr));
    freeaddrinfo(res);

    return ret;
}

