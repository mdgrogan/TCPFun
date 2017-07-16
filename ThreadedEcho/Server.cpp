#include <cstdlib>
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "TCPServer.h"

#define NUM_THREADS 3

void *job(void *arg) {
    char buf[256];
    ssize_t len;
    TCPStream *s = (TCPStream *)arg;

    if (s) {
        while ((len = s->recv(buf, sizeof(buf), 0)) > 0) {
    	buf[len] = 0;
    	printf("Recieved: %s\n", buf);

    	s->send(buf, len, 0);
    	memset(buf, 0, sizeof(buf));
        }
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    if (argc < 2 || argc > 4)
	return 1;
    
    int ret;
    pthread_t threadList[NUM_THREADS];
    pthread_attr_t attr;
    void *status;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    

    TCPStream *stream[NUM_THREADS];
    TCPServer *server = NULL;
    if (argc == 3)
	server = new TCPServer(atoi(argv[1]), argv[2]);
    else
	server = new TCPServer(atoi(argv[1]));

    if (server->start() != 0) {
	fprintf(stderr, "start() failed\n");
	return 1;
    }

    for (int i = 0; i < NUM_THREADS; i++) {
	stream[i] = server->accept();
	ret = pthread_create(&threadList[i], &attr, job, (void *)stream[i]);
	if (ret) {
	    fprintf(stderr, "pthread_create(%d) failed: %d\n", i, ret);
	    return 1;
	}
	printf("thread #%d created\n", i);
    }
    pthread_attr_destroy(&attr);

    for (int i = 0; i < NUM_THREADS; i++) {
	ret = pthread_join(threadList[i], &status);
	if (ret) {
	    fprintf(stderr, "pthread_join(%d) failed: %d\n", i, ret);
	    return 1;
	}
	printf("thread[%d] terminated stream with %d : %s\n",
	       	i, stream[i]->getPeerPort(), (stream[i]->getPeerIP()).c_str());
    }

    return 0;
}
