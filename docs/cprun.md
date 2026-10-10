# cprun

Compiles one file like the judge, runs it on `input.txt` under guards, and ends every run with a verdict. Sublime's build (Cmd+B) calls it; so can you.

```bash
cprun A.cpp                    # debug build, the default
cprun fast A.cpp               # -O2, judge-like timing
cprun san A.cpp                # AddressSanitizer + UBSan
TL=60 ML=2048 cprun fast A.cpp # time limit (s) and memory limit (MB)
```

Any file named `gen*.cpp` is a generator: its output becomes `input.txt` instead (see [generator.md](generator.md)).

## Files

All next to the source:

| File | Holds |
| --- | --- |
| `input.txt` | stdin |
| `output.txt` | stdout |
| `debug.txt` | `debug()` lines |
| `error.txt` | stderr: `cerr`, STL debug messages, sanitizer reports |
| `info.txt` | the verdict, also printed to the build panel |

## Modes

| Sublime | Mode | Compiler and flags | Use it for |
| --- | --- | --- | --- |
| Cmd+B | `debug` | GCC, `-O1 -DLOCAL`, STL bounds checks, UB traps, strict warnings | solving: is the answer right? |
| Cmd+Shift+B > Fast | `fast` | GCC `-O2`, nothing else | timing on a max-size test |
| Cmd+Shift+B > Sanitize | `san` | ASan + UBSan (clang on macOS, GCC on Linux) | memory bugs the debug build can't see |
| Cmd+Shift+B > Stress | `fast`, 60 s limit | GCC `-O2` | the stress-test template |

Sublime remembers the last variant you picked; Cmd+B repeats it. Pick plain **CP** to get back to the debug build.

On macOS, plain `g++` is Apple clang, which has no pb_ds and differs from the judge. cprun looks for Homebrew's `g++-NN` first.

## What the debug build catches

| Mistake | How it shows up |
| --- | --- |
| `v[i]` out of range, `*s.begin()` on an empty set | abort; the message names the index and size (`_GLIBCXX_DEBUG`) |
| sort comparator using `<=` | abort: "comparison doesn't meet irreflexive requirements" |
| signed overflow, `1 << j` with `j >= 31`, `x / 0`, C-array overflow | UB trap at the exact instruction |
| a function that falls off the end without `return` | compile error |
| a loop variable shadowing an outer one, `if (a == b) ... else if (a == b)` | warning |

When the program dies, cprun reruns it once under the debugger (lldb on macOS, gdb on Linux) and adds the line:

```
✗ Runtime error: UB trap, one of
    • signed overflow: a * b or a + b too big for long long
    • ...
    → line 67:  big += hidden(10);
```

The rerun works on a copy in a scratch folder. On macOS a binary started by a debugger loses the editor's folder permission, and running it from `~/Documents` would ask for access after every build.

Integer division by zero deserves a note: on Apple Silicon it quietly returns 0, while the judge's x86 machines crash. The debug build traps it, so it can't pass locally and fail remotely.

## Guards

| Guard | Limit | Why |
| --- | --- | --- |
| time | `TL`, 5 s | an endless loop gets killed, not waited on |
| memory | `ML`, 1024 MB | a `push_back` loop can eat gigabytes in seconds and swap the machine; macOS ignores `ulimit -v`, so a watchdog polls memory |
| file size | 10 MB per file | an endless print loop dies instead of filling the disk and freezing Sublime |
| stderr | redirected | endless `cerr` lands in `error.txt`, never in the build panel; the panel shows the first 3,000 characters |
| debug() | 10,000 lines / 4 MB | `debug.h` ends the run itself and cprun shows the last lines |
| stack | 512 MB (macOS), 1 GB (Linux) | the judge gives 256 MB; the default 8 MB makes deep DFS crash only locally |

## Verdicts

| Verdict | Usual cause |
| --- | --- |
| `✓ OK` | |
| `TLE: killed after 5 s` | endless loop, or reading more input than `input.txt` has |
| `Memory guard: killed above 1024 MB` | `push_back` / `insert` in an endless loop, or a giant array |
| `Stopped: debug() flooded` | endless loop; the last debug lines show the state it repeats |
| `Stopped: a file passed 10 MB` | endless print loop |
| `Runtime error: segfault` | C-array or pointer out of range, runaway recursion |
| `Runtime error: abort` | a failed assert or STL debug check; the message above names it |
| `Runtime error: UB trap` | overflow, bad shift, `x / 0`, C-array out of range |
| `Runtime error: memory error` / `undefined behavior` | Sanitize found it; the report above names the line |
| `exit code N` | `main` returned `N` or called `exit(N)` |

## Timing

The time shown is the program's CPU time, which is what the judge measures. Your machine is probably faster than the judge's: if Fast shows more than about half the time limit on a max-size test, treat it as a TLE risk.

On macOS the first run of every new binary also waits about 0.5 s for a malware scan. It doesn't count towards CPU time, but it makes runs feel slower. System Settings > Privacy & Security > Developer Tools, add Sublime Text and your terminal, and it's gone.

## Speed

cprun keeps a precompiled `<bits/stdc++.h>` per flag set in `~/Library/Caches/harly24-cp` (Linux: `~/.cache/harly24-cp`), rebuilt automatically when the compiler or flags change. It saves about a second per build, as long as `#include <bits/stdc++.h>` is the first line of code in the file. Comments and `#define`s above it are fine; anything else turns it off.

## Platforms

| | macOS | Linux, WSL |
| --- | --- | --- |
| compiler | Homebrew `g++-NN` | system `g++` |
| crash line | lldb (Xcode command line tools) | gdb |
| Sanitize | clang, so no `ordered_set` | GCC, `ordered_set` works |
| stack | linker flag, 512 MB | `ulimit -s`, 1 GB |

Windows: use WSL.

## GCC 16 and `assert`

From GCC 16, `<bits/stdc++.h>` no longer declares `assert`. The contest template still has it, because the pb_ds headers behind `ordered_set` include `<cassert>`. In any other file that calls `assert`, add `#include <cassert>`, or it stops compiling the day your compiler, or the judge's, moves to GCC 16.
