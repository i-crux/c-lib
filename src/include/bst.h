#ifndef _BST_H_
#define _BST_H_

#include "binTree.h"

/**
 * ! when initialize a [BST], [binTreeElemCmp] must be provided
 */

/**
 * @brief insert a node into a [BST]
 * NOTE: if inserting on error or duplicated key,
 *       the caller should deal with the memory of [key] and [data] proberly,
 *       if these memory come from heap
 *
 * @param bt: pointor to a [BinTree]
 * @param btn: pointor to a [BinTreeNode]
 *
 * @return 0 on successful, 1 on duplicated key, -1 error
 */
int bstInsert(BinTree *bt, BinTreeNode *btn);

/**
 * @brief search [key] a from a [BST]
 *
 * @param bt: pointor to a [BinTree]
 * @param key: pontor to key
 *
 * @return pointor to a [BinTreeNode] if [key] exsits, otherwise [NULL]
 */
BinTreeNode *bstSearch(BinTree *bt, void *key);

/**
 * @brief delete a [key] from a [BST]
 *
 * @param bt: pointor to a [BinTree]
 * @param key: pontor to key
 *
 * @return pointor to a [BinTreeNode] if [key] exsits, otherwise [NULL]
 */
BinTreeNode *bstDelete(BinTree *bt, void *key);

#endif /* _BST_H_ */