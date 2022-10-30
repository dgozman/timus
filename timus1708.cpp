#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <cstring>
#include <cmath>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <numeric>
#include <queue>
#include <random>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;
bool __hack1 = std::ios::sync_with_stdio(false);
auto __hack2 = cin.tie(nullptr);
template<typename T> using vec = vector<T>;
template<typename T> using vec2d = vec<vec<T>>;
template<typename T> using vec3d = vec<vec2d<T>>;
using i64 = int64_t;
using u64 = uint64_t;
using i32 = int32_t;
using u32 = uint32_t;
using i8 = int8_t;
using u8 = uint8_t;
using u16 = uint16_t;
using uint128_t = __uint128_t;

struct Input {
  Input(istream &in) : in(&in) {}
  istream *in;
  template<class T> T next() const { T x; *in >> x; return x; }
  int ni() const { return next<int>(); }
  i64 ni64() const { return next<i64>(); }
  string ns() const { return next<string>(); }
  template<class T> vec<T> nvec(int n) const { vec<T> v(n); for (int i = 0; i < n; ++i) v[i] = next<T>(); return v; }
  vec<int> nvi(int n) const { return nvec<int>(n); }
  string getline() { string s; std::getline(*in, s); return s; }
};

struct Output {
  Output(ostream &out) : out(&out) {}
  ostream *out;
  inline void print() {}
  template<typename T> inline void printOne(const T& v) { *out << v; }
  template<typename T> inline void printOne(const vec<T>& v) { for (int i = 0; i < v.size(); i++) { if (i) *out << ' '; printOne(v[i]); } }
  template<typename T> inline void printOne(const vec2d<T>& v) { for (int i = 0; i < v.size(); i++) println(v[i]); }
  template<typename A, typename B> inline void printOne(const pair<A, B>& pair) { print(pair.first, pair.second); }
  template<typename T, typename...Ts> inline void print(const T &f, const Ts&... args) {
    printOne(f);
    if (sizeof...(args) != 0) { *out << ' '; print(args...); }
  }
  template<typename...Ts> inline void println(const Ts&... args) { print(args...); *out << '\n'; }
  template<typename...Ts> inline void operator() (const Ts&... args) { println(args...); }
  void setPrecision(int p) { *out << std::setprecision(p) << std::fixed; }
};

template<typename T> vec2d<T> newVec2d(int n, int m, const T& init) {
  vec2d<T> v(n, vec<T>(m, init));
  return v;
}

template<typename T> vec3d<T> newVec3d(int n, int m, int k, const T& init) {
  vec3d<T> v(n, newVec2d(m, k, init));
  return v;
}

void panic() {
  int z = 0;
  z = 1 / z;
}

template<typename T> bool rmn(T& value, const T& candidate) {
  if (candidate < value) { value = candidate; return true; }
  return false;
}

template<typename T> bool rmx(T& value, const T& candidate) {
  if (candidate > value) { value = candidate; return true; }
  return false;
}

void solve(Input& in, Output& out);
int main() {
  // ifstream fin("input.txt");
  // Input in(fin);
  // ofstream fout("output.txt");
  // Output out(fout);
  Input in(cin);
  Output out(cout);
  solve(in, out);
  return 0;
}

const int none = 36;
struct DigitSet {
  u16 s1, s2;
  u8 last, last_count, max, max1, max2;
  DigitSet(): s1(0), s2(0), last(1), last_count(0), max(0), max1(none), max2(none) {}
  DigitSet(u16 s1, u16 s2, u8 last): s1(s1), s2(s2), last(last), last_count(0), max(0), max1(none), max2(none) {}
};

const int max_len = 35;
const int max_count = 14;
using Pattern = std::array<char, max_len>;

void solve(Input& in, Output& out) {
  int base = in.ni();
  // int base = 36;
  int key_step = (max_len * (base - 1) + 1);
  int max_key = key_step * (max_len * (base - 1) * (base - 1) + 1);

  vec<bool> seen(max_key, false);
  vec<bool> good(max_key, false);

  vec<DigitSet> prev, next;
  next.push_back(DigitSet());
  for (int len = 1; len <= max_len; len++) {
    prev.swap(next);
    next.resize(0);
    for (const DigitSet& ds : prev) {
      for (u8 i = ds.last; i < base; i++) {
        DigitSet ns(ds.s1 + i, ds.s2 + i * i, i);
        int key = (int) ns.s2 * key_step + ns.s1;
        if (seen[key])
          continue;

        ns.last_count = i == ds.last ? ds.last_count + 1 : 1;
        ns.max = max(ds.max, ns.last_count);
        ns.max1 = ds.max1;
        ns.max2 = ds.max2;
        if (ns.last_count == max_count) {
          if (ns.max1 == none)
            ns.max1 = i;
          else
            ns.max2 = i;
        }

        seen[key] = true;
        next.push_back(ns);
        if (ns.max > max_count)
          continue;

        good[key] = true;
        if (ns.max1 != none) {
          for (int c1 = 0; c1 <= max_count; c1++) {
            int s2 = ns.s2 - c1 * ns.max1 * ns.max1;
            int s1 = ns.s1 - c1 * ns.max1;
            if (c1 > 0) {
              int key = s2 * key_step + s1;
              good[key] = false;
            }
            if (ns.max2 != none) {
              for (int c2 = 0; c2 <= max_count; c2++) {
                if (c1 > 0 || c2 > 0) {
                  int key = (s2 - c2 * ns.max2 * ns.max2) * key_step + (s1 - c2 * ns.max2);
                  good[key] = false;
                }
              }
            }
          }
        }
      }
    }
  }

  // out("size", good.size());

  std::fill(seen.begin(), seen.end(), false);
  prev.clear();
  prev.shrink_to_fit();
  next.clear();
  next.shrink_to_fit();

  vec<pair<DigitSet, pair<u8, Pattern>>> prev2, next2;
  next2.push_back(make_pair(DigitSet(), make_pair(0, Pattern())));
  vec<string> result;
  for (int len = 1; len <= max_len; len++) {
    prev2.swap(next2);
    next2.resize(0);
    for (const auto& value : prev2) {
      const DigitSet& ds = value.first;
      u8 pattern_len = value.second.first;
      const Pattern& pattern = value.second.second;
      for (u8 i = ds.last; i < base; i++) {
        DigitSet ns(ds.s1 + i, ds.s2 + i * i, i);
        int key = (int) ns.s2 * key_step + ns.s1;
        if (seen[key])
          continue;
        seen[key] = true;
        ns.last_count = i == ds.last ? ds.last_count + 1 : 1;
        ns.max = max(ds.max, ns.last_count);
        Pattern np(pattern);
        u8 new_len = pattern_len;
        if (i == ds.last) {
          if (ns.last_count == max_count) {
            new_len -= max_count - 2;
            np[new_len++] = '*';
          } else if (ns.last_count < max_count) {
            np[new_len++] = i < 10 ? '0' + i : 'A' + i - 10;
          }
        } else {
          np[new_len++] = i < 10 ? '0' + i : 'A' + i - 10;
        }
        next2.push_back(make_pair(ns, make_pair(new_len, np)));
        if (good[key])
          result.push_back(string(np.begin(), np.begin() + new_len));
      }
    }
  }

  sort(result.begin(), result.end());
  // out("res:", result.size());
  for (const auto& s : result)
    out(s);
}

/*

2: none
1*

3: none
1*2*

4:
1113 -> 222

1*2*
112*3*
12*3*
2*3*

1: 012*
2: *
3: 0*


5:
1113 -> 222
114 -> 33
2224 -> 1333

1*2*
112*3*
12*3*
1223*4*
123*4*
13*4*
2*3*
223*4*
23*4*
3*4*


6:
11115 -> 234
111255 -> 3444
1113 -> 222
114 -> 33
1135 -> 244
12355 -> 4444
2224 -> 1333
225 -> 144
3335 -> 2444




36x50: 8655759
*/

