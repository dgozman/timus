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
  throw 42;
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

void solve(Input& in, Output& out) {
  int n = in.ni();
  int k = in.ni();
  n -= 2 * k;
  if (n <= 0) {
    out(0);
    return;
  }

  vec2d<long double> c = newVec2d(n + k + 1, n + k + 1, (long double) 0.0);
  for (int i = 1; i <= n + k; i++) {
    c[i][0] = (long double) 1.0;
    c[i][i] = (long double) 1.0;
    for (int j = 1; j < i; j++) {
      // i! / j! / (i - j)!
      // i! / (j - 1)! / (i - j + 1)!
      c[i][j] = c[i][j - 1] * (i - j + 1) / j;
      // c[i][j] = c[i][j - 1] - std::log((long double) j) + std::log((long double) (i - j + 1));
    }
  }

  vec2d<long double> r = newVec2d(n + 1, k + 1, (long double) 0.0);
  for (int b = 0; b <= k; b++)
    r[0][b] = 0.0;
    // r[0][b] = std::log((long double) 0.0);

  for (int g = 1; g <= n; g++) {
    r[g][0] = g;
    // r[g][0] = std::log((long double) g);
    for (int b = 1; b <= k; b++) {
      long double best = (long double) 0.0;
      for (int i = 0; i <= g + b; i++) {
        long double cur = 0;
        for (int j = max(0, i - b); j <= min(i, g); j++) {
          int ng = g - j;
          int nb = b - (i - j);
          // long double add = c[g][j] + c[b][i - j] + r[max(0, ng - nb)][nb] - c[b + g][i];
          long double add = c[g][j] / c[b + g][i] * c[b][i - j] * r[max(0, ng - nb)][nb];
          // cur += std::exp(add);
          cur += add;
        }
        best = max(best, cur);
      }
      // r[g][b] = std::log(best);
      r[g][b] = best;
    }
  }

  out.setPrecision(30);
  // out(std::exp(r[n][k]));
  out(r[n][k]);
}

/*

n, k    ->    n - k, k     ->    choose i,  total c[n + k][i] = sum (c[n][j] * c[k][i - j] * f[n - j][k - (i - j)])


*/
