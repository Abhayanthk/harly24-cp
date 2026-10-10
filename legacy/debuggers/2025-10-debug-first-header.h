// debug.h
#ifndef DEBUG_H
#define DEBUG_H

#include <bits/stdc++.h>
using namespace std;

#define PI                              3.141592653589793238462
#define pb                              push_back
#define vi                              vector<int>
#define vp                              vector<pair<int, int>>
#define fi                              first
#define sec                             second
#define vii                             vector<vector<int>>
#define vpp                             vector<vector<pair<int, int>>>
#define int                             long long
#define inf                             LLONG_MAX
#define endl                            "\n"
#define line                            cout << endl
#define sz(a)                           ((int)a.size())
#define neginf                          LLONG_MIN
#define Min(a)                          *min_element(a.begin(), a.end())
#define Max(a)                          *max_element(a.begin(), a.end())
#define all(v)                          v.begin(), v.end()
#define double                          long double
#define prec(n)                         fixed << setprecision(n)
#define que_max                         priority_queue <int>
#define que_min                         priority_queue <int, vector<int>, greater<int>>
#define lcm(a, b)                       ((a * b) / gcd(a, b))
#define printf(v)                       for(auto x: v) cout << x << " "
#define input(v)                        for(int i = 0; i < sz(v); i++) cin >> v[i]
#define sort1(arr)                      sort(arr.begin(), arr.end(), [](const vector<int>& a, const vector<int>& b) {return a[1] < b[1];});
#define nfor(i,a,b)                     for(int i = a; i < b; i++)
#define rfor(i,a,b)                     for(int i = a; i >= b; i--)
#define sort1rev(arr)                   sort(arr.begin(), arr.end(), [](const vector<int>& a, const vector<int>& b) {return a[1] > b[1];});
#define findString(s,c)                 s.find(c) != string::npos // s.erase(pointer, 1);

ofstream debug_out("debug.txt");

// --- BASE PRINT FUNCTIONS ---
void dbg_print(int x) { debug_out << x; }
// void dbg_print(long x) { debug_out << x; }
// void dbg_print(long long x) { debug_out << x; }
void dbg_print(unsigned x) { debug_out << x; }
void dbg_print(unsigned long x) { debug_out << x; }
void dbg_print(unsigned long long x) { debug_out << x; }
void dbg_print(float x) { debug_out << x; }
void dbg_print(double x) { debug_out << x; }
// void dbg_print(long double x) { debug_out << x; }
void dbg_print(char x) { debug_out << '\'' << x << '\''; }
void dbg_print(const char *x) { debug_out << '"' << x << '"'; }
void dbg_print(const string &x) { debug_out << '"' << x << '"'; }
void dbg_print(bool x) { debug_out << (x ? "true" : "false"); }

// --- STL CONTAINERS ---
template <typename T, typename U>
void dbg_print(const pair<T, U> &x) {
    debug_out << "("; dbg_print(x.first); debug_out << ", "; dbg_print(x.second); debug_out << ")";
}

template <typename... T>
void dbg_print(const tuple<T...> &t) {
    debug_out << "(";
    apply([](const auto&... args) {
        size_t n = 0;
        ((debug_out << (n++ ? ", " : ""), dbg_print(args)), ...);
    }, t);
    debug_out << ")";
}

template <typename T>
void dbg_print(const vector<T> &x) {
    debug_out << "[";
    for (size_t i = 0; i < x.size(); i++) {
        dbg_print(x[i]);
        if (i + 1 < x.size()) debug_out << ", ";
    }
    debug_out << "]";
}

template <typename T>
void dbg_print(const set<T> &x) {
    debug_out << "{";
    for (auto it = x.begin(); it != x.end();) {
        dbg_print(*it);
        if (++it != x.end()) debug_out << ", ";
    }
    debug_out << "}";
}

template <typename T>
void dbg_print(const multiset<T> &x) {
    debug_out << "{";
    for (auto it = x.begin(); it != x.end();) {
        dbg_print(*it);
        if (++it != x.end()) debug_out << ", ";
    }
    debug_out << "}";
}

template <typename K, typename V>
void dbg_print(const map<K, V> &x) {
    debug_out << "{";
    for (auto it = x.begin(); it != x.end();) {
        dbg_print(it->first);
        debug_out << ": ";
        dbg_print(it->second);
        if (++it != x.end()) debug_out << ", ";
    }
    debug_out << "}";
}

template <typename K, typename V>
void dbg_print(const unordered_map<K, V> &x) {
    debug_out << "{";
    for (auto it = x.begin(); it != x.end();) {
        dbg_print(it->first);
        debug_out << ": ";
        dbg_print(it->second);
        if (++it != x.end()) debug_out << ", ";
    }
    debug_out << "}";
}

template <typename T>
void dbg_print(const unordered_set<T> &x) {
    debug_out << "{";
    for (auto it = x.begin(); it != x.end();) {
        dbg_print(*it);
        if (++it != x.end()) debug_out << ", ";
    }
    debug_out << "}";
}

// --- VARIADIC DEBUG ---
template <typename T>
void _dbg_one(const char* name, const T &value) {
    debug_out << name << " = ";
    dbg_print(value);
}

template <typename T, typename... V>
void _dbg_names(const char* names, const T &t, const V&... v) {
    const char* comma = strchr(names, ',');
    if (!comma) {
        _dbg_one(names, t);
        if constexpr(sizeof...(v)) { debug_out << " | "; _dbg_names(names, v...); }
        return;
    }
    string varname(names, comma - names);
    while (!varname.empty() && isspace(varname.back())) varname.pop_back();
    while (!varname.empty() && isspace(varname.front())) varname.erase(varname.begin());

    _dbg_one(varname.c_str(), t);
    if constexpr(sizeof...(v)) {
        debug_out << " | ";
        _dbg_names(comma + 1, v...);
    }
}


#define DEBUG 1
#if DEBUG
    #define debug(...) do { debug_out << "DEBUG : "; _dbg_names(#__VA_ARGS__, __VA_ARGS__); debug_out << "\n"; } while(0)
#else
    #define debug(...)
#endif
#endif // DEBUG_H