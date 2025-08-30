#if defined(ENABLE_DEBUG)
    #include <stdio.h>
#endif /* if defined(ENABLE_DEBUG) */

#include "buddySys.h"
#include "utils.h"


/* const for align */
#define _BUDDY_ALIGN            0xffffffffffffffc0UL    /* that means the min memory size would be alloced is 64bytes(512bits) */



typedef union {
    uint64_t    addrOfBuddy;    /* [_BuddyMgrSec.addrOfBuddy >> 16] is the address of [Buddy] */
    uint16_t    order;          /* [_BuddyMgrSec.order] >> 8 is the order of the block of memory in this buddy system */
    uint8_t     magic;          /* [_BuddyMgrSec.magic] is the magic number of this buddy system */
} _BuddyMgrSec;


typedef struct {
    _BuddyMgrSec    headerSec;  /* 8bytes, 64bit */
    DoubleListNode  dln;        /* 64 + 64 + 64 = 192bit, 24bytes */
} _BuddyBlockMgr;               /* 256bit, 32bytes */



/**
 * @brief from original value of addrOfBuddy, order, magic build new value for [_BuddyMgrSec]
 * 
 * @param bmsp: the pointor to [_BuddyMgrSec]
 * @param aob: orignal value of addrOfBuddy
 * @param order: original value of order
 * @param magic: original value of magic
 */
#define _initBuddyMgrSec(bmsp, aob, order, magic)           \
    do {                                                    \
        _BuddyMgrSec *__bbms_bmsp = (_BuddyMgrSec *)(bmsp); \
        uint64_t __bbms_addrOfBuddy = (uint64_t)(aob);      \
        uint16_t __bbms_order = (uint16_t)(order);          \
        uint8_t __bbms_magic = (uint8_t)(magic);            \
        __bbms_addrOfBuddy <<= 16;                          \
        __bbms_order <<= 8;                                 \
        __bbms_bmsp->addrOfBuddy = __bbms_addrOfBuddy;      \
        __bbms_bmsp->addrOfBuddy |= __bbms_order;           \
        __bbms_bmsp->addrOfBuddy |= __bbms_magic;           \
    } while(0)


/**
 * @brief from original value of addrOfBuddy, order, magic build new value for [_BuddyBlockMgr]
 * 
 * @param bbmp: the pointor to [_BuddyBlockMgr]
 * @param aob: orignal value of addrOfBuddy
 * @param order: original value of order
 * @param magic: original value of magic
 */
#define _initBuddyBlockMgr(bbmp, aob, order, magic)             \
    do {                                                        \
        _BuddyBlockMgr *__ibbm_bbmp = (_BuddyBlockMgr *)(bbmp); \
        _initBuddyMgrSec(__ibbm_bbmp, aob, order, magic);       \
        initDoubleListNode(&__ibbm_bbmp->dln);                  \
    } while(0)


/**
 * @brief get the pointor to [Buddy] from [_BuddyMgrSec]
 * 
 * @param bmsp: the pointor to [_BuddyMgrSec]
 * 
 * @return pointor to [Buddy]
 */
#define _getPtrOfBuddyFromBuddyMgrSec(bmsp)                             \
    ({                                                                  \
        _BuddyMgrSec *__gpobfbms_bmsp = (_BuddyMgrSec *)(bmsp);         \
        uint64_t __gpobfbms_addrOfBuddy = __gpobfbms_bmsp->addrOfBuddy; \
        __gpobfbms_addrOfBuddy >>= 16;                                  \
        (Buddy *)__gpobfbms_addrOfBuddy;                                \
    })


/**
 * @brief get order of this block from [_BuddyMgrSec]
 * 
 * @param bmsp: the pointor to [_BuddyMgrSec]
 * 
 * @return order of this block
 */
#define _getOrderFromBuddyMgrSec(bmsp)                          \
    ({                                                          \
        _BuddyMgrSec *__gofbms_bmsp = (_BuddyMgrSec *)(bmsp);   \
        uint16_t __gofbms_order = __gofbms_bmsp->order;         \
        __gofbms_order >>= 8;                                   \
        (uint8_t)__gofbms_order;                                \
    })


/**
 * @brief get magic number of this block from [_BuddyMgrSec]
 * 
 * @param bmsp: the pointor to [_BuddyMgrSec]
 * 
 * @return magic numbe of this block
 */
#define _getMagicFromBuddyMgrSec(bmsp)                          \
    ({                                                          \
        _BuddyMgrSec *__gmfbms_bmsp = (_BuddyMgrSec *)(bmsp);   \
        uint8_t __gmfbms_magic = __gmfbms_bmsp->magic;          \
        __gmfbms_magic;                                         \
    })



/**
 * @brief get [_BuddyMgrSec] from the addr alloced to caller
 * 
 * @param ptr: the address that alloced to the caller who called [buddyAlloc]
 * 
 * @return pointor to the [_BuddyMgrSec]
 */
#define _getBuddyMgrSecPtr(ptr)                                 \
    ({                                                          \
        void *__gbmsp_bmsp = (void *)(ptr);                     \
        (_BuddyMgrSec *)(__gbmsp_bmsp - sizeof(_BuddyMgrSec));  \
    })


/**
 * @brief get [_BuddyBlockMgr] from the addr alloced to caller
 * 
 * @param ptr: the address that alloced to the caller who called [buddyAlloc]
 * 
 * @return pointor to the [_BuddyBlockMgr]
 */
#define _getBuddyBlockMgrPtr(ptr)                           \
    ({                                                      \
        void *__gbbmp_bbmp = (void *)(ptr);                 \
        (_BuddyBlockMgr *)_getBuddyMgrSecPtr(__gbbmp_bbmp); \
    })


/**
 * @brief get buddy address from the addr alloced to caller
 * 
 * @param ptr: the address that alloced to the caller who called [buddyAlloc]
 * 
 * @return address of the block with type [uintptr_t]
 */
#define _getBuddyBlockAddr(ptr)                     \
    ({                                              \
        void *__gbba_bbap = (void *)(ptr);          \
        (uintptr_t)_getBuddyMgrSecPtr(__gbba_bbap); \
    })


#define _sizeByOrder(order)                         \
    ({                                              \
        uint8_t __sbo_o = (uint8_t)(order);         \
        minSizeByAlign(_BUDDY_ALIGN) << __sbo_o;    \
    })


/**
 * @brief find the buddy addr of the address
 * INFO: the address add size is the buddy's address
 * 
 * @param a: address
 * 
 * @return address of buddy if [a] is could be union and buddy is valid else NULL
 */
static inline uintptr_t _findBuddy(uintptr_t addr) {

    uintptr_t ret = 0;
    Buddy *bp = _getPtrOfBuddyFromBuddyMgrSec(addr), *buddyBp;
    uint8_t order = _getOrderFromBuddyMgrSec(addr), buddyOrder;
    size_t blockSize = _sizeByOrder(order);
    /* address of buddy */
    uintptr_t buddyAddr = addr ^ blockSize;
    uint8_t magic, buddyMagic;
    

    /* buddyAddr should not over flow 
     * INFO: order should less then [BUDDY_MAX_ORDER], if order == [BUDDY_MAX_ORDER - 1]
     *       then, the addr could not be union.
     */
    if( buddyAddr < bp->start || buddyAddr >= bp->start + bp->size || order >= BUDDY_MAX_ORDER - 1) {
        goto _done;
    }

    buddyBp = _getPtrOfBuddyFromBuddyMgrSec(buddyAddr);
    buddyOrder = _getOrderFromBuddyMgrSec(buddyAddr);
    magic = _getMagicFromBuddyMgrSec(addr);
    buddyMagic = _getMagicFromBuddyMgrSec(buddyAddr);

    /* buddy is valid for this addr */
    if(buddyBp == bp && buddyOrder == order && buddyMagic == magic && bp->magic == magic) {
        ret = buddyAddr;
    }

_done:
    return ret;
}


/**
 * @brief get order in buddy sys from the given size
 * 
 * @param size: size of memory
 * 
 * @return the order suitable for the size
 */
static inline uint8_t _orderBySize(size_t size) {
    uint64_t order, cnt,  mask = -1; /* mask = ffffffffffffffffUL */
    uint64_t buddyMinSize = minSizeByAlign(_BUDDY_ALIGN);
    
    cnt = size / buddyMinSize;
    cnt = (size % buddyMinSize) ? (cnt + 1) : cnt;

    /* if cnt == 0, order could in a undefined status.
     * INFO: but at this case, cnt at least equal 1;
     */
    asm volatile (
        "bsrq %1, %0"
        : "=a"(order)
        : "b"(cnt)
        : "cc", "memory"
    );
    mask >>= (sizeof(uint64_t) * 8 - order);
    mask = order == 0 ? 0 : mask;

    order = (cnt & mask) ? (order + 1) : (order);

    return (uint8_t )order;
}


/**
 * @brief initialize Buddy Structure with a whole memory
 * 
 * @param ptr: address of range of memory
 * @param size: size of this range of memory
 * @param magic: magic for this buddy system
 * 
 * @return pointor to [Buddy] structure on successful. NULL on failure.
 */
static inline Buddy *_initBuddyStructure(void *ptr, size_t size, uint8_t magic) {
    /* 64bytes(256bits) align, same as the cache line */
    Buddy       *bp = (Buddy *)ceilAlign(ptr, _BUDDY_ALIGN);
    uintptr_t   blockEnd = (uintptr_t )ptr + size;
    uintptr_t   buddyMemoryBegin = (uintptr_t )bp + sizeof(Buddy);
    uintptr_t   start, buddyBlobEnd;
    size_t      buddyBlobSize;
    int         i;

    if(magic == 0) {
        bp = NULL;
        goto _done;
    }

    /* memory is not enough to content whole [Buddy] structure */
    if( buddyMemoryBegin >= blockEnd) {
        bp = NULL;
        goto _done;
    }

    start = ceilAlign(buddyMemoryBegin, _BUDDY_ALIGN);

    /* no left memory for even one memory blob */
    if (start >= blockEnd || start + minSizeByAlign(_BUDDY_ALIGN) >= blockEnd) {
        bp = NULL;
        goto _done;
    }

    buddyBlobEnd = floorAlign(blockEnd, _BUDDY_ALIGN);
    buddyBlobSize = buddyBlobEnd - start;

    bp->blockStart = (uintptr_t )ptr;
    bp->blockSize = size;
    bp->start = start;
    bp->size = buddyBlobSize;
    bp->magic = magic;
    initSpinLock(&bp->lock);
    
    /* initialize double list */
    for(i = 0; i < BUDDY_MAX_ORDER; i++) {
        initDoubleList(&bp->array[i], NULL);
    }

_done:
    return bp;
}


/**
 * @brief union buddy block from order [fromOrder] to order [toOrder] (excluding toOrder)
 * 
 * @param bp: pointor to [Buddy]
 * @param fromOrder: begin order
 * @param toOrder: end order
 */
static inline void _unionBuddy(Buddy *bp, uint8_t fromOrder, uint8_t toOrder) {
    DoubleList      dlTmp;
    uint8_t         order;
    DoubleListNode  *dlnp;
    _BuddyBlockMgr  *bbmp, *buddyBbmp;

    toOrder = min(toOrder, (uint8_t)BUDDY_MAX_ORDER);

    if (fromOrder >= toOrder) {
        goto _done;
    }

    initDoubleList(&dlTmp, NULL);

    for(order = fromOrder; order < toOrder; order++) {
        while(!isDoubleListSentinel(bp->array[order].header.next)) {
            dlnp = bp->array[order].header.next;
            removeDoubleListNode(dlnp);
            bbmp = containerOf(dlnp, _BuddyBlockMgr, dln);
            buddyBbmp = (_BuddyBlockMgr *)_findBuddy((uintptr_t)bbmp);

            if (buddyBbmp) {    /* buddy is valid */
                /* find out the address that should insert into [order + 1] double list */
                removeDoubleListNode(&buddyBbmp->dln);
                bbmp = (_BuddyBlockMgr *)min((uintptr_t)bbmp, (uintptr_t)buddyBbmp);
                /* reinitialize [_BuddyBlockMgr] */
                _initBuddyBlockMgr(bbmp, bp, order+1, bp->magic);
                /* insert buddy block into [order + 1] double list */
                addNodeAtDoubleListHeader(&bp->array[order+1], &bbmp->dln);
            } else { /* buddy is unvalid */
                /* insert buddy block into tmp double list [dlTmp] */
                addNodeAtDoubleListHeader(&dlTmp, &bbmp->dln);
            }
        }
        /* bring the nodes of [bbmp] no buddy back to the [order] double list */
        while(!isDoubleListSentinel(dlTmp.header.next)) {
            dlnp = dlTmp.header.next;
            removeDoubleListNode(dlnp);
            addNodeAtDoubleListHeader(&bp->array[order], dlnp);
        }
    }

_done:
    return;
}


/**
 * @brief splite a buddy block
 * 
 * @param bp: pointor to [Buddy]
 * @param order: split [order] into [order - 1]
 */
static inline void _splitBuddyBlock(Buddy *bp, uint8_t order) {
    DoubleListNode  *dlnp;
    _BuddyBlockMgr  *bbmp, *buddyBbmp;
    size_t          newBlockSize;

    /* can not split */
    if (order == 0 || order >= BUDDY_MAX_ORDER || bp->array[order].nodeCnt == 0) {
        goto _done;
    }

    dlnp = bp->array[order].header.next;
    removeDoubleListNode(dlnp);
    bbmp = containerOf(dlnp, _BuddyBlockMgr, dln);
    
    /* find buddy block address */
    newBlockSize = _sizeByOrder(order-1);
    buddyBbmp = (_BuddyBlockMgr *)((uintptr_t)bbmp ^ newBlockSize);

    /* reinitialize _BuddyBlockMgr and insert into [Buddy] with [order - 1] */
    _initBuddyBlockMgr(bbmp, bp, order-1, bp->magic);
    _initBuddyBlockMgr(buddyBbmp, bp, order-1, bp->magic);
    addNodeAtDoubleListHeader(&bp->array[order-1], &bbmp->dln);
    addNodeAtDoubleListHeader(&bp->array[order-1], &buddyBbmp->dln);

_done:
    return;
}


/**
 * @brief split buddy from order [fromOrder] to order [toOrder]
 * INFO: [fromOrder] must greater than [toOrder]
 * 
 * @param bp: pointor to [Buddy]
 * @param fromOrder: from order
 * @param toOrder: to order
 */
static inline void _splitBuddy(Buddy *bp, uint8_t fromOrder, uint8_t toOrder) {
    uint8_t order;

    if (fromOrder >= BUDDY_MAX_ORDER || toOrder >= fromOrder || bp->array[fromOrder].nodeCnt == 0) {
        goto _done;
    }

    for(order = fromOrder; order > toOrder; order--) {
        _splitBuddyBlock(bp, order);
    }

_done:
    return;
}



Buddy *initBuddy(void *ptr, size_t size, uint8_t magic) {
    Buddy   *bp;
    size_t  minBuddyBlockSize;
    uintptr_t addr, end;
    _BuddyBlockMgr *bbmp;

    bp = _initBuddyStructure(ptr, size, magic);

    ckpvThenReturn(bp, NULL, NULL);

    minBuddyBlockSize = minSizeByAlign(_BUDDY_ALIGN);
    end = bp->start + bp->size;


    for(addr = bp->start; addr < end; addr += minBuddyBlockSize) {
        bbmp = (_BuddyBlockMgr *)addr;
        _initBuddyBlockMgr(bbmp, bp, 0, magic);
        addNodeAtDoubleListHeader(&bp->array[0], &bbmp->dln);
    }

    _unionBuddy(bp, 0, BUDDY_MAX_ORDER);

    return bp;
}


void *buddyAlloc(Buddy *bp, size_t size) {
    void *ret = NULL;
    uint8_t order = _orderBySize(size + sizeof(_BuddyMgrSec)), fromOrder;
    DoubleListNode *dlnp;
    uintptr_t  addr;

    if(order >= BUDDY_MAX_ORDER) {
        goto _exit;
    }

    lockSpinLock(&bp->lock);

    if(bp->array[order].nodeCnt == 0) {

        /* try union block to order */
        _unionBuddy(bp, 0, order);
        if(bp->array[order].nodeCnt != 0) {
            goto _alloc;
        }

        /* union does not works, and then try to split block to get memory for [order] */
        fromOrder = order + 1;
        while(fromOrder < BUDDY_MAX_ORDER && bp->array[fromOrder].nodeCnt == 0) {
            fromOrder++;
        }
        _splitBuddy(bp, fromOrder, order);
        if(bp->array[order].nodeCnt == 0) { /* no memory left */
            goto _done;
        }
    }

_alloc:
    dlnp = bp->array[order].header.next;
    removeDoubleListNode(dlnp);
    addr = (uintptr_t )containerOf(dlnp, _BuddyBlockMgr, dln);
    ret = (void *)(addr + sizeof(_BuddyMgrSec));
    bzero(addr, _sizeByOrder(order));
    /* set magic to [0], means this block is in using */
    _initBuddyMgrSec(addr, bp, order, 0);

_done:
    unlockSpinLock(&bp->lock);

_exit:
    return ret;
}


void buddyFree(void *ptr) {
    if(!ptr) {
        goto _exit;
    }

    _BuddyBlockMgr  *bbmp = _getBuddyBlockMgrPtr(ptr);
    _BuddyMgrSec    *bbsp = (_BuddyMgrSec *)bbmp;
    Buddy           *bp = _getPtrOfBuddyFromBuddyMgrSec(bbsp);
    uint8_t         order = _getOrderFromBuddyMgrSec(bbsp);
    uint8_t         magic = _getMagicFromBuddyMgrSec(bbsp);

    lockSpinLock(&bp->lock);
    if (magic != 0) {
        goto _done;
    }
    _initBuddyBlockMgr(bbmp, bp, order, bp->magic);
    addNodeAtDoubleListHeader(&bp->array[order], &bbmp->dln);

_done:
    unlockSpinLock(&bp->lock);
_exit:
    return;
}

