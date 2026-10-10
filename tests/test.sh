#!/usr/bin/env bash
# tests/test.sh: prove the setup works on this machine.
#   debug.h prints every promised type exactly as expected
#   cprun gives the right verdict for every demo case, and names the crash line
#   every gen.h format produces valid input
#   every snippet compiles inside the contest template
#
# SKIP_CRASH_LINE=1 skips the crash-line checks (machines without a usable debugger).

set -u
repo=$(cd "$(dirname "$0")/.." && pwd -P)
cprun="$repo/bin/cprun"
work=$(mktemp -d)
trap 'rm -rf "$work"' EXIT
cd "$work"
[ -n "${SKIP_CRASH_LINE:-}" ] && export CPRUN_NO_DEBUGGER=1

pass=0; fail=0
check() {   # check NAME CONDITION-RESULT [DETAIL]
  if [ "$2" = 0 ]; then pass=$((pass + 1)); printf '  pass  %s\n' "$1"
  else fail=$((fail + 1)); printf '  FAIL  %s\n' "$1"; [ -n "${3:-}" ] && printf '%s\n' "$3" | sed 's/^/        /'; fi
}

echo "debug.h"
cp "$repo/tests/debug_cases.cpp" .
"$cprun" debug debug_cases.cpp > log.txt 2>&1
expected="$repo/tests/debug_cases.expected"
diff -u "$expected" debug.txt > diff.txt; check "prints every promised type" $? "$(head -20 diff.txt)"

echo "cprun verdicts"
verdicts=("✓ OK" "UB trap" "UB trap" "abort" "abort" "TLE" "debug() flooded" "Memory guard" "✓ OK")
lines=("" 67 72 77 81 "" "" "" "")
for demo in 0 1 2 3 4 5 6 7 8; do
  sed -E "s/^#define DEMO [0-9]+/#define DEMO $demo/" "$repo/workspace/demo.cpp" > demo.cpp
  : > input.txt
  TL=2 ML=300 "$cprun" debug demo.cpp > log.txt 2>&1
  grep -q "${verdicts[$demo]}" info.txt; check "demo $demo → ${verdicts[$demo]}" $? "$(tail -5 log.txt)"
  if [ -n "${lines[$demo]}" ] && [ -z "${SKIP_CRASH_LINE:-}" ]; then
    grep -q "→ line ${lines[$demo]}:" info.txt; check "demo $demo names line ${lines[$demo]}" $? "$(tail -3 info.txt)"
  fi
done

echo "gen.h formats"
formats=(
  "arrays|arrays(50, 2000, 1, 100)|50"         "arrays|arrays(0, 2000, 1, 100)|0"
  "arrays_k|arrays_k(20, 500, 1, 100)|20"      "two_arrays|two_arrays(20, 500, 1, 9)|20"
  "permutations|permutations(30, 600)|30"      "strings|strings(30, 600, \"ab\")|30"
  "bare_strings|bare_strings(30, 600, \"ab\")|30"   "string_pairs|string_pairs(30, 600, \"ab\")|30"
  "numbers|numbers(40, 1, 100)|40"             "number_pairs|number_pairs(0, 1, 100)|0"
  "segments|segments(20, 400, 1, 50)|20"       "points|points(20, 400, -50, 50)|20"
  "trees|trees(40, 400, \"line\")|40"          "parent_trees|parent_trees(40, 400)|40"
  "weighted_trees|weighted_trees(10, 300, 1, 9, \"star\")|10"
  "graphs|graphs(20, 400, 1500)|20"            "weighted_graphs|weighted_graphs(0, 300, 900, 1, 9)|0"
  "grids|grids(5, 4, 7, \".#\")|5"             "matrices|matrices(0, 3, 4, 1, 9)|0"
  "range_queries|range_queries(10, 500, 800, 1, 9)|10"
  "update_queries|update_queries(0, 500, 800, 1, 9)|0"
)
for spec in "${formats[@]}"; do
  IFS='|' read -r fmt call t <<< "$spec"
  printf '#include <bits/stdc++.h>\n#include "gen.h"\nint main() { %s; }\n' "$call" > gen_case.cpp
  "$cprun" gen_case.cpp > log.txt 2>&1 && python3 "$repo/tests/check_format.py" "$fmt" "$t" > check.txt 2>&1
  check "$call" $? "$(tail -3 log.txt check.txt 2>/dev/null)"
done

echo "snippets inside the contest template"
python3 - "$repo" > snippets.txt <<'EOF'
import pathlib, re, sys
repo = pathlib.Path(sys.argv[1])
def body(name):
    path = next(repo.glob(f"sublime/snippets/**/{name}.sublime-snippet"))
    s = re.search(r"<!\[CDATA\[(.*?)\]\]>", path.read_text(), re.S).group(1)
    s = re.sub(r"\$\{\d+:([^}]*)\}", r"\1", s)
    return re.sub(r"\$\{\d+\}|\$\d+", "", s).replace("\\$", "$")
template = body("contest-main")
head = template[: template.index("int n;")]
skip = {"contest-main", "leetcode-main", "stress-test", "binary-lifting",          # whole programs
        "sort-pairs-desc", "max-heap", "min-heap", "wildcard-patterns", "clock-timer"}   # statement fragments
needs = {"mod-ops": "ncr-mod", "kruskal": "dsu"}
for p in sorted(repo.glob("sublime/snippets/**/*.sublime-snippet")):
    name = p.stem
    if name in skip:
        continue
    pre = body(needs[name]) if name in needs else ""
    pathlib.Path(f"snip_{name}.cpp").write_text(head + pre + body(name) + "\nint32_t main() {}\n")
    print(name)
pathlib.Path("template_full.cpp").write_text(template)
EOF
gxx=''
for c in /opt/homebrew/bin/g++-{16,15,14,13} /usr/local/bin/g++-{16,15,14,13} g++-{16,15,14,13} g++; do
  p=$(command -v "$c" 2>/dev/null) || continue
  "$p" --version 2>/dev/null | grep -q 'Free Software Foundation' && { gxx=$p; break; }
done
"$gxx" -std=c++23 -fsyntax-only -DLOCAL -I"$repo/include" template_full.cpp 2> err.txt; check "contest-main" $? "$(head -3 err.txt)"
while read -r name; do
  "$gxx" -std=c++23 -fsyntax-only -DLOCAL -I"$repo/include" "snip_$name.cpp" 2> err.txt; check "$name" $? "$(grep -m2 error err.txt)"
done < snippets.txt

echo
echo "$pass passed, $fail failed"
[ "$fail" = 0 ]
