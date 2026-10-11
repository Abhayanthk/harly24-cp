# Snippets

Type the trigger in a C++ file and press Tab. Every snippet lives in `sublime/snippets/<group>/`;
drop a new `.sublime-snippet` file there and Sublime picks it up at once.

## Templates

| Trigger | What it gives you | File |
| --- | --- | --- |
| `CPP` | Contest template: ordered_set, debug(), macros, multi-test main | [contest-main](../sublime/snippets/templates/contest-main.sublime-snippet) |
| `leetcode` | LeetCode local runner: paste the Solution class and the examples as LeetCode shows them, debug every answer | [leetcode-main](../sublime/snippets/templates/leetcode-main.sublime-snippet) |
| `stressTesting` | Stress test: random small tests, brute vs fast, stop at the first mismatch | [stress-test](../sublime/snippets/templates/stress-test.sublime-snippet) |

## Math and number theory

| Trigger | What it gives you | File |
| --- | --- | --- |
| `digitMultiplication` | Product of decimal digits | [digit-product](../sublime/snippets/math/digit-product.sublime-snippet) |
| `digitSum` | Sum of decimal digits | [digit-sum](../sublime/snippets/math/digit-sum.sublime-snippet) |
| `factors` | All divisors in O(sqrt n) | [divisors](../sublime/snippets/math/divisors.sublime-snippet) |
| `isSaquare` | Perfect-square check | [is-square](../sublime/snippets/math/is-square.sublime-snippet) |
| `modularOperaton` | Modular add, sub, mul, inverse, div | [mod-ops](../sublime/snippets/math/mod-ops.sublime-snippet) |
| `ncrSimple` | Exact nCr with __int128, -1 above a cap | [ncr-exact](../sublime/snippets/math/ncr-exact.sublime-snippet) |
| `combinationUsingFermatsTheorem` | nCr / nPr mod p with factorials and Fermat inverses | [ncr-mod](../sublime/snippets/math/ncr-mod.sublime-snippet) |
| `powerMode` | Binary exponentiation mod m | [power-mod](../sublime/snippets/math/power-mod.sublime-snippet) |
| `powerSafe` | Power that returns -1 instead of overflowing | [power-safe](../sublime/snippets/math/power-safe.sublime-snippet) |
| `powerNormal` | Binary exponentiation, no mod | [power](../sublime/snippets/math/power.sublime-snippet) |
| `PrimeFactorizationForAbove1e12` | Factorize numbers above 1e12: Miller-Rabin + Pollard rho | [prime-factors-big](../sublime/snippets/math/prime-factors-big.sublime-snippet) |
| `primeFactorisation` | Prime factorization with a 2-3-5 wheel | [prime-factors](../sublime/snippets/math/prime-factors.sublime-snippet) |
| `seive_spf` | Smallest-prime-factor sieve for fast factorization | [sieve-spf](../sublime/snippets/math/sieve-spf.sublime-snippet) |
| `seive` | Sieve of Eratosthenes | [sieve](../sublime/snippets/math/sieve.sublime-snippet) |

## Graphs and trees

| Trigger | What it gives you | File |
| --- | --- | --- |
| `binaryLiftintTree` | Binary lifting: LCA, k-th ancestor, max edge on a path | [binary-lifting](../sublime/snippets/graphs/binary-lifting.sublime-snippet) |
| `dijkstra` | Dijkstra with a min-heap, skips stale entries | [dijkstra](../sublime/snippets/graphs/dijkstra.sublime-snippet) |
| `dsu` | Disjoint set union: path compression + union by rank | [dsu](../sublime/snippets/graphs/dsu.sublime-snippet) |
| `kruskalsMST` | Kruskal MST on top of the dsu snippet | [kruskal](../sublime/snippets/graphs/kruskal.sublime-snippet) |
| `topoSort` | Kahn's topological sort with cycle detection | [topo-sort](../sublime/snippets/graphs/topo-sort.sublime-snippet) |
| `weightedTree` | Weighted adjacency list | [weighted-tree](../sublime/snippets/graphs/weighted-tree.sublime-snippet) |

## Data structures

| Trigger | What it gives you | File |
| --- | --- | --- |
| `quemax` | priority_queue max-heap | [max-heap](../sublime/snippets/data-structures/max-heap.sublime-snippet) |
| `quemin` | priority_queue min-heap | [min-heap](../sublime/snippets/data-structures/min-heap.sublime-snippet) |
| `multiset` | Multiset wrapper with counted erase | [multibag](../sublime/snippets/data-structures/multibag.sublime-snippet) |
| `segmentTreeLazyPropagation` | Segment tree: range add, range sum (lazy propagation) | [segtree-lazy](../sublime/snippets/data-structures/segtree-lazy.sublime-snippet) |
| `segmentTreeMinMax` | Segment tree: point update, range min | [segtree-min](../sublime/snippets/data-structures/segtree-min.sublime-snippet) |
| `segmentTreeSumQuerry` | Segment tree: point update, range sum | [segtree-sum](../sublime/snippets/data-structures/segtree-sum.sublime-snippet) |

## Arrays and strings

| Trigger | What it gives you | File |
| --- | --- | --- |
| `isSorted` | Non-decreasing check | [is-sorted](../sublime/snippets/arrays-strings/is-sorted.sublime-snippet) |
| `makeUnique` | Remove duplicates, keep first occurrences in order | [make-unique](../sublime/snippets/arrays-strings/make-unique.sublime-snippet) |
| `smallestArrayRotationUsingBooth'sAlgo` | Lexicographically smallest rotation (Booth, O(n)) | [min-rotation](../sublime/snippets/arrays-strings/min-rotation.sublime-snippet) |
| `nextlexi` | Next string in lexicographic order | [next-lexi](../sublime/snippets/arrays-strings/next-lexi.sublime-snippet) |
| `print2DList` | Print a char grid | [print-grid](../sublime/snippets/arrays-strings/print-grid.sublime-snippet) |
| `pair_sort_decending` | Sort pairs by first, descending | [sort-pairs-desc](../sublime/snippets/arrays-strings/sort-pairs-desc.sublime-snippet) |
| `wildcardPattern` | Word -> wildcard pattern buckets (word ladder style) | [wildcard-patterns](../sublime/snippets/arrays-strings/wildcard-patterns.sublime-snippet) |

## Bits

| Trigger | What it gives you | File |
| --- | --- | --- |
| `bit_functions` | GCC bit builtins cheat sheet | [builtins](../sublime/snippets/bits/builtins.sublime-snippet) |
| `bitTonumber` | LSB-first bit string -> number | [from-bits](../sublime/snippets/bits/from-bits.sublime-snippet) |
| `bitForm` | Number -> bit vector | [to-bits](../sublime/snippets/bits/to-bits.sublime-snippet) |

## Utility

| Trigger | What it gives you | File |
| --- | --- | --- |
| `time` | Time a block with clock() | [clock-timer](../sublime/snippets/utility/clock-timer.sublime-snippet) |
