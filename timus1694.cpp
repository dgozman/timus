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
using i16 = int16_t;
using u16 = uint16_t;
// using uint128_t = __uint128_t;

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

i64 gen(Input& in, Output& out, int m, int k, int n) {
  std::array<int, 22> cur;
  std::array<i64, 18 + 1> f;
  for (int i = 1; i <= m; i++) {
    // m * (m - 1) * .... (m - i + 1)
    f[i] = 1;
    for (int j = 0; j < i; j++)
      f[i] = f[i] * (m - j);
  }

  int iter = 0;

  auto check = [&] () {
    for (int i = n; i < n * k; i++)
      cur[i] = cur[i % n];
    for (int s = 0; s < n; s++) {
      for (int len = 1; len < n; len++) {
        bool eq = true;
        for (int p = 0; p < len && eq; p++) {
          for (int j = 1; j < k && eq; j++) {
            if (cur[s + p] != cur[s + p + len * j])
              eq = false;
          }
        }
        if (eq)
          return false;
      }
    }
    return true;
  };

  i64 result = 0;
  function<void(int, int)> go = [&] (int index, int maxd) {
    if (index == n) {
      if (check())
        result += f[maxd];
      return;
    }
    // if (!(++iter % 10000))
    //   out(iter);
    for (int take = 1; take <= maxd + 1 && take <= m; take++) {
      cur[index] = take;
      go(index + 1, take == maxd + 1 ? maxd + 1 : maxd);
    }
  };
  go(0, 0);
  return result;
}

void solve(Input& in, Output& out) {
  // for (int m = 3; m <= 18; m++) {
  //   for (int k = 2; k <= 5; k++) {
  //     vec<i64> r;
  //     r.push_back(0);
  //     for (int n = 1; n * k <= 22; n++) {
  //       r[n] = gen(in, out, m, k, n);
  //       out(m, k, n, "=>", r[n]);
  //     }
  //   }
  // }
  int m = in.ni();
  int k = in.ni();
  int n = in.ni();
  n = n / k;
  i64 r = 0;
  for (int x = 1; x <= n; x++)
    r += gen(in, out, m, k, x);
  out(r);
}

// 18 2 22

