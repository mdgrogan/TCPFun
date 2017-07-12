#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TCPServer.h"

int main(int argc, char* argv[])
{
    if (argc < 2 || argc > 4)
	return 1;

    char buf[256];
    ssize_t len;
    TCPStream* stream = NULL;
    TCPServer* server = NULL;
    if (argc == 3)
	server = new TCPServer(atoi(argv[1]), argv[2]);
    else
	server = new TCPServer(atoi(argv[1]));

    if (server->start() == 0) {
	stream = server->accept();
	if (stream) {
	    while ((len = stream->recv(buf, sizeof(buf), 0)) > 0) {
		buf[len] = 0;
		printf("Recieved: %s\n", buf);

		stream->send(buf, len, 0);
		memset(buf, 0, sizeof(buf));
	    }
	    delete stream;
	}
    }

    return 0;
}
