#ifndef _AVL_TREE_H_
#define _AVL_TREE_H_

#include "binTree.h"
#include "bst.h"

/**
 * ! when initialize a [AVLTree], [binTreeElemCmp] must be provided
 */

// typedef enum
// {
//     leftHeight = -1,
//     sameHeight = 0,
//     rightHeight = 1,
// } AVLBalanceFactor;

#define AVLTree BinTree
/*
 * [property] is means: height of tree
 */
#define AVLTreeNode BinTreeNode
#define AVLTreeNodeData BinTreeNodeData

#define avlTreeInit binTreeInit
#define avlTreeCreate binTreeCreate
#define avlTreeNodeDataCreate binTreeNodeDataCreate
#define avlTreeNodeCreate(keySize, key, deNode, freeKey) \
    binTreeNodeCreate(0, keySize, key, deNode, freeKey)
#define avlTreeNodeAddData binTreeNodeAddData
#define avlTreeSearch bstSearch

/**
 * @brief insert a node into a [AVLTree]
 * NOTE: if inserting on error or duplicated key,
 *       the caller should deal with the memory of [key] and [data] proberly,
 *       if these memory come from heap
 *
 * @param avlt: pointor to a [AVLTree]
 * @param avltn: pointor to a [AVLTreeNode]
 *
 * @return 0 on successful, 1 on duplicated key, -1 error
 */
int avlTreeInsert(AVLTree *avlt, AVLTreeNode *avltn);

/**
 * @brief delete a [key] from a [AVLTree]
 *
 * @param avlt: pointor to a [AVLTree]
 * @param key: pontor to key
 *
 * @return pointor to a [AVLTreeNode] if [key] exsits, otherwise [NULL]
 */
AVLTreeNode *avlTreeDelete(AVLTree *avlt, void *key);

#endif /* _AVL_TREE_H_ */