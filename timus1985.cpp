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

bool is_prime(int n) {
  for (int i = 2; i * i <= n; i++) {
    if (n % i == 0)
      return false;
  }
  return true;
}

void solve(Input& in, Output& out) {
  int n = in.ni();
  if (n == 1) {
    out(1);
    return;
  }
  if (n == 2) {
    out(2, 1);
    out(3, 4);
    return;
  }
  if (n == 3) {
    out(1, 6, 7);
    out(2, 5, 8);
    out(3, 4, 9);
    return;
  }

  auto fill_2 = [&](vec2d<int>& a, int row, int x) {
    int y = x + 1;
    while (!is_prime(y))
      y++;
    int mid = x + (y - x) / 2;
    int j = 0;
    while (x < mid) {
      a[row][j] = x;
      x++;
      j++;
    }
    j--;
    while (j >= 0) {
      a[row + 1][j] = x;
      x++;
      j--;
    }
    return x;
  };

  auto fill_rest = [&](vec2d<int>& a, int n, int x) {
    int row = n - 1;
    int col = 0;
    int going_right = true;
    while (row >= 0) {
      int ncol = col + (going_right ? 1 : -1);
      if (ncol < 0 || ncol >= n || a[row][ncol] != 0) {
        row--;
        going_right = !going_right;
      } else {
        col = ncol;
      }
      if (row < 0)
        break;
      if (a[row][col] != 0)
        panic();
      a[row][col] = x++;
    }
    if (x != n * n + 1)
      panic();
  };

  auto a = newVec2d<int>(n, n, 0);
  if (n % 2) {
    a[0][0] = 3;
    a[0][1] = 4;
    a[0][2] = 5;
    a[1][0] = 2;
    a[1][1] = 1;
    a[1][2] = 6;
    a[2][0] = 11;
    a[2][1] = 10;
    a[2][2] = 7;
    a[3][0] = 12;
    a[3][1] = 9;
    a[3][2] = 8;
    int x = 13;
    for (int i = 4; i < n - 1; i += 2)
      x = fill_2(a, i, x);
    a[n - 1][0] = x;
    fill_rest(a, n, x + 1);
  } else {
    a[0][0] = 2;
    a[0][1] = 1;
    int x = 3;
    for (int i = 1; i < n - 1; i += 2)
      x = fill_2(a, i, x);
    a[n - 1][0] = x;
    fill_rest(a, n, x + 1);
  }
  out(a);
}
