#ifndef _TEST_H_
#define _TEST_H_

#include <stdio.h>
#include <assert.h>

#define _TEST_BEGIN()                                           \
    do {                                                        \
        printf("test: %s ---------> begin.\n", &__func__[5]);   \
    } while(0)


#define _TEST_END()                                                 \
    do {                                                            \
        printf("test: %s ---------> passed.\n\n", &__func__[5]);   \
    } while(0)

#endif /* _TEST_H_ */