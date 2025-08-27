#include <pthread.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>

#include "utils.h"

#include "test.h"


#define __NUM_THREADS 4
#define __INCREMENTS 10000

static SpinLock _spinLock;

static long long counter = 0;

/* __attribute__((unused)) avoid compiler WARNING */
static void *_worker(__attribute__((unused)) void *arg) {
    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = 100000L; /* sleep 0.1ms */
    for(int i = 0; i < __INCREMENTS; i++) {
        lockSpinLock(&_spinLock);
        counter++;
        nanosleep(&ts, NULL);
        unlockSpinLock(&_spinLock);
    }

    return NULL;
}

static int test_SpinLock() {
    _TEST_BEGIN();

    pthread_t threads[__NUM_THREADS];

    initSpinLock(&_spinLock);

    /* create threads */
    for(int i = 0; i < __NUM_THREADS; i++) {
        if(pthread_create(&threads[i], NULL, _worker, NULL) != 0) {
            perror("pthread_create");
            return -1;
        }
    }

    /* join threads */
    for(int i = 0; i < __NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    assert(counter == __NUM_THREADS * __INCREMENTS);
    
    _TEST_END();

    return 0;
}


int main(void) {
    assert(test_SpinLock() >= 0);
}