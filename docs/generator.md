# gen.h

Describe a test in C++, press Cmd+B, and `input.txt` holds it. Built for the two tests samples never cover: the max-size test that decides TLE, and thousands of tiny tests that find the edge case behind a WA on test 2.

```cpp
#include <bits/stdc++.h>
#include "gen.h"

int main() {
    arrays(10, 200000, 1, 1e9);
}
```

Name the file `gen*.cpp` (`gen.cpp`, `gen2.cpp`, ...). cprun recognises it whatever variant is selected, runs it, and replaces `input.txt` with what it printed.

- **Your sample survives.** The previous `input.txt` is saved as `input.prev.txt`.
- **Failures change nothing.** If the generator crashes or asserts (say, more edges than a simple graph can hold), `input.txt` is left as it was and the panel shows why.
- **Reproducible.** The panel shows each run's seed; `seed(42);` at the start of `main` gives the same test every time.

## Whole inputs

One call prints a complete input. `/` means a new line.

- `t` is the number of tests, printed first. `t = 0` means the problem has no test-count line.
- `n`, `m`, `q` are totals over all tests, the statement's "sum of n over all tests ≤ 2·10⁵". They are split randomly between tests.

| Call | Prints |
| --- | --- |
| `arrays(t, n, lo, hi)` | n / a |
| `arrays_k(t, n, lo, hi, kmax)` | n k / a (`kmax = 0` means k ≤ n) |
| `two_arrays(t, n, lo, hi)` | n / a / b |
| `permutations(t, n)` | n / p |
| `strings(t, n, "ab")` | n / s |
| `bare_strings(t, n, "ab")` | s |
| `string_pairs(t, n, "ab")` | n / s / t |
| `numbers(t, lo, hi)` | x |
| `number_pairs(t, lo, hi)` | a b |
| `segments(t, n, lo, hi)` | n / n lines `l r`, l ≤ r |
| `points(t, n, lo, hi)` | n / n lines `x y` |
| `trees(t, n, shape)` | n / n−1 lines `u v`; shape `"random"`, `"line"` or `"star"` |
| `parent_trees(t, n)` | n / p₂ … pₙ with pᵢ < i |
| `weighted_trees(t, n, wlo, whi)` | n / n−1 lines `u v w` |
| `graphs(t, n, m)` | n m / m lines `u v`: connected, no self-loops or repeated edges |
| `weighted_graphs(t, n, m, wlo, whi)` | n m / m lines `u v w` |
| `grids(t, n, m, ".#")` | n m / n rows (every test n × m) |
| `matrices(t, n, m, lo, hi)` | n m / n rows of m numbers |
| `range_queries(t, n, q, lo, hi)` | n q / a / q lines `l r` |
| `update_queries(t, n, q, lo, hi)` | n q / a / q lines `1 i x` or `2 l r` |

## Pieces

For a format that isn't listed, write the loop yourself:

| Make | Gives |
| --- | --- |
| `rnd(lo, hi)` | one integer |
| `arr(n, lo, hi)`, `distinct_arr(n, lo, hi)` | n integers, or n different ones |
| `perm(n)` | 1..n shuffled |
| `split(total, parts)` | `parts` sizes that add up to `total` |
| `str(n, "ab")`, `grid(n, m, ".#")` | a string, a grid |
| `tree(n, shape)`, `graph(n, m)`, `weighted(edges, lo, hi)` | edge lists |

| Print | Does |
| --- | --- |
| `out(a, b, v)` | one line, space-separated; vectors and pairs are spread out |
| `rows(x)` | one item per line |

```cpp
int t = 5;  out(t);                                     // each test: "n x" then the array
for (ll n : split(200000, t)) { out(n, rnd(1, 1e9)); out(arr(n, 1, 1e9)); }
```

## Recipes

| Goal | Generator | Then |
| --- | --- | --- |
| Will it TLE? | `arrays(1, 200000, 1, 1e9);` | Cmd+Shift+B > Fast |
| Deep recursion | `trees(1, 200000, "line");` | Cmd+B: a DFS that would crash on a default Mac stack runs |
| Find the WA | `arrays(10000, 30000, 1, 5);` (tiny tests, small values) | compare with a brute force, or use the stress template |
| Worst case for a search | fix the slow parameter at its max, randomise the rest | Fast |
