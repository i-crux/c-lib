#include <pthread.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>

#include "utils.h"

#include "test.h"


#define __NUM_THREADS 20
#define __INCREMENTS 1000

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

static void *_worker_without_spinlock(__attribute__((unused)) void *arg) {
    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = 100000L; /* sleep 0.1ms */
    for(int i = 0; i < __INCREMENTS; i++) {
        counter++;
        nanosleep(&ts, NULL);
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

    printf("counter = %lld WITH 'spinlock'\n", counter);
    assert(counter == __NUM_THREADS * __INCREMENTS);

    counter = 0;
    /* create threads */
    for(int i = 0; i < __NUM_THREADS; i++) {
        if(pthread_create(&threads[i], NULL, _worker_without_spinlock, NULL) != 0) {
            perror("pthread_create");
            return -1;
        }
    }

    /* join threads */
    for(int i = 0; i < __NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("counter = %lld WITHOUT 'spinlock'\n", counter);
    assert(counter <= __NUM_THREADS * __INCREMENTS);
    
    _TEST_END();

    return 0;
}

static void test_minSizeByAlign() {
    _TEST_BEGIN();

    uintptr_t alignV1 = 0xffffffffffffff00, alignV2=0xffffffffffffffe0;

    printf("minSizeByAlign(%p) = %p(%ld)\n", (void *)alignV1, 
            (void *)minSizeByAlign(alignV1), minSizeByAlign(alignV1));
    assert(minSizeByAlign(alignV1) == 0x100);
    printf("minSizeByAlign(%p) = %p(%ld)\n", 
            (void *)alignV2, (void *)minSizeByAlign(alignV2), minSizeByAlign(alignV2));
    assert(minSizeByAlign(alignV2) == 0x20);

    _TEST_END();
}


static void test_floorAlign() {
    _TEST_BEGIN();

    uintptr_t v, alignV1 = 0xffffffffffffff00, alignV2=0xffffffffffffffe0;

    v = 0x130;
    printf("floorAlign(%p, %p) = %p\n", (void *)v, (void *)alignV1, (void *)floorAlign(v, alignV1));
    assert(floorAlign(v, alignV1) == 0x100);
    printf("floorAlign(%p, %p) = %p\n", (void *)v, (void *)alignV2, (void *)floorAlign(v, alignV2));
    assert(floorAlign(v, alignV2) == 0x120);

    v = 0x30;
    printf("floorAlign(%p, %p) = %p\n", (void *)v, (void *)alignV1, (void *)floorAlign(v, alignV1));
    assert(floorAlign(v, alignV1) == 0x0);
    printf("floorAlign(%p, %p) = %p\n", (void *)v, (void *)alignV2, (void *)floorAlign(v, alignV2));
    assert(floorAlign(v, alignV2) == 0x20);

    _TEST_END();
}

static void test_ceilAlign() {
    _TEST_BEGIN();

    uintptr_t v, alignV1 = 0xffffffffffffff00, alignV2=0xffffffffffffffe0;

    v = 0x130;
    printf("ceilAlign(%p, %p) = %p\n", (void *)v, (void *)alignV1, (void *)ceilAlign(v, alignV1));
    assert(ceilAlign(v, alignV1) == 0x200);
    printf("ceilAlign(%p, %p) = %p\n", (void *)v, (void *)alignV2, (void *)ceilAlign(v, alignV2));
    assert(ceilAlign(v, alignV2) == 0x140);

    v = 0x30;
    printf("ceilAlign(%p, %p) = %p\n", (void *)v, (void *)alignV1, (void *)ceilAlign(v, alignV1));
    assert(ceilAlign(v, alignV1) == 0x100);
    printf("ceilAlign(%p, %p) = %p\n", (void *)v, (void *)alignV2, (void *)ceilAlign(v, alignV2));
    assert(ceilAlign(v, alignV2) == 0x40);

    _TEST_END();
}

static void test_offsetAndcontainerOf() {
    _TEST_BEGIN();
    typedef struct {
        int a;
        int b;
    } TestStruct;
    TestStruct ta;
    assert(offsetOf(TestStruct, a) == 0);
    assert(offsetOf(TestStruct, b) == sizeof(int));
    assert(containerOf(&ta.a, TestStruct, a) == &ta);
    assert(containerOf(&ta.b, TestStruct, b) == &ta);

    _TEST_END();
}


static void test_minAndmax() {
    _TEST_BEGIN();

    int a = 1, b = 3;
    assert(min(a, b) == a);
    assert(max(a, b) == b);

    _TEST_END();
}

static void test_swapMem() {
    _TEST_BEGIN();

    char a[] = "hello Bitch";
    char b[] = "hello World";
    size_t s = sizeof("hello Bitch");

    printf("before swap(size = %lu) a: %s, b: %s\n", s, a, b);

    swapMem(a, b, s);

    printf("after swap(size = %lu) a: %s, b: %s\n", s, a, b);
    
    _TEST_END();
}


int main(void) {
    assert(test_SpinLock() >= 0);
    test_minSizeByAlign();
    test_floorAlign();
    test_ceilAlign();
    test_offsetAndcontainerOf();
    test_minAndmax();
    test_swapMem();
}