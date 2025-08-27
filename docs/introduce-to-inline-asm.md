# Tow Forms in GCC/Clang

## basic inline asm

```c
asm("nop");

__asm__("nop");
```

this just inserts an instruction into the code stream. the compiler doesn't know what it does, so it can cause optimization/reordering issues if used carelessly.

## extended inline asm
```c
asm volatile (
    "assembly template"
    : output Constraints
    : input Constraints
    : clobber which register or memory has been changed
);
```

Example
```c
int a = 10, b;

asm volatile (
    "movl %1, %0\n\t"
    "addl $5, %0\n"
    : (=r)(b)       /* output operand (register) */
    : "r"(a)        /* input operand (register) */
    : "memory"      /* clobbers, memory would be changed */
);
```
- `movl %1, %0`: `%1` is the input(`a`), `%0` is the output(`b`)
- `=r`: output into a register
- `r`: input in a register
- `memory`: tells the compiler this asm may read/write memory.

result: `b = a + 5`

## operand Constraints
constraints guide the compiler in choosing registers or memory locations:
- `r`: register
- `m`: memory
- `i`: immediate constant
- `=r`: output in register
- `+r`: read/write register
- `"0"`: using the same register as operand 0

example
```c
: "=r"(x)       // output in register
: "0"(1)        // input: constant 1, reuse same register as output
```

## clobbers
clobbers tell the compiler what the asm may modify beyond inputs/outputs:
- `"memory"`: the asm touches memory, so dont optimize loads/stores around it.
- `"cc"`: modifies condition codes(flags).
- "eax","ebx": specific registers that are overwritten.

example:
```c
asm volatile("addl $1, %0": "+r"(x) : : "cc");
```

## Classic Example: Atomic Increment
```c
void atomic_inc(int *ptr) {
    asm volatile (
        "lock; incl %0"
        : "+m"(*ptr)        /* input/output: memory */
        :
        : "cc", "memory"    /* clobbers */
    )
}
```

