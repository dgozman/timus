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

i64 pow(i64 x, i64 n, i64 mod) {
  i64 result = 1 % mod;
  while (n) {
    if (n & 1)
      result = (result * x) % mod;
    x = (x * x) % mod;
    n >>= 1;
  }
  return result;
}

i64 inv(i64 x, i64 mod) {
  return x ? pow(x, mod - 2, mod) : 0;
}

// https://sites.math.rutgers.edu/~zeilberg/Theses/TrevorBassMA.pdf
// part 4.3.1, page 29
void solve(Input& in, Output& out) {
  const i64 mod = 1000000000 + 7;

  int l = in.ni();
  int w = in.ni();

  if ((l * w) % 2) {
    out(0);
    return;
  }
  if (l == 1 || w == 1) {
    out(1);
    return;
  }

  int n = (l + w - 1) / 2;
  auto a = newVec2d<i64>(n, n, (i64) 0);
  for (int i = -1; i < l; i++) {
    for (int j = -1; j < w; j++) {
      if ((i + j + 2) % 2)
        continue;
      int r = n - 1 - (l - 1) / 2;
      int c = 0;
      r += (i - j) / 2;
      c += (i + j) / 2;
      if (r >= 0 && r < n && c >= 0 && c < n)
        a[r][c] = (i >= 0 && i < l - 1 && j >= 0 && j < w - 1) ? 2 : 1;
    }
  }
  auto b = newVec2d<i64>(n + 1, n + 1, (i64) 1);
  auto c = newVec2d<i64>(n, n, (i64) 0);

  auto& cur = a;
  auto& prev = b;
  auto& next = c;
  for (int k = n - 1; k >= 1; k--) {
    for (int i = 0; i < k; i++) {
      for (int j = 0; j < k; j++) {
        next[i][j] = (cur[i][j] * cur[i + 1][j + 1] + cur[i][j + 1] * cur[i + 1][j]) % mod;
        next[i][j] = next[i][j] * inv(prev[i + 1][j + 1], mod) % mod;
      }
    }
    auto& temp = prev;
    prev = cur;
    cur = next;
    next = temp;
  }

  out(cur[0][0]);
}
