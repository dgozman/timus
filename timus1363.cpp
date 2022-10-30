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

void solve(Input& in, Output& out) {
  // https://tannerhelland.com/2012/12/28/dithering-eleven-algorithms-source-code.html

  int rows = in.ni();
  int cols = in.ni();
  vec2d<int> w = newVec2d(rows, cols, 0);
  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < cols; c++)
      w[r][c] = in.ni();
  }

  auto choose = [&] (int value) {
    return value < 128 ? 0 : 255;
  };

  vec2d<int> x = newVec2d(rows, cols, 0);
  vec2d<int> res = newVec2d(rows, cols, 0);

  auto check = [&] () {
    for (int r = 0; r < rows; r++) {
      for (int c = 0; c < cols; c++) {
        int sum = 0;
        int count = 0;
        for (int dr = -10; dr <= 10; dr++) {
          for (int dc = -10; dc <= 10; dc++) {
            if (r + dr >= 0 && r + dr < rows && c + dc >= 0 && c + dc < cols) {
              count++;
              sum += res[r + dr][c + dc] - w[r + dr][c + dc];
            }
          }
        }
        if (abs(sum) > 20 * count)
          return false;
      }
    }
    return true;
  };

  auto apply = [&] (const vec2d<int> m) {
    int col = 0;
    while (m[0][col] != -1)
      col++;
    for (int c = 0; c < col; c++) {
      if (m[0][c] != 0)
        panic();
    }
    int maxdr = m.size();
    int maxdc = m[0].size();

    for (int r = 0; r < rows; r++) {
      for (int c = 0; c < cols; c++)
        x[r][c] = 0;
    }

    for (int r = 0; r < rows; r++) {
      for (int c = 0; c < cols; c++) {
        int value = x[r][c] + w[r][c];
        res[r][c] = choose(value);
        int error = value - res[r][c];
        int sum = 0;
        for (int dr = 0; dr < maxdr; dr++) {
          for (int dc = 0; dc < maxdc; dc++) {
            if (m[dr][dc] != -1 && r + dr < rows && c + dc - col >= 0 && c + dc - col < cols)
              sum += m[dr][dc];
          }
        }
        if (sum == 0)
          continue;
        for (int dr = 0; dr < maxdr; dr++) {
          for (int dc = 0; dc < maxdc; dc++) {
            if (m[dr][dc] != -1 && r + dr < rows && c + dc - col >= 0 && c + dc - col < cols)
              x[r + dr][c + dc - col] += error * m[dr][dc] / sum;
          }
        }
      }
    }

    return check();
  };

  vec2d<int> floyd_steinberg({{0, -1, 7}, {3, 5, 1}});
  if (apply(floyd_steinberg)) {
    out(res);
    return;
  }

  vec2d<int> javis_judice_ninke({{0, 0, -1, 7, 5}, {3, 5, 7, 5, 3}, {1, 3, 5, 3, 1}});
  if (apply(javis_judice_ninke)) {
    out(res);
    return;
  }

  vec2d<int> stucki({{0, 0, -1, 8, 4}, {2, 4, 8, 4, 2}, {1, 2, 4, 2, 1}});
  if (apply(stucki)) {
    out(res);
    return;
  }

  vec2d<int> atkinson({{0, -1, 1, 1}, {1, 1, 1, 0}, {0, 1, 0, 0}});
  if (apply(atkinson)) {
    out(res);
    return;
  }

  vec2d<int> burkes({{0, 0, -1, 8, 4}, {2, 4, 8, 4, 2}});
  if (apply(burkes)) {
    out(res);
    return;
  }

  vec2d<int> sierra({{0, 0, -1, 5, 3}, {2, 4, 5, 4, 2}, {0, 2, 3, 2, 0}});
  if (apply(sierra)) {
    out(res);
    return;
  }

  vec2d<int> two_row_sierra({{0, 0, -1, 4, 3}, {1, 2, 3, 2, 1}});
  if (apply(two_row_sierra)) {
    out(res);
    return;
  }

  vec2d<int> sierra_lite({{0, -1, 2}, {1, 1, 0}});
  if (apply(sierra_lite)) {
    out(res);
    return;
  }

  panic();
}
