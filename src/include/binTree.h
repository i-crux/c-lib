#ifndef _BIN_TREE_H_
#define _BIN_TREE_H_

#include <stdint.h>
#include <stdlib.h>

typedef int (*binTreeElemCmp)(const void *a, const void *b);
typedef void (*binTreeDateFree)(void *data);
typedef struct binTreeNodeStruct BinTreeNode;

struct binTreeNodeStruct
{
    BinTreeNode     *parent;
    BinTreeNode     *left;
    BinTreeNode     *right;
    uintptr_t       property;       /* some property for the TreeNode, could using for RBtree, AVLtree */
    void            *data;          /* pointor to data */
    uint8_t         key[1];
};

typedef struct BinTreeStruct {
    BinTreeNode     *root;
    binTreeElemCmp  cmp;        /* compare function for key, NULLable */
    binTreeDateFree freeData;   /* free data pointor, NULLable */
    int             size;       /* size of this tree */
} BinTree;

typedef void (*binTreeVisit)(BinTreeNode *binTreeNode);


/**
 * @brief initialize a binary tree, memory of [binTree] is managed by caller

 * @param binTree: pointor of [BinTree]
 * @param cmp: compare function for key, could be null
 * @param freeData: function for free data memory, could be null
 */
void binTreeInit(BinTree *binTree, binTreeElemCmp cmp, binTreeDateFree freeData);


/**
 * @brief create a binary tree

 * @param cmp: compare function for key, could be null
 * @param freeData: function for free data memory, could be null
 * 
 * @return pointor to [BinTree] on successful, [NULL] on failure
 */
BinTree *binTreeCreate(binTreeElemCmp cmp, binTreeDateFree freeData);


#if 0
/**
 * @brief traval a binary tree in preorder
 * 
 * @param binTree: pointor of [BinTree]
 * @param binTreeVisit: function for visiting [BinTreeNode]
 */
void binTreeTravalPreorder(BinTree *binTree, binTreeVisit binTreeVisit);


/**
 * @brief traval a binary tree in inorder
 * 
 * @param binTree: pointor of [BinTree]
 * @param binTreeVisit: function for visiting [BinTreeNode]
 */
void binTreeTravalInorder(BinTree *binTree, binTreeVisit binTreeVisit);


/**
 * @brief traval a binary tree in Postorder
 * 
 * @param binTree: pointor of [BinTree]
 * @param binTreeVisit: function for visiting [BinTreeNode]
 */
void binTreeTravalPostorder(BinTree *binTree, binTreeVisit binTreeVisit);


/**
 * @brief traval a binary tree in Levelorder
 * 
 * @param binTree: pointor of [BinTree]
 * @param binTreeVisit: function for visiting [BinTreeNode]
 */
void binTreeTravalLevelorder(BinTree *binTree, binTreeVisit binTreeVisit);
#endif /* 0 */

#endif /* _BIN_TREE_H_ */