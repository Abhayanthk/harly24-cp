# Legacy

Everything this setup replaced, kept for the record. Nothing here is installed.

## Five debuggers in two years

| When | File | The idea | Where it broke |
| --- | --- | --- | --- |
| Nov 2024 | [2024-11-cp-debug-version-02](debuggers/2024-11-cp-debug-version-02.sublime-snippet) | `operator<<` for vector, set and map, written to `debug.txt` | names printed as one string (`a, b : 1 \| 2 \|`); only three containers; buffered, so a crash ate the output |
| Mar 2025 | [2025-03-debugging-normal](debuggers/2025-03-debugging-normal.sublime-snippet) | split names at commas, print to `cout` | wrote into the answer stream; `debug(f(a, b))` split inside the call |
| Mar 2025 | [2025-03-debugging-advanced](debuggers/2025-03-debugging-advanced.sublime-snippet) | the Nov 2024 debugger as a drop-in snippet | same limits |
| Aug 2025 | [2025-08-universal-debugger](debuggers/2025-08-universal-debugger.sublime-snippet) | one `dbg_print` overload per type, names split per argument | nesting failed whenever the inner type's overload came later in the file; comparators and hashes unsupported |
| Oct 2025 | [2025-10-debug-first-header.h](debuggers/2025-10-debug-first-header.h), [2025-10-debug.h](debuggers/2025-10-debug.h) | a header behind `-DLOCAL`, plus stack, queue and priority_queue | carried the template's macros, so `int` became 64-bit locally and `debug(1.5)` or a plain `int` was ambiguous; multiset printed through a set and lost duplicates |
| Oct 2026 | [include/debug.h](../include/debug.h) | one template dispatching on concepts, crash-safe, flood stop | |

The Oct 2025 header failed 9 of 11 cases in a test of nested and unusual types, and printed a multiset wrong without any error. The current one prints all of them; [tests/debug_cases.cpp](../tests/debug_cases.cpp) checks it on every push.

## Templates

| File | Replaced by |
| --- | --- |
| [contest-main-2025](templates/contest-main-2025.sublime-snippet) | `CPP`: ordered_set built in, no macro leaks |
| [leetcode-main-v1](templates/leetcode-main-v1.sublime-snippet) | `leetcode` |
| [ordered-set-template](templates/ordered-set-template.sublime-snippet) | `CPP`, which includes `ordered_set` |
| [cp-simple](templates/cp-simple.sublime-snippet), [boilerplate-test](templates/boilerplate-test.sublime-snippet), [AutoTimer.h](templates/AutoTimer.h) | cprun times every run (CPU time, crashes included) |
| [snippet-list](templates/snippet-list.sublime-snippet) | [docs/snippets.md](../docs/snippets.md), generated from the snippet files |
