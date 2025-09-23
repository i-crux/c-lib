#include "singleList.h"
#include "test.h"

typedef struct
{
    SingleList l;
    int key;
} _SListData;

void __free(void *p)
{
    _SListData *sld = p;
    printf("key = %d\n", sld->key);
    FREE(sld);
}

void __visit(SingleList *p)
{
    _SListData *sld = containerOf(p, _SListData, l);
    printf("key = %d\n", sld->key);
}

static inline void test_singleListInitCreate()
{
    _TEST_BEGIN();

    SingleList *l = singleListCreate();
    assert(l);
    assert(l->next == NULL);

    FREE(l);
    _TEST_END();
}

static inline void test_singleListAddDelete()
{
    _TEST_BEGIN();

    SingleList *l1, *l2, *l3, *h;
    l1 = singleListCreate();
    l2 = singleListCreate();
    l3 = singleListCreate();
    h = l1;
    h = singleListAdd(h, l2);
    assert(h->next == l1);
    h = singleListAdd(h, l3);
    assert(h == l3);
    assert(l2->next == l1);

    h = singleListDelete(h);
    assert(h == l2);
    FREE(l3);
    h = singleListDelete(h);
    assert(h == l1);
    FREE(l2);
    h = singleListDelete(h);
    assert(h == NULL);
    FREE(l1);

    singleListDestory(h, FREE);

    _TEST_END();
}

static inline void test_singleListAppendReverse()
{
    _TEST_BEGIN();
    _SListData *lds[6];
    SingleList *h1, *h2;

    for (int i = 0; i < 6; i++)
    {
        lds[i] = MALLOC(sizeof(_SListData));
        singleListInit(&lds[i]->l);
        lds[i]->key = i;
    }

    h1 = singleListAdd(&lds[0]->l, &lds[1]->l);
    h1 = singleListAdd(h1, &lds[2]->l);
    h2 = singleListAdd(&lds[3]->l, &lds[4]->l);
    h2 = singleListAdd(h2, &lds[5]->l);
    singleListTraval(h1, __visit);
    printf("init ----------\n");
    singleListTraval(h2, __visit);
    printf("init ----------\n");
    h1 = singleListAppend(h2, h1);
    singleListTraval(h1, __visit);
    printf("append ----------\n");

    h1 = singleListReverse(h1);
    singleListTraval(h1, __visit);
    printf("reverse ----------\n");

    h2 = singleListSplit(h1, 3);
    singleListTraval(h1, __visit);
    printf("split ----------\n");
    singleListTraval(h2, __visit);
    printf("split ----------\n");

    singleListDestory(h1, __free);
    if (h1 != h2)
        singleListDestory(h2, __free);

    printf("destory ------------\n");

    _TEST_END();
}

int main(void)
{

    test_singleListInitCreate();
    test_singleListAddDelete();
    test_singleListAppendReverse();

    return 0;
}
