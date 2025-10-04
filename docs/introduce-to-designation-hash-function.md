# Integer

* **Small-range positive integers:** Use directly.
* **Small-range negative integers:** Apply an offset.
  Example: `-100 ~ 100` → `0 ~ 200`
* **Large integers:**

  * Apply modulo with a prime number.
    Reference for good primes: [PlanetMath - Good Hashtable Primes](https://planetmath.org/goodhashtableprimes)

# Floating-point Numbers

In computers, floats are represented as 32-bit or 64-bit binary numbers. To hash, first convert them to integers.

# Strings

Convert strings into integers:

```
code = c * 26^3 + o * 26^2 + d * 26^1 + e * 26^0
```

Or consider it as a number in base `B`:

```
code = c * B^3 + o * B^2 + d * B^1 + e * B^0
```

Hash function:

```
hash(code) = (c * B^3 + o * B^2 + d * B + e) % M
```

Equivalently:

```
hash(code) = ((((c * B) + o) * B + d) * B + e) % M
```

Or applying modulo at each step for efficiency:

```
hash(code) = ((((c % M) * B + o) % M * B + d) % M * B + e) % M
```

# Composite Types

Convert composite data types to integers. Example:

```
Data: year, month, day

hash(Data) = (((data.year % M) * B + data.month) % M * B + data.day) % M
```

# Principles

1. **Consistency:** If `a == b`, then `hash(a) == hash(b)`.
2. **Efficiency:** Hash computation should be fast and simple.
3. **Uniformity:** Hash values should be evenly distributed.
