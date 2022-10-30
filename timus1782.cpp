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

u8 min(u8 a, u8 b) {
  return a < b ? a : b;
}

void solve(Input& in, Output& out) {
  string s = in.ns();
  int n = s.length();
  vec<int> a(n);
  for (int i = 0; i < n; i++)
    a[i] = s[i] == '1' ? 1 : 0;

  int m = 18;
  int k = 101;
  vec2d<u8> is0 = newVec2d(n, m, (u8) 0);
  vec2d<u8> is1 = newVec2d(n, m, (u8) 0);
  for (int i = 0; i < n; i++) {
    is0[i][0] = a[i] ^ 1;
    is1[i][0] = a[i];
    for (int j = 1; j < m; j++) {
      int v = i - (1 << (j - 1));
      if (v >= 0) {
        is0[i][j] = is1[i][j - 1] & is0[v][j - 1];
        is1[i][j] = is0[i][j - 1] & is1[v][j - 1];
      }
    }
  }

  vec2d<u8> res_left = newVec2d(n, m, (u8) 0);
  for (int i = 0; i < n; i++) {
    res_left[i][0] = i == 0 ? 1 : min(k, res_left[i - 1][0] + 1);
    for (int j = 1; j < m; j++) {
      res_left[i][j] = res_left[i][j - 1];
      int v = i - (1 << j);
      if (is0[i][j] | is1[i][j]) {
        u8 prev = v >= 0 ? res_left[v][j] : 0;
        res_left[i][j] = min(res_left[i][j], prev + 1);
      }
    }
  }

  vec2d<u8> res_right = newVec2d(n, m, (u8) 0);
  for (int i = n - 1; i >= 0; i--) {
    res_right[i][0] = i == n - 1 ? 1 : min(k, res_right[i + 1][0] + 1);
    for (int j = 1; j < m; j++) {
      res_right[i][j] = res_right[i][j - 1];
      int v = i + (1 << j) - 1;
      if (v < n && (is0[v][j] | is1[v][j])) {
        u8 prev = v + 1 < n ? res_right[v + 1][j] : 0;
        res_right[i][j] = min(res_right[i][j], prev + 1);
      }
    }
  }

  int best_result = k;
  int best_l = 0;
  int best_r = 0;
  int best_j = 0;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      int result = j + res_left[i][j] + (i + 1 < n ? res_right[i + 1][j] : 0);
      if (result < best_result) {
        best_result = result;
        best_j = j;
        best_l = i;
        best_r = i + 1;
      }
    }
  }

  if (best_result == k) {
    out(-1);
    return;
  }

  vec<string> res;
  while (best_j >= 0) {
    while (best_l >= 0 && (best_j == 0 || res_left[best_l][best_j] != res_left[best_l][best_j - 1])) {
      int v = best_l - (1 << best_j);
      if (is0[best_l][best_j])
        res.push_back("front 0");
      else if (is1[best_l][best_j])
        res.push_back("front 1");
      else
        panic();
      best_l = v;
    }
    while (best_r < n && (best_j == 0 || res_right[best_r][best_j] != res_right[best_r][best_j - 1])) {
      int v = best_r + (1 << best_j) - 1;
      if (is0[v][best_j])
        res.push_back("back 0");
      else if (is1[v][best_j])
        res.push_back("back 1");
      else
        panic();
      best_r = v + 1;
    }
    if (best_j > 0)
      res.push_back("double");
    best_j--;
  }

  if (res.size() != best_result)
    panic();
  out(res.size());
  for (auto& s : res)
    out(s);
}
