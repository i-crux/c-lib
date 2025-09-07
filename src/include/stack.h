#ifndef _STACK_H_
#define _STACK_H_

#include "array.h"

typedef Array Stack;

/**
 * @brief initialize a [Stack], memory for [stack] is managed by caller
 * 
 * @param stack: pointor to [Stack]
 * @param eSize: size of single element
 * 
 * @return 1 on successfull. 0 on failure
 */
bool stackInit(Stack *stack, int eSize);


/**
 * @brief create a [Stack]
 * 
 * @param eSize: size of single element
 * 
 * @return pointor to a [Stack] on successful. [NULL] on failure
 */
Stack *stackCreate(int eSize);


#define stackEmpty arrayEmpty

#define stackPush arrayPush

#define stackPop arrayPop

#define stackDestroy arrayDestroy

#endif /* _STACK_H_ */