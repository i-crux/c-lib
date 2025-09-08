#ifndef _BIN_TREE_H_
#define _BIN_TREE_H_

#include <stdint.h>
#include <stdlib.h>

#include "doubleList.h"

typedef int (*binTreeElemCmp)(const void *a, const void *b);
typedef struct binTreeNodeStruct BinTreeNode;
typedef void (*binTreeNodeFreeKey)(void *key);
typedef void (*binTreeNodeVisit)(BinTreeNode *binTreeNode);

struct binTreeNodeStruct
{
    BinTreeNode         *parent;
    BinTreeNode         *left;
    BinTreeNode         *right;
    uintptr_t           property;       /* some property for the TreeNode, could using for RBtree, AVLtree */
    DoubleList          dlist;          /* data list, deal with same key */
    binTreeNodeFreeKey  freeKey;
    /* size_t              keySize; */       /* size of key */
    uint8_t             key[1];
};

typedef struct {
    DoubleListNode  dln;
    /* size_t          dataSize; */
    uint8_t         data[1];
} BinTreeNodeData;

typedef struct BinTreeStruct {
    BinTreeNode     *root;
    binTreeElemCmp  cmp;            /* compare function for key, NULLable */
    int             size;           /* size of this tree */
    bool            allowSameKey;   /* allow same key or not */
} BinTree;


/**
 * @brief initialize a binary tree, memory of [binTree] is managed by caller

 * @param binTree: pointor of [BinTree]
 * @param cmp: compare function for key, could be null
 * @param freeData: function for free data memory, could be null
 * @param allowSameKey: allow same key or not
 */
void binTreeInit(BinTree *binTree, binTreeElemCmp cmp, bool allowSameKey);


/**
 * @brief create a binary tree

 * @param cmp: compare function for key, could be null
 * @param freeData: function for free data memory, could be null
 * @param allowSameKey: allow same key or not
 * 
 * @return pointor to [BinTree] on successful, [NULL] on failure
 */
BinTree *binTreeCreate(binTreeElemCmp cmp, bool allowSameKey);


/**
 * @brief create a [BinTreeNodeData]
 * 
 * @param dataSize: size of data;
 * @param data: pointor to data
 * @param deNode: function for free double list node
 * 
 * @return pointor to [BinTreeNode] on successful, [NULL] on failure
 */
BinTreeNodeData *binTreeNodeDataCreate(size_t dataSize, void *data);

/**
 * @brief create a [BinTreeNode]
 * 
 * @param property: property of the binary tree node
 * @param keySize: size of key;
 * @param key: pointor to key
 * 
 * @return pointor to [BinTreeNode] on successful, [NULL] on failure
 */
BinTreeNode *binTreeNodeCreate(uintptr_t property, size_t keySize, void *key, 
                               doubleListNodeFree deNode, binTreeNodeFreeKey freeKey);


/**
 * @brief add data to a [BinTreeNode]
 * 
 * @param btn: pointor to a [BinTreeNode]
 * @param dataSize: size of data;
 * @param data: pointor to data
 * 
 * @return 1 on add successful; 0 on failure
 */
bool binTreeNodeAddData(BinTreeNode *btn, size_t dataSize, void *data);

/**
 * @brief traval a binary tree in preorder
 * 
 * @param binTree: pointor of [BinTree]
 * @param binTreeVisit: function for visiting [BinTreeNode]
 * 
 * @return 1 on successful, 0 on failure;
 */
bool binTreeTravalPreorder(BinTree *binTree, binTreeNodeVisit binTreeNodeVisit);


/**
 * @brief traval a binary tree in inorder
 * 
 * @param binTree: pointor of [BinTree]
 * @param binTreeVisit: function for visiting [BinTreeNode]
 * 
 * @return 1 on successful, 0 on failure;
 */
bool binTreeTravalInorder(BinTree *binTree, binTreeNodeVisit binTreeNodeVisit);


/**
 * @brief traval a binary tree in Postorder
 * 
 * @param binTree: pointor of [BinTree]
 * @param binTreeVisit: function for visiting [BinTreeNode]
 * 
 * @return 1 on successful, 0 on failure;
 */
bool binTreeTravalPostorder(BinTree *binTree, binTreeNodeVisit binTreeNodeVisit);


/**
 * @brief traval a binary tree in Levelorder
 * 
 * @param binTree: pointor of [BinTree]
 * @param binTreeVisit: function for visiting [BinTreeNode]
 * 
 * @return 1 on successful, 0 on failure;
 */
bool binTreeTravalLevelorder(BinTree *binTree, binTreeNodeVisit binTreeNodeVisit);


/**
 * @brief destory a [BinTree]
 * 
 * @param binTree: pointor to [binTree]
 * @param freeSelf: free [binTree] or not
 */
void binTreeDestory(BinTree *binTree, bool freeSelf);
#endif /* _BIN_TREE_H_ */