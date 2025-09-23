#include "test.h"
#include "hash.h"
#include "tools.h"

static int __intKeyCmp(uintptr_t a, uintptr_t b)
{
    int aInt = (int)a, bInt = (int)b;

    return aInt - bInt;
}

static int __strKeyCmp(uintptr_t a, uintptr_t b)
{
    char *stra = (char *)a;
    char *strb = (char *)b;

    return strcmp(stra, strb);
}

static void _dlnFreeIntKeyStrData(void *n)
{
    DoubleListNode *dln = n;
    HashNode *hn = containerOf(dln, HashNode, dln);
    FREE(hn);
}

static void _dlnFreeStrKeyStrDataPtr(void *n)
{
    DoubleListNode *dln = n;
    HashNode *hn = containerOf(dln, HashNode, dln);

    FREE((void *)(hn->key));
    FREE(*((char **)(hn->data)));
    FREE(hn);
}

static inline void test_defaultHashStr()
{
    _TEST_BEGIN();

    char *str1 = "hello world";
    char *str2 = "hello bitch";

    assert(defaultHashStr((uintptr_t)str1) != defaultHashStr((uintptr_t)str2));
    assert(defaultHashStr((uintptr_t)str1) == defaultHashStr((uintptr_t)str1));
    assert(defaultHashStr((uintptr_t)str2) == defaultHashStr((uintptr_t)str2));

    printf("hash(str1) = %lu, hash(str2) = %lu\n", defaultHashStr((uintptr_t)str1), defaultHashStr((uintptr_t)str2));

    _TEST_END();
}

static inline void test_defaultHashNum()
{
    _TEST_BEGIN();

    int a = 1, b = 2;

    assert(defaultHashNum(a) != defaultHashNum(b));
    assert(defaultHashNum(a) == defaultHashNum(a));
    assert(defaultHashNum(b) == defaultHashNum(b));

    printf("hash(a) = %lu, hash(b) = %lu\n", defaultHashNum(a), defaultHashNum(b));

    _TEST_END();
}

static inline void test_hashCreate()
{
    _TEST_BEGIN();
    Hash *h;

    h = hashCreate(defaultHashNum, __intKeyCmp, 17, _dlnFreeIntKeyStrData);
    assert(h);
    assert(h->cmp == __intKeyCmp);
    assert(h->hash == defaultHashNum);
    assert(h->slot != NULL);
    assert(h->slotCnt == 17);

    hashDestory(h);

    h = hashCreate(defaultHashStr, __strKeyCmp, 29, _dlnFreeStrKeyStrDataPtr);
    assert(h);
    assert(h->cmp == __strKeyCmp);
    assert(h->hash == defaultHashStr);
    assert(h->slot != NULL);
    assert(h->slotCnt == 29);
    hashDestory(h);

    _TEST_END();
}

#define _KEY1 "hello"
#define _KEY2 "bitch"
#define _KEY3 "world"
#define _KEY4 "fuck"
#define _MAX_LEN 32

static inline void test_hashNodeCreate()
{
    _TEST_BEGIN();

    srand(time(NULL));

    HashNode *hn;
    void *data;
    size_t dataSize;

    dataSize = rand() % _MAX_LEN + 1;
    hn = hashNodeCreate((uintptr_t)genRandomAsciiStr(rand() % _MAX_LEN + 1),
                        dataSize + 1, (void *)genRandomAsciiStr(dataSize));
    assert(hn);
    assert(hn->data != NULL);
    assert(hn->dataSize == dataSize + 1);
    assert(hn->key != 0);
    printf("key: %s, data: %s\n", (char *)hn->key, (char *)hn->data);
    FREE((void *)hn->key);
    FREE(hn);

    dataSize = rand() % _MAX_LEN + 1;
    data = genRandomAsciiStr(dataSize);
    hn = hashNodeCreate((uintptr_t)genRandomAsciiStr(rand() % _MAX_LEN + 1),
                        sizeof(char *), &data);
    assert(hn);
    assert(*((char **)hn->data) != NULL);
    assert(hn->dataSize == sizeof(char *));
    assert(hn->key != 0);
    printf("key: %s, data: %s\n", (char *)hn->key, *((char **)hn->data));
    FREE((void *)hn->key);
    FREE(*((char **)hn->data));
    FREE(hn);

    dataSize = rand() % _MAX_LEN + 1;
    hn = hashNodeCreate(1,
                        dataSize + 1, (void *)genRandomAsciiStr(dataSize));

    assert(hn);
    assert(hn->data != NULL);
    assert(hn->dataSize == dataSize + 1);
    assert(hn->key == 1);
    printf("key: %lu, data: %s\n", hn->key, (char *)hn->data);
    FREE(hn);

    _TEST_END();
}

static inline void test_hashSearchAddDelete()
{
    _TEST_BEGIN();

    Hash *h;
    HashNode *hn;
    size_t dataSize;
    char *cArr[_MAX_LEN], *tmp, *tmpKey;

    srand(time(NULL));

    h = hashCreate(defaultHashNum, __intKeyCmp, 29, _dlnFreeStrKeyStrDataPtr);

    assert(h);
    for (int i = 0; i < _MAX_LEN; i++)
    {
        dataSize = rand() % _MAX_LEN + 1;
        tmp = genRandomAsciiStr(dataSize);
        assert(hashAdd(h, (uintptr_t)i, dataSize + 1, tmp) == 0);
        cArr[i] = tmp;
    }

    for (int i = 0; i < _MAX_LEN; i++)
    {
        assert(hashAdd(h, (uintptr_t)i, dataSize + 1, tmp) == 1);
    }

    for (int i = 0; i < _MAX_LEN * 2; i++)
    {
        hn = hashSearch(h, (uintptr_t)i);
        if (i < _MAX_LEN)
        {
            assert(hn);
            if (strcmp(cArr[i], (char *)hn->data) != 0)
            {
                printf("%d %s %s\n", i, cArr[i], (char *)hn->data);
            }

            assert(strcmp(cArr[i], (char *)hn->data) == 0);
            assert(hashDelete(h, (uintptr_t)i));
            FREE(hn);
            FREE(cArr[i]);
        }
        else
        {
            assert(!hn);
        }
    }

    for (int i = 0; i < _MAX_LEN; i++)
    {
        assert(hashSearch(h, (uintptr_t)i) == NULL);
    }

    hashDestory(h);

    h = hashCreate(defaultHashStr, __strKeyCmp, 17, _dlnFreeStrKeyStrDataPtr);
    for (int i = 0; i < _MAX_LEN; i++)
    {
        dataSize = rand() % _MAX_LEN + 1;
        tmp = genRandomAsciiStr(dataSize);
        tmpKey = genRandomAsciiStr(rand() % _MAX_LEN + 1);
        int res = hashAdd(h, (uintptr_t)tmpKey, sizeof(char *), &tmp);
        if (res != 0)
        {
            hn = hashSearch(h, (uintptr_t)tmpKey);
            assert(hn);
            printf("%s, %s\n", tmpKey, (char *)hn->key);
        }
        assert(res >= 0);
    }
    hashDestory(h);

    _TEST_END();
}

int main(void)
{

    test_defaultHashStr();
    test_defaultHashNum();
    test_hashCreate();
    test_hashNodeCreate();
    test_hashSearchAddDelete();

    return 0;
}