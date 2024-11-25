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
  auto p = in.nvi(n);
  vec<int> rv(2 * n, -1);
  vec<int> ri(2 * n, -1);
  for (int i = 0; i < n; i++) {
    rv[i + n] = p[i];
    ri[i + n] = i + 1;
  }
  for (int i = n - 1; i >= 1; i--) {
    if (rv[2 * i] < rv[2 * i + 1]) {
      rv[i] = rv[2 * i];
      ri[i] = ri[2 * i];
    } else {
      rv[i] = rv[2 * i + 1];
      ri[i] = ri[2 * i + 1];
    }
  }

  auto rmq = [&](int l, int r) {
    l += n - 1;
    r += n - 1;
    int v = rv[l];
    int i = ri[l];
    while (l <= r) {
      if (rv[l] < v) {
        v = rv[l];
        i = ri[l];
      }
      if (rv[r] < v) {
        v = rv[r];
        i = ri[r];
      }
      l = (l + 1) / 2;
      r = (r - 1) / 2;
    }
    return i;
  };

  const i64 mod = 1000000007;
  function<i64(int, int)> find = [&](int l, int r) {
    if (l >= r)
      return (i64)1;
    int pos = rmq(l, r);
    i64 res = (find(l, pos - 1) * find(pos + 1, r)) % mod;
    i64 len = r - l + 1;
    return len == n ? res : (res * len) % mod;
  };

  auto inv = [&](i64 x) {
    i64 res = 1;
    i64 p = mod - 2;
    while (p > 0) {
      if (p % 2 == 1)
        res = (res * x) % mod;
      x = (x * x) % mod;
      p /= 2;
    }
    return res;
  };

  i64 r = find(1, n);
  // out("r", r);
  r = inv(r);
  for (int i = 2; i <= n - 1; i++)
    r = (r * i) % mod;
  out(r);
}
