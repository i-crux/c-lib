#include <assert.h>

#include "test.h"
#include "stack.h"


static inline void test_stackInit() {
    _TEST_BEGIN();
    
    Stack stack;
    assert(stackInit(&stack, sizeof(int)));

    assert(stack.compare == NULL);
    assert(stack.data != NULL);
    assert(stack.currentCapacity == 128);
    assert(stack.initCapacity == 128);
    assert(stack.elemSize == sizeof(int));
    assert(stack.search != NULL);
    assert(stack.nextIndex==0);

    stackDestroy(&stack, 0);

    assert(stack.compare == NULL);
    assert(stack.data == NULL);
    assert(stack.currentCapacity == 0);
    assert(stack.initCapacity == 0);
    assert(stack.elemSize == 0);
    assert(stack.search == 0);
    assert(stack.nextIndex==0);

    _TEST_END();
}

static inline void test_stackCreate() {
    _TEST_BEGIN();

    int a = 1;

    Stack *stack = stackCreate(sizeof(int));
    assert(stack);
    assert(stack->compare == NULL);
    assert(stack->data != NULL);
    assert(arrayCapacity(stack) == 128);
    assert(stack->initCapacity == 128);
    assert(arrayElemCnt(stack) == 0);
    assert(arrayNextIdx(stack) == 0);
    assert(arrayElemSize(stack) == sizeof(int));
    assert(arrayOverFlow(stack, 0));
    assert(arrayOverFlow(stack, 2));
    assert(arrayOverFlow(stack, -1));
    assert(arrayEmpty(stack));
    assert(!arrayIsFull(stack));
    assert(arrayGetAddr(stack, 0) == stack->data);
    assert(arrayGetElem(stack, 0) == NULL);
    assert(!arraySetElem(stack, 0, &a));
    
    assert(stack->search != NULL);
    assert(stack->nextIndex==0);

    stackDestroy(stack, 1);

    _TEST_END();
}


static inline void test_stackPeek() {
    _TEST_BEGIN();

    int *a;

    Stack *stack = stackCreate(sizeof(int));
    assert(stack);

    for(int i; i < 1000; i++) {
        stackPush(stack, &i);
        a = stackPeek(stack);
        assert(*a == i);
    }

    stackDestroy(stack, 1);
    _TEST_END();
}


int main(void) {

    test_stackInit();
    test_stackCreate();
    return 0;
}