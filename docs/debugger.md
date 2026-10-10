# debug.h

A local-only debugger for competitive C++. One macro, any value, written to `debug.txt` and flushed every line. On the judge it compiles to nothing.

```cpp
#ifdef LOCAL
#include "AllHeaders.h"     // pulls in debug.h; cprun adds -DLOCAL and the include path
#else
#define debug(...) ;
#endif
```

The contest template (`CPP` + Tab) already has these lines.

## Four forms

| You write | `debug.txt` shows |
| --- | --- |
| `debug(x, v, mp);` | `[solve:42] x = 5 \| v = [1, 2, 3] \| mp = {1: [2, 3]}` |
| `debug(dp);` | one row per line with its index, for any grid or adjacency list |
| `debug("after dfs");` | `[solve:42] after dfs`: a string literal prints as a label |
| `debug();` | `[solve:42] reached`: the last one printed before a crash is the last line that ran |

Every line starts with `[function:line]`, so output from a recursive function or a lambda is easy to place.

## What it prints

| Type | Printed as |
| --- | --- |
| integers, `__int128`, floating point | `42`, `1267650600228229401496703205376`, `2.5` |
| the value of `inf`, `LLONG_MAX`, `INT_MAX` and their negatives | `inf`, `-inf` |
| `char`, `bool`, strings, C strings | `'c'`, `true`, `"abc"`, `nullptr` |
| `pair`, `tuple` | `(1, "a", 2.5)` |
| `vector`, `deque`, `list`, `array`, C arrays | `[1, 2, 3]` |
| `set`, `multiset`, `unordered_set`, pb_ds `ordered_set` | `{1, 1, 2}` (duplicates kept, any comparator) |
| `map`, `unordered_map` | `{1: [2, 3]}` (any comparator or hash) |
| `stack` / `queue` / `priority_queue` | bottom to top / front to back / pop order, top first |
| `vector<vector<T>>`, `vector<string>`, `T[N][M]` | one row per line, rows numbered |
| anything with `operator<<` | whatever your operator prints |

Nesting has no limit: `map<pair<int,int>, vector<set<int>>>` prints like everything else. A container longer than 100 items prints the first 100 and `...+N more`.

Your own struct needs one line:

```cpp
struct Edge { int u, v, w; friend ostream& operator<<(ostream& o, const Edge& e) { return o << e.u << "-" << e.v << " (" << e.w << ")"; } };
```

A type with no printer fails to compile with a message that says exactly that, instead of a page of template errors.

## Safety

- **Crash-proof output.** Every line is flushed, so a segfault or abort keeps everything printed before it.
- **Flood stop.** Past 10,000 lines or 4 MB, `debug.h` ends the program with exit code 86. `cprun` reports it as "Stopped: debug() flooded" and shows the last lines, which is where an endless loop repeats.
- **No macro leaks.** `debug.h` is included before the template's macros (`#define int long long`, `#define endl`, ...), so local types are exactly the judge's. A header that defines `int` as `long long` hides overflow bugs that only appear on the judge.

## Settings

At the top of `include/debug.h`:

| Setting | Default | Meaning |
| --- | --- | --- |
| `max_items` | 100 | items shown per container |
| `max_lines` | 10000 | lines per run before the flood stop |
| `max_bytes` | 4 MB | size per run before the flood stop |
| `stop_on_flood` | `true` | end the program at the cap; `false` only stops printing |
| `shown_as_inf` | `2e18` | your template's `inf`, printed as `inf` |

## How it works

Most debug templates are a list of overloads, one per container. A call is resolved against the overloads declared *above* it, so `print(vector<set<int>>)` cannot see `print(set)` declared later, and anything nested fails to compile. Custom comparators, hashes and `__int128` need overloads nobody wrote.

`debug.h` has a single `print` template that dispatches with `if constexpr` on what a type *can do*, using C++20 concepts: is it string-like, a range, a map (has `mapped_type`), a container adaptor (has `container_type`), tuple-like, streamable. One template calling itself has no declaration-order problem, so any nesting works. Adaptors are read through their protected container `c`, so printing never pops your stack.

Names come from `#__VA_ARGS__`, split at top-level commas only: brackets and string literals are tracked, so `debug(f(a, b), mp[{1, 2}])` keeps both names whole. When a comma hides inside `<...>` and the count no longer matches, it prints the whole expression once instead of mislabelling values.
