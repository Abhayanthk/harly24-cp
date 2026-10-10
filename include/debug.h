// debug.h: local-only debug printer. Included through AllHeaders.h when built with -DLOCAL.
//
//   debug(x, v, mp);     [Harly24:42] x = 5 | v = [1, 2, 3] | mp = {1: [2, 3]}
//   debug();             [Harly24:42] reached        (crash hunting: the last line printed is the last line reached)
//   debug("after dfs");  [Harly24:42] after dfs      (a string literal prints as a label)
//   debug(dp);           grids (vector<vector<..>>, vector<string>, adjacency lists) print one row per line
//
// Prints any nesting of numbers, __int128, char, bool, strings, pair, tuple, every STL container
// (custom comparators and hashes too), C arrays, stack/queue/priority_queue, pb_ds ordered_set,
// and any type with operator<<. Writes to debug.txt and flushes every line, so output survives a crash.
// If debug() floods (over 10000 lines or 4 MB: an endless loop), the run ends with exit code 86.
//
// Self-contained on purpose: it is included before the template's macros (#define int long long, ...)
// so local types match the judge's.

#pragma once
#include <bits/stdc++.h>

namespace dbg {

// ---- settings ----
inline constexpr std::size_t max_items = 100;     // per container; the rest prints as "...+N more"
inline constexpr std::size_t max_lines = 10000;   // per run; keeps debug.txt small enough for Sublime
inline constexpr std::streamoff max_bytes = 4 << 20;   // 4 MB, for runs with few but huge lines
inline constexpr bool stop_on_flood = true;       // past either cap, end the program (exit code 86): it's a runaway loop
inline constexpr long long shown_as_inf = 2e18;   // the template's inf, printed as "inf"

inline std::ofstream file = [] { std::ofstream f("debug.txt"); f.precision(10); return f; }();
inline std::ostream& out() { return file.is_open() ? static_cast<std::ostream&>(file) : std::cerr; }
inline std::size_t lines_written = 0;
inline bool stopped = false;

inline bool flooded() {
    if (stopped) return true;
    if (lines_written < max_lines && !(file.is_open() && file.tellp() > max_bytes)) return false;
    out() << "[debug] flood: over " << max_lines << " lines or " << (max_bytes >> 20)
          << " MB. Endless loop? (caps are in debug.h)" << std::endl;
    stopped = true;
    if (stop_on_flood) { std::cout.flush(); std::_Exit(86); }
    return true;
}

// ---- type tests ----
template <class T> using bare = std::remove_cvref_t<T>;
template <class> inline constexpr bool always_false = false;

template <class T> concept StringLike = std::is_convertible_v<const T&, std::string_view>;
template <class T> concept Range = requires(const T& x) { std::begin(x); std::end(x); };
template <class T> concept Adaptor = requires { typename T::container_type; } && !Range<T>;
template <class T> concept TupleLike = requires { std::tuple_size<T>::value; } && !Range<T>;
template <class T> concept Streamable = requires(std::ostream& o, const T& x) { o << x; };
template <class T> concept SetOrMap = requires { typename T::key_type; };
template <class T> concept MapLike = requires { typename T::mapped_type; };
template <class T> concept IsPair = requires(const T& p) { p.first; p.second; };
template <class T> using element_t = bare<decltype(*std::begin(std::declval<const T&>()))>;
template <class T> concept Grid = Range<T> && !StringLike<T> && Range<element_t<T>>;

// stack/queue/priority_queue keep their container in a protected member `c`.
template <class A>
const typename A::container_type& underlying(const A& a) {
    struct Peek : A {
        static const typename A::container_type& get(const A& x) { return x.*&Peek::c; }
    };
    return Peek::get(a);
}

inline std::string i128_to_string(unsigned __int128 x, bool negative) {
    std::string s;
    do { s += char('0' + int(x % 10)); x /= 10; } while (x);
    if (negative) s += '-';
    return {s.rbegin(), s.rend()};
}

// ---- printer ----
template <class T> void print(std::ostream& o, const T& x);

template <class R> void print_items(std::ostream& o, const R& r, const char* open, const char* close) {
    o << open;
    std::size_t shown = 0, total = 0;
    for (const auto& item : r) {
        if (shown < max_items) {
            if (shown++) o << ", ";
            if constexpr (MapLike<R> && IsPair<bare<decltype(item)>>) {
                print(o, item.first); o << ": "; print(o, item.second);
            } else {
                print(o, item);
            }
        }
        ++total;
    }
    if (total > shown) o << ", ...+" << total - shown << " more";
    o << close;
}

template <class T> void print(std::ostream& o, const T& x) {
    if constexpr (std::is_same_v<T, char>) {
        o << '\'' << x << '\'';
    } else if constexpr (std::is_same_v<T, bool>) {
        o << (x ? "true" : "false");
    } else if constexpr (StringLike<T>) {
        if constexpr (std::is_pointer_v<T>) { if (!x) { o << "nullptr"; return; } }
        o << '"' << std::string_view(x) << '"';
    } else if constexpr (std::is_same_v<T, __int128>) {
        o << i128_to_string(x < 0 ? -(unsigned __int128)x : (unsigned __int128)x, x < 0);
    } else if constexpr (std::is_same_v<T, unsigned __int128>) {
        o << i128_to_string(x, false);
    } else if constexpr (std::is_integral_v<T>) {
        if constexpr (std::is_signed_v<T>) {
            if (x == std::numeric_limits<T>::max() || (long long)x == shown_as_inf) { o << "inf"; return; }
            if (x == std::numeric_limits<T>::min() || (long long)x == -shown_as_inf) { o << "-inf"; return; }
        }
        o << +x;
    } else if constexpr (std::is_floating_point_v<T>) {
        o << x;
    } else if constexpr (Range<T>) {
        if constexpr (SetOrMap<T>) print_items(o, x, "{", "}");
        else print_items(o, x, "[", "]");
    } else if constexpr (Adaptor<T>) {
        if constexpr (requires { typename T::value_compare; }) {
            T copy = x;                                   // priority_queue: pop order, top first
            std::vector<typename T::value_type> order;
            while (!copy.empty()) { order.push_back(copy.top()); copy.pop(); }
            print_items(o, order, "[", "]");
        } else {
            print_items(o, underlying(x), "[", "]");      // stack: bottom..top, queue: front..back
        }
    } else if constexpr (TupleLike<T>) {
        o << '(';
        std::apply([&](const auto&... e) { std::size_t i = 0; ((o << (i++ ? ", " : ""), print(o, e)), ...); }, x);
        o << ')';
    } else if constexpr (Streamable<T>) {
        o << x;
    } else {
        static_assert(always_false<T>, "debug(): no printer for this type. Give it `friend ostream& operator<<(ostream&, const T&)` or debug its fields.");
    }
}

template <class T> void print_value(std::ostream& o, const T& x) {
    if constexpr (Grid<T>) {
        std::size_t rows = 0;
        for (const auto& r : x) {
            if (rows < max_items) {
                o << "\n    " << std::setw(3) << rows << ": ";
                print(o, r);
                ++lines_written;
            }
            ++rows;
        }
        if (rows == 0) o << "[]";
        if (rows > max_items) o << "\n    ...+" << rows - max_items << " more rows";
    } else {
        print(o, x);
    }
}

// "a, f(b, c), mp[{1, 2}]" -> {"a", "f(b, c)", "mp[{1, 2}]"}: splits at top-level commas only.
inline std::vector<std::string> split_names(std::string_view s) {
    std::vector<std::string> names(1);
    int depth = 0;
    char quote = 0;
    for (std::size_t i = 0; i < s.size(); ++i) {
        char c = s[i];
        if (quote) {
            if (c == '\\' && i + 1 < s.size()) { names.back() += c; c = s[++i]; }
            else if (c == quote) quote = 0;
        } else if (c == '"' || c == '\'') {
            quote = c;
        } else if (c == '(' || c == '[' || c == '{') {
            ++depth;
        } else if (c == ')' || c == ']' || c == '}') {
            --depth;
        } else if (c == ',' && depth == 0) {
            names.emplace_back();
            continue;
        }
        names.back() += c;
    }
    for (auto& n : names) {
        auto first = n.find_first_not_of(' '), last = n.find_last_not_of(' ');
        n = first == std::string::npos ? "" : n.substr(first, last - first + 1);
    }
    return names;
}

template <class... Ts>
void emit(const char* func, int line, const char* names, const Ts&... values) {
    if (flooded()) return;
    std::ostream& o = out();
    o << '[' << func << ':' << line << "] ";
    if constexpr (sizeof...(Ts) == 0) {
        o << "reached";
    } else {
        const std::vector<std::string> parts = split_names(names);
        const bool named = parts.size() == sizeof...(Ts);   // false only when a comma sits inside <...>
        if (!named) o << names << " = ";
        std::size_t i = 0;
        auto one = [&](const auto& v) {
            if (i) o << " | ";
            if constexpr (StringLike<bare<decltype(v)>>) {
                if (named && parts[i].starts_with('"')) { o << std::string_view(v); ++i; return; }
            }
            if (named) o << parts[i] << (Grid<bare<decltype(v)>> ? " =" : " = ");
            print_value(o, v);
            ++i;
        };
        (one(values), ...);
    }
    o << '\n';
    o.flush();
    ++lines_written;
}

}  // namespace dbg

#define debug(...) ::dbg::emit(__func__, __LINE__, #__VA_ARGS__ __VA_OPT__(,) __VA_ARGS__)
