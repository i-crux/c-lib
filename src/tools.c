#include "tools.h"
#include "utils.h"

_Thread_local static bool __seedSet = 0;

char *genRandomAsciiStr(int length)
{
    const char charset[] = "abcdefghijklmnopqrstuvwxyz"
                           "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                           "0123456789";
    int idx, size = sizeof(charset) - 1; /* remove '\0' */

    char *str = MALLOC(length + 1);

    ckpvThenReturn(str, NULL, NULL);

    if (!__seedSet)
    {
        srand(time(NULL)); /* set seed of random */
        __seedSet = 1;
    }

    for (int i = 0; i < length; i++)
    {
        idx = rand() % size;
        str[i] = charset[idx];
    }

    str[length] = '\0';
    return str;
}