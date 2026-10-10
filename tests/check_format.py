"""Reads input.txt back and checks it matches the format a gen.h function promises."""
import sys

fmt, t = sys.argv[1], int(sys.argv[2])
lines = open("input.txt").read().split("\n")
pos = 0


def line():
    global pos
    pos += 1
    return lines[pos - 1]


def ints():
    return list(map(int, line().split()))


def connected(n, edges):
    parent = list(range(n + 1))

    def find(x):
        while parent[x] != x:
            parent[x] = parent[parent[x]]
            x = parent[x]
        return x

    for u, v in edges:
        parent[find(u)] = find(v)
    return len({find(i) for i in range(1, n + 1)}) <= 1


tests = int(line()) if t else 1
assert tests == max(t, 1), "wrong t line"
for _ in range(tests):
    if fmt == "numbers":
        assert len(ints()) == 1
        continue
    if fmt == "number_pairs":
        assert len(ints()) == 2
        continue
    if fmt == "bare_strings":
        assert set(line()) <= set("ab")
        continue
    if fmt in ("grids", "matrices"):
        n, m = ints()
        rows = [line() for _ in range(n)]
        assert all(len(r) == m if fmt == "grids" else len(r.split()) == m for r in rows)
        continue
    head = ints()
    n = head[0]
    if fmt == "arrays":
        assert len(ints()) == n
    elif fmt == "arrays_k":
        assert len(ints()) == n and 1 <= head[1] <= n
    elif fmt == "two_arrays":
        assert len(ints()) == n and len(ints()) == n
    elif fmt == "permutations":
        assert sorted(ints()) == list(range(1, n + 1))
    elif fmt == "strings":
        assert len(line()) == n
    elif fmt == "string_pairs":
        assert len(line()) == n and len(line()) == n
    elif fmt == "segments":
        assert all(l <= r for l, r in (ints() for _ in range(n)))
    elif fmt == "points":
        assert all(len(ints()) == 2 for _ in range(n))
    elif fmt in ("trees", "weighted_trees"):
        edges = [ints() for _ in range(n - 1)]
        assert connected(n, [(e[0], e[1]) for e in edges])
    elif fmt == "parent_trees":
        p = ints()
        assert len(p) == n - 1 and all(1 <= p[i] < i + 2 for i in range(n - 1))
    elif fmt in ("graphs", "weighted_graphs"):
        m = head[1]
        edges = [ints()[:2] for _ in range(m)]
        assert n - 1 <= m <= n * (n - 1) // 2 and connected(n, edges)
        assert len({(min(u, v), max(u, v)) for u, v in edges}) == m and all(u != v for u, v in edges)
    elif fmt in ("range_queries", "update_queries"):
        q = head[1]
        assert len(ints()) == n
        for _ in range(q):
            x = ints()
            if fmt == "range_queries":
                assert 1 <= x[0] <= x[1] <= n
            else:
                assert (x[0] == 1 and 1 <= x[1] <= n) or (x[0] == 2 and 1 <= x[1] <= x[2] <= n)
    else:
        sys.exit("unknown format " + fmt)
