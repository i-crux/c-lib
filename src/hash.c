#include "hash.h"
#include "utils.h"



Hash *hashCreate(hashFunc hashFunc, hashCmpKey cmp, int slotCnt, doubleListNodeFree dlnFunc) {
    Hash *h = MALLOC(sizeof(Hash));

    ckpvThenReturn(h, NULL, NULL);

    h->slot = MALLOC(sizeof(DoubleList) * slotCnt);

    if(!h->slot) {
        FREE(h);
        h = NULL;
        goto _done;
    }

    for(int i = 0; i < slotCnt; i++) {
        initDoubleList(&h->slot[i], dlnFunc);
    }
    
    h->hash = hashFunc;
    h->cmp = cmp;
    h->slotCnt = slotCnt;

_done:
    return h;
}


HashNode *hashNodeCreate(uintptr_t key, size_t dataSize, void *data) {
    HashNode *hn = MALLOC(sizeof(HashNode) + dataSize - 1);

    ckpvThenReturn(hn, NULL, NULL);

    initDoubleListNode(&hn->dln);

    memcpy(hn->data, data, dataSize);

    hn->dataSize = dataSize;
    hn->key = key;

    return hn;
}


HashNode *hashSearch(Hash *h, uintptr_t key) {
    HashNode *hn = NULL, *hnTmp;
    DoubleList *dl;
    DoubleListNode *dln;
    int slotIdx;

    ckpvThenReturn(h, NULL, NULL);
    
    slotIdx = h->hash(key) % h->slotCnt;

    dl = &h->slot[slotIdx];
    dln = dl->header.next;

    while(!isDoubleListSentinel(dln)) {
        hnTmp = containerOf(dln, HashNode, dln);
        if(h->cmp(hnTmp->key, key) == 0) {
            hn = hnTmp;
            goto _done;
        }
        dln = dln->next;
    }

_done:
    return hn;
}


int hashAdd(Hash *h, uintptr_t key, size_t dataSize, void *data) {
    int ret = -1;
    HashNode *hn;
    DoubleList *dl;
    int slotIdx;

    ckpvThenReturn(h, NULL, ret);

    if(hashSearch(h, key)) {    /* duplicated key */
        ret = 1;
        goto _done;
    }

    hn = hashNodeCreate(key, dataSize, data);

    ckpvThenReturn(hn, NULL, ret);

    slotIdx = h->hash(key) % h->slotCnt;
    dl = &h->slot[slotIdx];
    
    addNodeAtDoubleListTail(dl, &hn->dln);
    ret = 0;

_done:
    return ret;
}


HashNode *hashDelete(Hash *h, uintptr_t key) {
    HashNode *hn = hashSearch(h, key);

    ckpvThenReturn(hn, NULL, NULL);

    removeDoubleListNode(&hn->dln);

    return hn;
}

void hashDestory(Hash *h) {
    for(int i = 0; i < h->slotCnt; i++) {
        destoryDoubleList(&h->slot[i], 0);
    }
    FREE(h);
}


uintptr_t defaultHashStr(uintptr_t str) {
    char *c = (char *)str;
    uintptr_t salt = 0;
    while(*c != 0) {
        salt = __hash(salt, *c);
        c++;
    }
    return salt;
}


uintptr_t defaultHashNum(uintptr_t num) {
    uintptr_t n = (uintptr_t)num;
    uintptr_t salt = 0;
    for(int i = 0; i < 7; i++) {
        salt = __hash(salt, n);
    }
    return salt;
}

