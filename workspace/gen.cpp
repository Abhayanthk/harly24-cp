// gen.cpp: describe the input in main, press Cmd+B, and input.txt is rewritten (any gen*.cpp file works).
// The panel shows the seed; seed(42); at the start of main gives the same test every run.
//
// WHOLE INPUTS: one call prints everything. "/" = new line.
//   t = number of tests (printed first); t = 0 means the problem has no t line.
//   n, m, q = totals over all tests ("sum of n <= 2e5"), split randomly between the tests.
//
//   arrays(t, n, lo, hi)                     n / a
//   arrays_k(t, n, lo, hi, kmax)             n k / a                  kmax = 0 means k <= n
//   two_arrays(t, n, lo, hi)                 n / a / b
//   permutations(t, n)                       n / p
//   strings(t, n, "ab")                      n / s
//   bare_strings(t, n, "ab")                 s
//   string_pairs(t, n, "ab")                 n / s / t
//   numbers(t, lo, hi)                       x
//   number_pairs(t, lo, hi)                  a b
//   segments(t, n, lo, hi)                   n / n lines "l r"
//   points(t, n, lo, hi)                     n / n lines "x y"
//   trees(t, n, "line")                      n / n-1 lines "u v"      shape: "random" "line" "star"
//   parent_trees(t, n)                       n / p_2 .. p_n           p_i < i
//   weighted_trees(t, n, wlo, whi)           n / n-1 lines "u v w"
//   graphs(t, n, m)                          n m / m lines "u v"      connected; add false for any graph
//   weighted_graphs(t, n, m, wlo, whi)       n m / m lines "u v w"
//   grids(t, n, m, ".#")                     n m / n rows             every test n x m
//   matrices(t, n, m, lo, hi)                n m / n rows of m numbers
//   range_queries(t, n, q, lo, hi)           n q / a / q lines "l r"
//   update_queries(t, n, q, lo, hi)          n q / a / q lines "1 i x" or "2 l r"
//
// PIECES, for a format that isn't listed (write the loop yourself):
//   make:  rnd(lo, hi)  arr(n, lo, hi)  distinct_arr(n, lo, hi)  perm(n)  split(total, parts)
//          str(n, "ab")  grid(n, m, ".#")  tree(n, shape)  graph(n, m)  weighted(edges, lo, hi)
//   print: out(a, b, v)  one line, vectors spread out      rows(x)  one item per line

#include <bits/stdc++.h>
#include "gen.h"

int main() {
    arrays(10, 200000, 1, 1e9);   // 10 tests, sum of n = 2e5, values up to 1e9: swap in the problem's format
}

// ---- examples: copy one line into main ----
//
//   arrays(10, 200000, 1, 1e9);                     10 tests, sum of n = 2e5        (speed check)
//   arrays(10000, 30000, 1, 5);                     10000 tiny tests, small values  (edge cases, with a brute force)
//   arrays(0, 200000, 1, 1e9);                      no t line: "n / a" once
//   arrays_k(5, 200000, 1, 1e9, 1e9);               "n k / a", k up to 1e9
//   strings(5, 200000, "01");                       binary strings
//   trees(1, 200000, "line");                       one long path: deep DFS check
//   weighted_graphs(0, 200000, 200000, 1, 1e9);     Dijkstra-size graph
//   grids(0, 1000, 1000, ".#");                     one 1000 x 1000 grid
//   update_queries(0, 200000, 200000, 1, 1e9);      segment-tree input
//
// A format that isn't listed, e.g. "n x / a" per test:
//   int t = 5;  out(t);
//   for (ll n : split(200000, t)) { out(n, rnd(1, 1e9)); out(arr(n, 1, 1e9)); }
