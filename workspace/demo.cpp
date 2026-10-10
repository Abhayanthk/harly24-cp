// demo.cpp: debug() and every run verdict, one switch away.
// Change DEMO, press Cmd+B, then read the build panel (or info.txt) and debug.txt.
//
//   0  OK, plus a debug() tour of many types          → read debug.txt
//   1  UB trap: signed overflow                        → the panel names the line ("→ line 67")
//   2  UB trap: 1 << 40 on an int (needs 1LL << 40)
//   3  abort: v[i] with i out of range                 → message names the index and size, plus the line
//   4  abort: sort comparator using <=
//   5  TLE: endless loop, killed after 5 s
//   6  debug() inside an endless loop                  → stopped fast, last debug lines shown
//   7  memory guard: push_back forever, killed above 1 GB
//   8  recursion 1,000,000 deep: OK (512 MB stack locally, Codeforces gives 256 MB)

#define DEMO 0   // a #define, not a variable: anything else above #include disables the precompiled header

#include <bits/stdc++.h>
#if __has_include(<ext/pb_ds/assoc_container.hpp>)
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
template <class T>
using ordered_set = __gnu_pbds::tree<T, __gnu_pbds::null_type, std::less<T>, __gnu_pbds::rb_tree_tag, __gnu_pbds::tree_order_statistics_node_update>;
#endif

#ifdef LOCAL
#include "AllHeaders.h"
#else
#define debug(...)       ;
#endif

using namespace std;

#define int                             long long
#define inf                             (long long)2e18
#define sz(a)                           ((int)(a).size())
#define all(v)                          (v).begin(), (v).end()

// Hides a value from the optimizer, so these demos fail at run time like real input would.
int hidden(int x) { static volatile int box; box = x; return box; }

int depth(int n) { return n == 0 ? 0 : 1 + depth(n - 1); }

int32_t main()
{
    if (DEMO == 0) {
        int x = 42;
        vector<int> v = {5, 1, 4};
        map<int, vector<int>> groups = {{1, {2, 3}}, {7, {}}};
        set<pair<int, int>> edges = {{1, 2}, {2, 3}};
        vector<string> grid = {"#..", ".#."};
        vector<vector<int>> dp(3, vector<int>(4, inf));
        dp[0][0] = 0; dp[1][2] = 5;
        priority_queue<int> pq; for (int y : {3, 9, 1}) pq.push(y);
        ordered_set<int> os; for (int y : {10, 20, 30}) os.insert(y);

        debug("--- demo 0 ---");
        debug(x, v, sz(v));
        debug(groups, edges);
        debug(grid);
        debug(dp);
        debug(pq, os, os.order_of_key(25));
        debug();
        cout << "answer " << x << "\n";
    }
    if (DEMO == 1) {
        int big = LLONG_MAX - 5;
        debug(big);
        big += hidden(10);
        cout << big << "\n";
    }
    if (DEMO == 2) {
        int j = hidden(40);
        int mask = 1 << j;
        cout << mask << "\n";
    }
    if (DEMO == 3) {
        vector<int> v(3);
        v[hidden(5)] = 1;
    }
    if (DEMO == 4) {
        vector<int> v(40, 7);
        sort(all(v), [](int a, int b) { return a <= b; });
    }
    if (DEMO == 5) {
        volatile int i = 0;
        while (i >= 0) i = 0;
    }
    if (DEMO == 6) {
        int i = 0, steps = 0;
        while (i != 5) { i = (i + 2) % 4; steps++; debug(i, steps); }
    }
    if (DEMO == 7) {
        vector<int> v;
        while (true) v.push_back(1);
    }
    if (DEMO == 8) {
        cout << depth(hidden(1000000)) << "\n";
    }
}
