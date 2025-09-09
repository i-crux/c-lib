#include <string.h>

#include "test.h"
#include "tools.h"
#include "utils.h"

static inline void test_genRandomAsciiStr() {
    _TEST_BEGIN();

    char *str = genRandomAsciiStr(16);
    assert(str);
    printf("%s\n", str);
    assert(strlen(str) == 16);
    FREE(str);

    str = genRandomAsciiStr(16);
    assert(str);
    printf("%s\n", str);
    assert(strlen(str) == 16);
    FREE(str);


    _TEST_END();
}

int main(void) {

    test_genRandomAsciiStr();
    return 0;
}