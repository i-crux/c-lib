#ifndef _HASH_H_
#define _HASH_H_
#include "doubleList.h"

#include <stdint.h>

typedef uintptr_t (*hashFunc)(uintptr_t);
typedef int (*hashCmpKey)(uintptr_t, uintptr_t);

typedef struct
{
    DoubleListNode dln;
    uintptr_t key;
    size_t dataSize;
    uint8_t data[1];
} HashNode;

typedef struct
{
    hashFunc hash;
    hashCmpKey cmp;
    int slotCnt;
    DoubleList *slot;
} Hash;

/**
 * @brief create a [Hash] structure
 *
 * @param hashFunc: hash function
 * @param cmp: compare function for key
 * @param slotCnt: count of slot for [HashNode]
 * @param dlnFunc: function for free double list node
 *
 * @return pointor to [Hash] on successful. [NULL] on failure
 */
Hash *hashCreate(hashFunc hashFunc, hashCmpKey cmp, int slotCnt, doubleListNodeFree dlnFunc);

/**
 * @brief create a [HashNode]
 *
 * @param key: key for hash
 * @param dataSize: size of data
 * @param data: pointor to data
 *
 * @return pointor to [HashNode] on successful. [NULL] on failure
 */
HashNode *hashNodeCreate(uintptr_t key, size_t dataSize, void *data);

/**
 * @brief search a [key] in a [Hash]
 *
 * @param h: pointor to a [Hash]
 * @param key: key to be added
 *
 * @return pointor to [HashNode] if [key] exists, [NULL] otherwise
 */
HashNode *hashSearch(Hash *h, uintptr_t key);

/**
 * @brief add a key in a hash
 *
 * @param h: pointor to a [Hash]
 * @param key: key to be added
 * @param dataSize: size of [data] to be added(if data is [NULL], no meaning for this arg)
 * @param data: data to be added
 *
 * @return 0 when success, 1 for duplicated key, -1 error
 */
int hashAdd(Hash *h, uintptr_t key, size_t dataSize, void *data);

/**
 * @brief delete a [key] in a [Hash]
 *
 * @param h: pointor to a [Hash]
 * @param key: key to be added
 *
 * @return pointor to [HashNode] if [key] exists, [NULL] otherwise
 */
HashNode *hashDelete(Hash *h, uintptr_t key);

/**
 * @brief Destory a [Hash] structure
 *
 * @param h: pointor to a [Hash]
 */
void hashDestory(Hash *h);

/**
 * @brief hash a key with [salt]
 *
 * @param salt: salt to hash a [key]
 * @param key: which needed be hashed
 *
 * @return value of hash
 */
#define __hash(salt, key)                        \
    ({                                           \
        uintptr_t __dh_salt = (uintptr_t)(salt); \
        uintptr_t __dh_key = (uintptr_t)(key);   \
        __dh_salt * 31 + __dh_key;               \
    })

/**
 * @brief get a [str(terminated by '\0')]'s hash value
 *
 * @param str: pointor to a string
 *
 * @return hash value of string
 */
uintptr_t defaultHashStr(uintptr_t str);

/**
 * @brief get a [num]'s hash value
 *
 * @param num: number which needed be hashed. (max 8bytes(x64), 4bytes(x86))
 *
 * @return hash value of num
 */
uintptr_t defaultHashNum(uintptr_t num);

#endif /* _HASH_H_ */