#ifndef _SET_H_
#define _SET_H_
#include "hash.h"

/*
 * unordered set Implemented by hash and double list
 */
#define unorderSetCreate            hashCreate
#define unorderSetAdd               hashAdd
#define unorderSetDelete            hashDelete
#define unorderSetDestory           hashDestory


#define unorderSetContain(s, key)   (hashSearch(h, key) != NULL)

#endif /* _SET_H_ */