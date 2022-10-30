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

void solve(Input& in, Output& out) {
  int n = in.ni();
  vec2d<int> w = newVec2d(n, n, 0);
  for (int i = 0; i < n; i++) {
    string s = in.ns();
    for (int j = 0; j < n; j++)
      w[i][j] = s[j] - '0';
    w[i][i] = 1;
  }

  int left = min(23, n);
  vec<int> subsets(1 << left, 0);
  subsets[0] = 1;
  for (int high = 0; high < left; high++) {
    int mask = 0;
    for (int i = 0; i < high; i++)
      mask |= w[high][i] << i;
    for (int subset = 0; subset < 1 << high; subset++)
      subsets[subset | (1 << high)] = subsets[subset] + subsets[subset & mask];
  }

  auto get_mask = [&] (int i, int from, int len) {
    int m = 0;
    for (int j = 0; j < len; j++)
      m |= w[i][from + j] << j;
    return m;
  };

  int right1 = (n - left) / 2;
  int right2 = n - left - right1;

  vec<int> mask1_left(1 << right1, 0);
  vec<int> mask1_right2(1 << right1, 0);
  vec<int> mask1_self(1 << right1, 0);
  for (int mright1 = 0; mright1 < (1 << right1); mright1++) {
    mask1_self[mright1] = mright1;
    mask1_left[mright1] = (1 << left) - 1;
    mask1_right2[mright1] = (1 << right2) - 1;
    for (int j = 0; j < right1; j++) {
      if (!((mright1 >> j) & 1))
        continue;
      mask1_self[mright1] &= get_mask(j + left, left, right1);
      mask1_left[mright1] &= get_mask(j + left, 0, left);
      mask1_right2[mright1] &= get_mask(j + left, left + right1, right2);
    }
  }
  u64 result = 0;
  for (int mright2 = 0; mright2 < (1 << right2); mright2++) {
    int mask2_self = mright2;
    int mask2_left = (1 << left) - 1;
    int mask2_right1 = (1 << right1) - 1;
    for (int j = 0; j < right2; j++) {
      if (!((mright2 >> j) & 1))
        continue;
      mask2_self &= get_mask(j + left + right1, left + right1, right2);
      mask2_left &= get_mask(j + left + right1, 0, left);
      mask2_right1 &= get_mask(j + left + right1, left, right1);
    }
    if (mask2_self < mright2)
      continue;
    for (int mright1 = 0; mright1 < (1 << right1); mright1++) {
      if ((mask2_right1 & mright1) != mright1)
        continue;
      if (mask1_self[mright1] < mright1)
        continue;
      if ((mask1_right2[mright1] & mright2) != mright2)
        continue;
      result += subsets[mask2_left & mask1_left[mright1]];
    }
  }
  out(result);
}
