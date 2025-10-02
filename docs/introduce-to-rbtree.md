# Properties
1. Each node is either red or black.
2. The root is black.
3. Each leaf node (nil[T], the sentinel) is black.
4. If a node is red, then both of its children are black.
5. For each node, all paths from that node to descendant leaves contain the same number of black nodes (i.e., the black-height is the same).

**The height of a red-black tree is at most `2 * log(n+1).`**

# structure of Red-Black Tree
> pseudocode
```c
    struct RBTree       // Red-Black tree data structure
        color ;         // Color: R = red, B = black
        key ;           // Key
        right ;         // Right child
        left ;          // Left child
        parent ;        // Parent
```

# Binary Tree Rotations
```
        |                                 |
        X         LeftRotate              Y
      /   \      --------------->       /   \
     a     Y    <---------------       X     c
         /   \    RightRotate        /   \
        b     c                     a     b
```

> pseudocode
```c
    LeftRotate( T, x )      // Left rotation (requires right subtree)
        y = right[x] ;
        right[x] = left[y] ;
        left[y] && parent[left[y]] = x ;

        if parent[x]        // x is not the root
            left[parent[x]] == x and left[parent[x]] = y \
            or right[parent[x]] = y ;
        else
            root[T] = y ;

        parent[y] = parent[x] ;
        left[y] = x ;
        parent[x] = y ;

    /*
    Right rotation is symmetric to left rotation
    */
```

# Red-Black Tree Insertion
> pseudocode
```c
    RBTreeInsert( T, z )        // First, set color[z] = red
        y = nil[T] ;            // Insert z as in a normal BST
        x = root[T] ;           // Finally call RBTreeInsertFixup to restore RB-tree properties

        while x != nil[T]
            y = x ;
            if key[x] > key[z]
                x = left[x] ;
            else
                x = right[x] ;

        if y = nil[T]
            root[T] = z ;
        else if key[y] > key[z]
            left[y] = z ;
        else
            right[y] = z ;

        parent[z] = y ;
        left[z] = nil[T] ;
        right[z] = nil[T] ;
        color[z] = red ;

        RBTreeInsertFixup( T, z ) ;
```

## Fixing Properties after Insertion
> `B = black node, R = red node`
* A red-black tree can be viewed as a 2-3-4 B-tree.

```
        B
      /   \      A full 2-3-4 tree node,
     R     R     black nodes represent 2-3-4 tree nodes
    / \   / \
```

### case1
```
            1B
          /    \       # 3R is the newly inserted node
         2R      B     # 1B and 2R together form a non-full 2-3-4 node
        /  \    / \    # Insert 3R directly
       3R   a  b   c
      /  \
     d    e

             |
             |
             V

            2B           # After right rotation and recoloring
          /    \         # 3R merges into the 2-3-4 balanced node
         3R    1R        # 2R becomes 2B, 1B becomes 1R
        /  \  /  \
       d   e  a   B
                 / \
                b   c

```

### case2
```
            1B
          /    \         # 3R is newly inserted   
         2R     B        # Convert to Case 1
        /  \   / \       # By left-rotating at 2R
       a   3R b   c     
          /  \
         d    e

             |
             |
             V

            1B
          /    \      
         3R      B    
        /  \    / \   
       2R   e  b   c
      /  \
     a    d

```

### case3
```
        1B       # 3R is the newly inserted node
      /    \     # Regardless of whether it is left or right child
     2R    4R    # Recolor: 1B → 1R, 2R and 4R → 2B and 4B
     |           # Then point 3R upward and rebalance
     3R

        |
        |
        V

        1R      
      /    \     # Now the black-height of the subtree increases by 1
     2B    4B    # Corresponds to height+1 in 2-3-4 tree
     |      
     3R

```

### Implementation

> pseudocode
```c
    RBTreeInsertFixup( T, z )
        while color[parent[z]] == red
            if left[parent[parent[z]]] == parent[z]     // Parent is a left child
                y = right[parent[parent[z]]] ;

                if color[y] == red                          // Case 3
                    color[parent[z]] = black ;
                    color[parent[parent[z]]] = red ;
                    color[y] = black ;
                    z = parent[parent[z]] ;
                else
                    if right[parent[z]] == z                // Case 2
                        z = parent[z] ;
                        LeftRotate( T, z ) ;
                    color[parent[z]] = black ;              // Case 1
                    color[parent[parent[z]]] = red ;
                    RightRotate( T, parent[parent[z]] ) ;
            else                                         // Symmetric for right child
                ...

        color[root[T]] = black ;

```


# Red-Black Tree Deletion

- First, delete the node as in a standard BST.
- If the deleted node was black, call `RBTreeDeleteFixup` to restore RB-tree properties.

> pseudocode
```c
    RBTreeDelete( T, z )
    // Standard BST deletion
        if left[z] == nil[T] or right[z] == nil[T]
            y = z ;
        else
            y = BinTreeSuccessor( z ) ;

        if left[y] != nil[T]
            x = left[y] ;
        else
            x = right[y] ;

        if x != nil[T]
            parent[x] = parent[y] ;

        if parent[p] != nil[T]
            if left[parent[y]] == y
                left[parent[y]] = x ;
            else
                right[parent[y]] = x ;
        else
            root[T] = x ;

        if y != z
            copy y to z except( left, right, parent ) ;
        // End of standard BST deletion

        if color[y] == black 
            RBTreeDeleteFixup( T, x ) ;

        return y ; 
```

## Fixing Properties after Deletion
**Only when the deleted node is black do we need to rebalance.**

```
        |
        R       # x's parent is red, i.e., original y’s parent
      /   \     # Then [a] must not be nil[T], and must be black, since [y] was black
     x     a
    / \
   b   c
```

### Case 1 (x is red):
```
      y       # From the 2-3-4 tree perspective, the deleted y had a red child
    /        
   x(R)       # Just recolor x to black, done

```

### Case 2 (x is black):
When parent’s element is not full (in 2-3-4 tree) and sibling has only one element
```
        1(B)        # z cannot be nil[T]
       /    \       # Merge z with parent
      x      z(B)   # Tree height decreases by 1

```

#### When sibling has 2+ elements
```
       ... F(B) ...             # ... may have red elements or none    
        /        \              # Move one element from sibling up to parent
       x        1 - 2 ...       # Parent splits one element down into sibling
     /   \     /  |  \          # x becomes child of the new node
    a     b   c   d   ?

            |
            |
            V

       ... 1(B) ...             
        /        \              
       F(B)      2 ...
     /   \     /  | 
    x     c   d   ?   
   / \
  a   b

```

#### When sibling has one element but parent has multiple elements
```
            F1 - F2 ...         # Split one element from parent
          /    |    \           # Merge with sibling
         x     B     a          # x becomes child of the new merged node
        / \   / \
       c   d e   f

                |
                |
                V

                 F2 ...    
               /    \      
            F1- B    a     
           /  |  \
          x   e   f
         / \  
        c   d

```

### Implementation

> pseudocode
```c
    RBTreeDeleteFixup( T, x )
        while x != root[T] and color[x] == black
            if x == left[parent[x]]
                w = right[parent[x]] ;
                
                if color[w] == red          // Parent has 2 elements
                    color[w] = black ;      
                    color[parent[x]] = red ;
                    LeftRotate( T, parent[x] ) ;
                    w = right[parent[x]] ;

                if color[left[w]] == black and color[right[w]] == black
                // Sibling has only one element → merge with parent
                    color[w] = red ;
                    x = parent[x] ;
                else
                // Sibling has multiple elements
                    if color[right[w]] == black
                        color[left[w]] = black ;
                        color[w] = red ;
                        RightRotate( T, w ) ;
                        w = right[parent[x]] ;

                    color[w] = color[parent[x]] ;
                    color[parent[x]] = black ;
                    color[right[x]] = black ;
                    LeftRotate( T, parent[x] ) ;
                    x = root[T] ;

            else // Symmetric for right child
                ...

        color[x] = black ;
```
