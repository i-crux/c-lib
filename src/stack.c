#include "stack.h"
#include "utils.h"

#define _INIT_STACK_SIZE 128


bool stackInit(Stack *stack, int eSize) {
    return arrayInit(stack, eSize, _INIT_STACK_SIZE, NULL);
}


Stack *stackCreate(int eSize) {
    return arrayCreate(eSize, _INIT_STACK_SIZE, NULL);
}
