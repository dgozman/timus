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

using D = long double;

D absl(D x) {
  return x < 0.0l ? -x : x;
}

D maxl(D a, D b) {
  return a > b ? a : b;
}

D minl(D a, D b) {
  return a < b ? a : b;
}

void solve(Input& in, Output& out) {
  D a0 = in.ni();
  D a1 = in.ni();
  D a2 = in.ni();
  D a3 = in.ni();
  D a4 = in.ni();
  bool neg = false;
  if (a4 < 0) {
    a0 = -a0;
    a1 = -a1;
    a2 = -a2;
    a3 = -a3;
    a4 = -a4;
    neg = true;
  }

  function<D(D)> f = [&] (D x) {
    return 1.0l / (a0 + x * (a1 + x * (a2 + x * (a3 + x * a4))));
  };

  // https://en.wikipedia.org/wiki/Gaussian_quadrature
  function<D(D, D)> gauss = [&] (D l, D r) {
    D A = sqrtl(3.0l / 5.0l) / 2;
    D x1 = 0.5 - A;
    D x2 = 0.5 + A;
    return (5 * f(l * x1 + r * x2) + 8 * f((l + r) / 2) + 5 * f(l * x2 + r * x1)) / 18 * (r - l);
  };

  // https://en.wikipedia.org/wiki/Adaptive_quadrature
  function<D(D, D)> integrate = [&] (D l, D r) {
    D f1 = f(l);
    D f2 = f(r);
    D f3 = f((l + r) / 2);
    D e = (maxl(f1, maxl(f2, f3)) - minl(f1, minl(f2, f3))) * (r - l);
    if (e < 1e-10l)
      return gauss(l, r);
    return integrate(l, (l + r) / 2) + integrate((l + r) / 2, r);
  };

  D result = 0.0l;
  D delta = 0.01l;
  for (D l = -10000.0l; l < 10000.0l; l += delta)
    result += integrate(l, l + delta);
  if (neg)
    result = -result;
  out.setPrecision(15);
  out(result);
}
