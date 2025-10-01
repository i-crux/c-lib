#ifndef _RB_TREE_H_
#define _RB_TREE_H_

#include "binTree.h"
#include "bst.h"

#define RBT_BLACK 0
#define RBT_RED 1

typedef BinTreeNode RBTreeNode;
typedef BinTreeNodeData RBTreeNodeData;
typedef BinTree RBTree;

#define rbtnColor(rbtn)                                    \
    ({                                                     \
        RBTreeNode *__rc_rbtn = (rbtn);                    \
        ((__rc_rbtn) ? (__rc_rbtn)->property : RBT_BLACK); \
    })

#define rbTreeInit binTreeInit
#define rbTreeCreate binTreeCreate

#define rbTreeNodeDataCreate binTreeNodeDataCreate
#define rbTreeNodeCreate(keySize, key, deNode, freeKey) \
    binTreeNodeCreate(RBT_RED, keySize, key, deNode, freeKey)
#define rbTreeNodeAddData binTreeNodeAddData
#define rbTreeSearch bstSearch

/**
 * @brief insert a node into a [RBTree]
 * NOTE: if inserting on error or duplicated key,
 *       the caller should deal with the memory of [key] and [data] proberly,
 *       if these memory come from heap
 *
 * @param rbt: pointor to a [RBTree]
 * @param rbtn: pointor to a [RBTreeNode]
 *
 * @return 0 on successful, 1 on duplicated key, -1 error
 */
int rbTreeInsert(RBTree *rbt, RBTreeNode *rbtn);

/**
 * @brief delete a [key] from a [RBTree]
 *
 * @param rbt: pointor to a [RBTree]
 * @param key: pontor to key
 *
 * @return pointor to a [RBTreeNode] if [key] exsits, otherwise [NULL]
 */
RBTreeNode *rbTreeDelete(RBTree *rbt, void *key);

#endif /* _RB_TREE_H_ */