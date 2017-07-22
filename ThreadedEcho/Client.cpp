#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TCPClient.h"

int main(int argc, char* argv[])
{
    if (argc != 3)
	return 1;

    char buf[1024] = "";
    ssize_t len;
    TCPClient* client = new TCPClient();
    TCPStream* stream = client->connect(atoi(argv[1]), argv[2]);

    while (stream) {
	scanf("%[^\n]%*c", buf);
	stream->send(buf, sizeof(buf), 0);
	printf("sent: %s\n", buf);

	memset(buf, 0, sizeof(buf));
	len = stream->recv(buf, sizeof(buf), 0);
	buf[len] = 0;
	printf("recieved: %s\n", buf);
	memset(buf, 0, sizeof(buf));
    }

    delete stream;
    return 0;
}
