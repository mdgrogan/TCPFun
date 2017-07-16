#include <cstdlib>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define NUM_THREADS 10

void *job(void *arg) {
    int count = 0;
    long id = (long)arg;
    int x = std::rand() % 10;
    sleep(x);
    printf("thread #%ld: slept %ld seconds\n", id, x);
    count++;
    pthread_exit((void*)count);
}

int main()
{
    int ret;
    pthread_t threadList[NUM_THREADS];
    pthread_attr_t attr;
    void *status;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    for (int i = 0; i < NUM_THREADS; i++) {
	printf("main(): creating thread #%d\n", i);
	ret = pthread_create(&threadList[i], &attr, job, (void*)i);
	if (ret) {
	    fprintf(stderr, "pthread_create() failed: %d\n", ret);
	    return 1;
	}
    }
    pthread_attr_destroy(&attr);

    for (int i = 0; i < NUM_THREADS; i++) {
	ret = pthread_join(threadList[i], &status);
	if (ret) {
	    fprintf(stderr, "unable to join: %d\n", ret);
	    return 1;
	}
	printf("main(): completed thread #%d with status (count) %d\n", i, status);
    }

    printf("main(): exiting\n");
    pthread_exit(NULL);
}
