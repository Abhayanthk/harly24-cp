// gen.h: helpers for gen.cpp. Cmd+B on any gen*.cpp writes what it prints to input.txt (see run.sh).
#pragma once
#include <bits/stdc++.h>
#include <cassert>   // GCC 16's <bits/stdc++.h> no longer provides assert

using ll = long long;

namespace gen_detail {
inline std::uint64_t pick_seed() {
    std::uint64_t s = std::random_device{}();
    std::cerr << "seed " << s << "\n";
    return s;
}
inline const bool fast_io = (std::ios::sync_with_stdio(false), true);
}  // namespace gen_detail

inline std::mt19937_64 rng(gen_detail::pick_seed());

// The same test every run: call seed(42); at the start of main.
inline void seed(std::uint64_t s) { rng.seed(s); std::cerr << "seed " << s << " (fixed)\n"; }

// ---- make ----

// random integer in [lo, hi]
inline ll rnd(ll lo, ll hi) {
    assert(lo <= hi);
    return std::uniform_int_distribution<ll>(lo, hi)(rng);
}

// n random integers in [lo, hi]
inline std::vector<ll> arr(ll n, ll lo, ll hi) {
    std::vector<ll> a(n);
    for (auto& x : a) x = rnd(lo, hi);
    return a;
}

// n different integers in [lo, hi], random order
inline std::vector<ll> distinct_arr(ll n, ll lo, ll hi) {
    assert(hi - lo + 1 >= n);
    std::set<ll> seen;
    std::vector<ll> a;
    while ((ll)a.size() < n) {
        ll x = rnd(lo, hi);
        if (seen.insert(x).second) a.push_back(x);
    }
    return a;
}

// 1..n shuffled (or start..start+n-1)
inline std::vector<ll> perm(ll n, ll start = 1) {
    std::vector<ll> p(n);
    std::iota(p.begin(), p.end(), start);
    std::shuffle(p.begin(), p.end(), rng);
    return p;
}

// `parts` positive integers that add up to `total`: sizes for "t tests, sum of n <= total"
inline std::vector<ll> split(ll total, ll parts) {
    assert(1 <= parts && parts <= total);
    std::vector<ll> cuts = distinct_arr(parts - 1, 1, total - 1);
    cuts.push_back(0);
    cuts.push_back(total);
    std::sort(cuts.begin(), cuts.end());
    std::vector<ll> sizes;
    for (ll i = 1; i < (ll)cuts.size(); i++) sizes.push_back(cuts[i] - cuts[i - 1]);
    return sizes;
}

// random string of length n over the given letters
inline std::string str(ll n, const std::string& letters = "abcdefghijklmnopqrstuvwxyz") {
    std::string s(n, ' ');
    for (auto& c : s) c = letters[rnd(0, (ll)letters.size() - 1)];
    return s;
}

// n rows of m cells
inline std::vector<std::string> grid(ll n, ll m, const std::string& cells = ".#") {
    std::vector<std::string> g(n);
    for (auto& row : g) row = str(m, cells);
    return g;
}

// n-1 edges of a tree on nodes 1..n.
// shape "random" is shallow; "line" is one long path (tests deep recursion); "star" is one centre.
inline std::vector<std::pair<ll, ll>> tree(ll n, const std::string& shape = "random") {
    assert(shape == "random" || shape == "line" || shape == "star");
    std::vector<ll> label = perm(n);
    std::vector<std::pair<ll, ll>> edges;
    for (ll i = 1; i < n; i++) {
        ll parent = shape == "line" ? i - 1 : shape == "star" ? 0 : rnd(0, i - 1);
        edges.push_back({label[parent], label[i]});
        if (rnd(0, 1)) std::swap(edges.back().first, edges.back().second);
    }
    std::shuffle(edges.begin(), edges.end(), rng);
    return edges;
}

// m different edges on nodes 1..n, no self-loops. connected = true builds on a random tree.
inline std::vector<std::pair<ll, ll>> graph(ll n, ll m, bool connected = true) {
    assert(m <= n * (n - 1) / 2 && (!connected || m >= n - 1));
    std::vector<std::pair<ll, ll>> edges;
    if (connected) edges = tree(n);
    std::set<std::pair<ll, ll>> seen;
    for (auto [u, v] : edges) seen.insert({std::min(u, v), std::max(u, v)});
    while ((ll)edges.size() < m) {
        ll u = rnd(1, n), v = rnd(1, n);
        if (u != v && seen.insert({std::min(u, v), std::max(u, v)}).second) edges.push_back({u, v});
    }
    std::shuffle(edges.begin(), edges.end(), rng);
    return edges;
}

// adds a weight in [lo, hi] to every edge: (u, v) -> (u, v, w)
inline std::vector<std::array<ll, 3>> weighted(const std::vector<std::pair<ll, ll>>& edges, ll lo, ll hi) {
    std::vector<std::array<ll, 3>> out;
    for (auto [u, v] : edges) out.push_back({u, v, rnd(lo, hi)});
    return out;
}

// ---- print ----

namespace gen_detail {
template <class T> void put(const T& x) {
    if constexpr (std::is_arithmetic_v<T> || std::is_convertible_v<const T&, std::string_view>) {
        std::cout << x;
    } else if constexpr (requires { x.first; x.second; }) {
        put(x.first); std::cout << ' '; put(x.second);
    } else {
        bool first = true;
        for (const auto& y : x) { if (!first) std::cout << ' '; first = false; put(y); }
    }
}
}  // namespace gen_detail

// everything on one line, space separated; vectors and pairs are spread out: out(n, k); out(a);
template <class... T> void out(const T&... xs) {
    bool first = true;
    ((std::cout << (first ? "" : " "), gen_detail::put(xs), first = false), ...);
    std::cout << '\n';
}

// one item per line: rows(edges); rows(grid);
template <class C> void rows(const C& items) {
    for (const auto& x : items) out(x);
}

// ---- whole inputs: the common Codeforces formats ----
// One call prints a complete input. "/" below means a new line.
// t = number of tests, printed first. t = 0 means the problem has no test-count line (one test).
// n, m, q = totals over all tests (the statement's "sum of n <= 2e5"), split randomly between tests.

template <class F> void for_tests(ll t, ll n, F test) {
    if (t == 0) { test(n); return; }
    out(t);
    for (ll ni : split(n, t)) test(ni);
}

// This test's part of a total that grows with n (m edges, q queries).
inline ll share(ll total, ll ni, ll n) { return std::max<ll>(1, (ll)((__int128)total * ni / n)); }

// n / a
inline void arrays(ll t, ll n, ll lo, ll hi) {
    for_tests(t, n, [&](ll ni) { out(ni); out(arr(ni, lo, hi)); });
}

// n k / a          (k in [1, kmax]; kmax = 0 means k <= n)
inline void arrays_k(ll t, ll n, ll lo, ll hi, ll kmax = 0) {
    for_tests(t, n, [&](ll ni) { out(ni, rnd(1, kmax ? kmax : ni)); out(arr(ni, lo, hi)); });
}

// n / a / b
inline void two_arrays(ll t, ll n, ll lo, ll hi) {
    for_tests(t, n, [&](ll ni) { out(ni); out(arr(ni, lo, hi)); out(arr(ni, lo, hi)); });
}

// n / p            (p = a permutation of 1..n)
inline void permutations(ll t, ll n) {
    for_tests(t, n, [&](ll ni) { out(ni); out(perm(ni)); });
}

// n / s
inline void strings(ll t, ll n, const std::string& letters = "abcdefghijklmnopqrstuvwxyz") {
    for_tests(t, n, [&](ll ni) { out(ni); out(str(ni, letters)); });
}

// s                (no length line)
inline void bare_strings(ll t, ll n, const std::string& letters = "abcdefghijklmnopqrstuvwxyz") {
    for_tests(t, n, [&](ll ni) { out(str(ni, letters)); });
}

// n / s / t        (two strings of length n)
inline void string_pairs(ll t, ll n, const std::string& letters = "abcdefghijklmnopqrstuvwxyz") {
    for_tests(t, n, [&](ll ni) { out(ni); out(str(ni, letters)); out(str(ni, letters)); });
}

// x                (one number per test)
inline void numbers(ll t, ll lo, ll hi) {
    if (t) out(t);
    for (ll i = 0; i < std::max<ll>(t, 1); i++) out(rnd(lo, hi));
}

// a b              (two numbers per test)
inline void number_pairs(ll t, ll lo, ll hi) {
    if (t) out(t);
    for (ll i = 0; i < std::max<ll>(t, 1); i++) out(rnd(lo, hi), rnd(lo, hi));
}

// n / n lines "l r" with l <= r
inline void segments(ll t, ll n, ll lo, ll hi) {
    for_tests(t, n, [&](ll ni) {
        out(ni);
        for (ll i = 0; i < ni; i++) { ll l = rnd(lo, hi); out(l, rnd(l, hi)); }
    });
}

// n / n lines "x y"
inline void points(ll t, ll n, ll lo, ll hi) {
    for_tests(t, n, [&](ll ni) {
        out(ni);
        for (ll i = 0; i < ni; i++) out(rnd(lo, hi), rnd(lo, hi));
    });
}

// n / n-1 lines "u v"            (shape "random", "line" or "star")
inline void trees(ll t, ll n, const std::string& shape = "random") {
    for_tests(t, n, [&](ll ni) { out(ni); rows(tree(ni, shape)); });
}

// n / p_2 .. p_n with p_i < i    (the "parent of each vertex" format)
inline void parent_trees(ll t, ll n, const std::string& shape = "random") {
    assert(shape == "random" || shape == "line" || shape == "star");
    for_tests(t, n, [&](ll ni) {
        std::vector<ll> p;
        for (ll i = 2; i <= ni; i++) p.push_back(shape == "line" ? i - 1 : shape == "star" ? 1 : rnd(1, i - 1));
        out(ni);
        out(p);
    });
}

// n / n-1 lines "u v w"
inline void weighted_trees(ll t, ll n, ll wlo, ll whi, const std::string& shape = "random") {
    for_tests(t, n, [&](ll ni) { out(ni); rows(weighted(tree(ni, shape), wlo, whi)); });
}

namespace gen_detail {
inline ll edges_for(ll m, ll ni, ll n, bool connected) {
    return std::clamp(share(m, ni, n), connected ? ni - 1 : 0, ni * (ni - 1) / 2);
}
}

// n m / m lines "u v"            (connected unless connected = false)
inline void graphs(ll t, ll n, ll m, bool connected = true) {
    for_tests(t, n, [&](ll ni) {
        ll mi = gen_detail::edges_for(m, ni, n, connected);
        out(ni, mi);
        rows(graph(ni, mi, connected));
    });
}

// n m / m lines "u v w"
inline void weighted_graphs(ll t, ll n, ll m, ll wlo, ll whi, bool connected = true) {
    for_tests(t, n, [&](ll ni) {
        ll mi = gen_detail::edges_for(m, ni, n, connected);
        out(ni, mi);
        rows(weighted(graph(ni, mi, connected), wlo, whi));
    });
}

// n m / n rows of m cells        (every test is n x m)
inline void grids(ll t, ll n, ll m, const std::string& cells = ".#") {
    if (t) out(t);
    for (ll i = 0; i < std::max<ll>(t, 1); i++) { out(n, m); rows(grid(n, m, cells)); }
}

// n m / n rows of m numbers      (every test is n x m)
inline void matrices(ll t, ll n, ll m, ll lo, ll hi) {
    if (t) out(t);
    for (ll i = 0; i < std::max<ll>(t, 1); i++) { out(n, m); for (ll r = 0; r < n; r++) out(arr(m, lo, hi)); }
}

// n q / a / q lines "l r"
inline void range_queries(ll t, ll n, ll q, ll lo, ll hi) {
    for_tests(t, n, [&](ll ni) {
        ll qi = share(q, ni, n);
        out(ni, qi);
        out(arr(ni, lo, hi));
        for (ll i = 0; i < qi; i++) { ll l = rnd(1, ni); out(l, rnd(l, ni)); }
    });
}

// n q / a / q lines "1 i x" (set a_i = x) or "2 l r" (ask about a_l..a_r)
inline void update_queries(ll t, ll n, ll q, ll lo, ll hi) {
    for_tests(t, n, [&](ll ni) {
        ll qi = share(q, ni, n);
        out(ni, qi);
        out(arr(ni, lo, hi));
        for (ll i = 0; i < qi; i++) {
            if (rnd(0, 1)) out(1, rnd(1, ni), rnd(lo, hi));
            else { ll l = rnd(1, ni); out(2, l, rnd(l, ni)); }
        }
    });
}
