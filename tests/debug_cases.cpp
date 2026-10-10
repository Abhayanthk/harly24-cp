// Every type debug.h promises to print. tests/test.sh compares debug.txt with debug_cases.expected.
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
#define debug(...) ;
#endif
using namespace std;
#define int long long
#define inf (long long)2e18

struct Node { int a, b; friend ostream& operator<<(ostream& o, const Node& n) { return o << "Node{" << n.a << "," << n.b << "}"; } };
long long f(long long a, long long b) { return a * b; }

int32_t main() {
    vector<set<int>> vs(2); vs[1] = {3, 1};             debug(vs);
    vector<map<int, int>> vm(1); vm[0][5] = 6;           debug(vm);
    pair<int, vector<int>> p{1, {2, 3}};                 debug(p);
    int32_t x32 = 5;                                     debug(x32, 1.5, 'c', true, string("s"));
    __int128 big = 1; big <<= 100;                       debug(big, -big);
    set<int, greater<int>> desc{1, 2, 3};                debug(desc);
    array<int, 3> ar{1, 2, 3}; int carr[4] = {4, 3, 2, 1};   debug(ar, carr);
    multiset<int> ms{1, 1, 2};                           debug(ms);
    debug(__builtin_popcountll(7), f(3, 4), max(1LL, 2LL));
    stack<int> st; st.push(1); st.push(2);
    queue<int> q; q.push(7); q.push(8);
    priority_queue<int> pq; for (int v : {5, 1, 9}) pq.push(v);
    priority_queue<int, vector<int>, greater<int>> mn; for (int v : {5, 1, 9}) mn.push(v);
    debug(st, q, pq, mn);
    tuple<int, string, double> tp{1, "a", 2.5};          debug(tp);
    vector<vector<int>> dp(2, vector<int>(3, inf)); dp[0][0] = 0; dp[1][2] = LLONG_MAX;   debug(dp);
    vector<string> grid = {"#..", ".#."};                debug(grid);
    vector<Node> nodes{{1, 2}};                          debug(nodes);
    vector<int> longv(150, 7);                           debug(longv);
    vector<bool> vb{true, false};                        debug(vb);
    bitset<5> bs(5);                                     debug(bs);
    map<pair<int, int>, set<int>> mps; mps[{1, 2}] = {3};   debug(mps[{1, 2}], mps);
    set<pair<int, int>> sp{{1, 2}};                      debug(sp);
    vector<vector<set<int>>> v3(2, vector<set<int>>(2)); v3[1][1] = {4};   debug(v3);
    debug(pair<int, int>(1, 2));
    debug("label");
    debug();
    deque<int> dq{1, 2}; list<int> ls{3};               debug(dq, ls);
#ifdef __GLIBCXX__
    ordered_set<int> os; os.insert(5); os.insert(2);     debug(os, os.order_of_key(3));
#endif
    const char* np = nullptr;                            debug(np);
}
