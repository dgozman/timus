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
  int m = in.ni();
  int n = in.ni();
  auto w = newVec2d<int>(m, n, 0);
  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < n; ++j)
      w[i][j] = in.ni();
  }

  int layers = min(m, n) / 2;
  auto r = newVec3d<int>(layers, m / 2, n / 2, 0);

  auto process = [&](int layer, int row, int col, bool top_left) {
    int i = layer + row * 2;
    int j = layer + col * 2;
    int count = top_left ? w[i][j] : w[i + 1][j + 1];
    // out(layer, row, col, i, j, count);
    for (int l = 0; l < count; ++l) {
      int target_layer = (layer % 2) + 2 * l;
      int target_row = layer / 2 - l + row;
      int target_col = layer / 2 - l + col;
      if (r[target_layer][target_row][target_col] == 1)
        panic();
      r[target_layer][target_row][target_col] = 1;
    }
    w[i][j] -= count;
    w[i + 1][j] -= count;
    w[i][j + 1] -= count;
    w[i + 1][j + 1] -= count;
  };

  for (int layer = 0; layer < layers; ++layer) {
    for (int row = 0; row < m / 2 - layer; ++row) {
      process(layer, row, 0, true);
      process(layer, row, n / 2 - layer - 1, false);
    }
    for (int col = 1; col < n / 2 - layer - 1; ++col) {
      process(layer, 0, col, true);
      process(layer, m / 2 - layer - 1, col, false);
    }
  }

  // out(w);

  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < n; ++j) {
      if (w[i][j] != 0)
        panic();
    }
  }

  for (int layer = 0; layer < layers; ++layer) {
    for (int row = 0; row < m / 2 - layer; ++row) {
      for (int col = 0; col < n / 2 - layer; ++col)
        out.print(r[layer][row][col] ? "R" : "W");
      out.println();
    }
    out.println();
  }
}
