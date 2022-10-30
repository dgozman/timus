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

const i64 mod = 1000000009;

i64 pow(i64 x, i64 p) {
  i64 r = 1;
  while (p > 0) {
    if (p & 1)
      r = (r * x) % mod;
    x = (x * x) % mod;
    p = p >> 1;
  }
  return r;
}

i64 inv(i64 x) {
  return pow(x, mod - 2);
}

vec2d<i64> mul(const vec2d<i64>& a, const vec2d<i64>& b) {
  vec2d<i64> c = newVec2d(a.size(), b[0].size(), (i64) 0);
  if (a[0].size() != b.size())
    panic();
  for (int i = 0; i < a.size(); i++) {
    for (int j = 0; j < a[i].size(); j++) {
      for (int k = 0; k < b[0].size(); k++)
        c[i][k] = (c[i][k] + a[i][j] * b[j][k]) % mod;
    }
  }
  return c;
}

void solve(Input& in, Output& out) {
  vec<i64> precalc({ 0, 0, 0, 0, 0, 0, 12, 46, 144, 110, 312, 290, 670, 706, 1538, 1732, 3504, 4288, 8098, 10568, 19044, 26042, 45222, 64220, 108382, 158324, 261754, 390314, 635666, 962282, 1550244, 2372372, 3792560, 5848746, 9299148, 14419296, 22838014, 35548790, 56153296, 87640646, 138180196, 216065986, 340223834, 532680994, 838025410, 1313251780 - mod });

  int k = 19;
  vec2d<i64> w = newVec2d(k, k + 1, (i64) 0);
  for (int i = 0; i < k; i++) {
    for (int j = 0; j < k; j++)
      w[i][j] = precalc[7 + i + j];
    w[i][k] = precalc[7 + i + k];
  }

  for (int i = 0; i < k; i++) {
    int row = -1;
    for (int j = i; j < k; j++) {
      if (w[i][j] != 0)
        row = j;
    }
    if (row == -1)
      panic();
    for (int j = 0; j <= k; j++) {
      i64 tmp = w[i][j];
      w[i][j] = w[row][j];
      w[row][j] = tmp;
    }
    i64 d = (mod - inv(w[i][i])) % mod;
    for (int row = 0; row < k; row++) {
      if (row == i)
        continue;
      i64 c = (w[row][i] * d) % mod;
      for (int j = 0; j <= k; j++)
        w[row][j] = (w[row][j] + c * w[i][j] % mod) % mod;
    }
  }

  vec<i64> p(k);
  for (int i = 0; i < k; i++)
    p[i] = w[i][k] * inv(w[i][i]) % mod;

  int n = in.ni();
  if (n < 7 + k) {
    out(precalc[n]);
    return;
  }

  vec2d<i64> m = newVec2d(k, k, (i64) 0);
  vec2d<i64> r = newVec2d(k, k, (i64) 0);
  for (int i = 0; i < k; i++)
    r[i][i] = 1;
  for (int i = 0; i < k - 1; i++)
    m[i][i + 1] = 1;
  for (int i = 0; i < k; i++)
    m[k - 1][i] = p[i];

  n -= 7 + k - 1;
  while (n > 0) {
    if (n & 1)
      r = mul(r, m);
    m = mul(m, m);
    n = n >> 1;
  }

  i64 result = 0;
  for (int i = 0; i < k; i++)
    result = (result + r[k - 1][i] * precalc[7 + i]) % mod;
  out(result);
}
