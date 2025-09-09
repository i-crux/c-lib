#ifndef _TEST_H_
#define _TEST_H_

#include <stdio.h>
#include <assert.h>

#define _TEST_BEGIN()                                                           \
    do {                                                                        \
        printf("\033[34mtest: %s ---------> begin.\033[0m\n", &__func__[5]);    \
    } while(0)


#define _TEST_END()                                                             \
    do {                                                                        \
        printf("\033[32mtest: %s ---------> passed.\033[0m\n\n", &__func__[5]); \
    } while(0)

#endif /* _TEST_H_ */