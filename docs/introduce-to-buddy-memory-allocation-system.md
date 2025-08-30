The Buddy System is a memory allocation and management algorithm widely used in operating systems and low-level memory managers. Its main goal is to efficiently handle dynamic memory allocation while keeping fragmentation under control and ensuring fast allocation/deallocation.

The idea was first introduced by Knowlton (1965), and it is still used in kernel memory allocators today (for example, in the Linux kernel buddy allocator).

# how it works
1. Memory Partitioning
  - The entire available memory (say 2^N bytes) is managed as one large block.
  - When a memory request arrives, the system finds the smallest free block that can satisfy the request.
  - If the free block is larger than needed, it is repeatedly split into two equal halves (called buddies) until the smallest sufficient block is obtained.
2. Buddy Blocks
  - Each block of memory has exactly one unique buddy.
  - Two blocks are considered buddies if:
    - They are of the same size.
    - Their addresses differ only in the bit corresponding to their size (i.e., their addresses are identical except for one bit).
  - Example:
    - Block size = 2^k
    - Block starting addresses are aligned to multiples of 2^k.
    - Two blocks are buddies if their starting addresses differ by exactly 2^k.
3. Deallocation and Merging
  - When a block is freed, the system checks whether its buddy is also free.
  - If both are free, they are merged into a single block of double size.
  - This merging continues recursively up the hierarchy until:
    - The buddy is not free, or
    - The maximum block size is reached.

# Merge (Coalescing) Conditions
Two blocks can be merged into a larger block if all of the following conditions hold:
1. Same Size Condition
  - Both blocks must have the same size (2^k).
  - Buddy Relationship Condition
    - The two blocks must be buddies:
      - Their starting addresses differ only by 2^k.
      - Formally:
```java
addr_A XOR addr_B = 2^k
```
> where addr_A and addr_B are the starting addresses of the two blocks, and 2^k is the block size.

3. Free Status Condition
  - Both blocks must be free (not allocated to any process).

If all three conditions are satisfied, the two blocks are merged into a larger block of size `2^(k+1)`.

# Example
- Suppose total memory = 1024 bytes.
- Request: 100 bytes.
  - Rounded up to nearest power of two = 128 bytes.
  - System splits 1024 → 512 → 256 → 128.
  - Allocates one 128 block.
- Later, if the allocated 128 block is freed:
  - The allocator checks its buddy (another 128 block).
  - If the buddy is also free, they merge into 256.
  - If the new 256’s buddy is also free, they merge into 512, and so on.

# Advantages
- Fast allocation/deallocation: Splitting and merging is efficient with bitwise operations.
- Low external fragmentation: Free space can be coalesced quickly.
- Predictable structure: Easy to implement with free lists indexed by block size.

# Disadvantages
- Internal fragmentation: Requests are rounded up to the nearest power of two, which wastes memory.
- Buddy constraint: Sometimes blocks cannot merge because their buddies are still allocated.