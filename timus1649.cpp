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
using u16 = uint16_t;
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
  int rows = in.ni();
  int cols = in.ni();
  // int rows = 20;
  // int cols = 20;
  int n = rows * cols;
  vec2d<int> w = newVec2d(rows, cols, 0);

  // vec<int> perm(n);
  // srand(43);
  // for (int i = 0; i < n; i++) {
  //   int j = rand() % (i + 1);
  //   perm[i] = perm[j];
  //   perm[j] = i;
  // }
  // for (int i = 0; i < n; i++) {
  //   int r = perm[i] / cols;
  //   int c = perm[i] % cols;
  //   w[r][c] = 1;
  //   if (r > 0 && w[r - 1][c] > 0)
  //     w[r - 1][c] = w[r - 1][c] == 4 ? 1 : w[r - 1][c] + 1;
  //   if (c > 0 && w[r][c - 1] > 0)
  //     w[r][c - 1] = w[r][c - 1] == 4 ? 1 : w[r][c - 1] + 1;
  //   if (r + 1 < rows && w[r + 1][c] > 0)
  //     w[r + 1][c] = w[r + 1][c] == 4 ? 1 : w[r + 1][c] + 1;
  //   if (c + 1 < cols && w[r][c + 1] > 0)
  //     w[r][c + 1] = w[r][c + 1] == 4 ? 1 : w[r][c + 1] + 1;
  // }
  // w[0][0] = w[0][0] == 4 ? 1 : w[0][0] + 1;

  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < cols; c++)
      w[r][c] = in.ni();
  }

  vec<u8> near_size(n, 0);
  vec<std::array<u16, 4>> near(n);
  vec<u8> deg(n, 0);
  vec<u8> value(n);
  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < cols; c++) {
      u16 i = r * cols + c;
      value[i] = w[r][c];
      if (r > 0)
        near[i][deg[i]++] = (r - 1) * cols + (c);
      if (c > 0)
        near[i][deg[i]++] = (r) * cols + (c - 1);
      if (r + 1 < rows)
        near[i][deg[i]++] = (r + 1) * cols + (c);
      if (c + 1 < cols)
        near[i][deg[i]++] = (r) * cols + (c + 1);
    }
  }
  for (int i = 0; i < n; i++)
    near_size[i] = deg[i];

  vec<u16> result(n);
  function<bool(u16, u16, u16)> go = [&] (u16 len, u16 need, u16 min_free) {
    if (len == need)
      return true;

    auto take = [&] (u16 x, u16 new_min_free) {
      result[len] = x;

      value[x] = 0;

      bool bad = false;
      for (u8 j = 0; j < near_size[x]; j++) {
        u16 y = near[x][j];
        if (value[y] > 0) {
          value[y]--;
          if (value[y] == 0)
            value[y] = 4;
          if (value[y] > deg[y] + 1)
            bad = true;
          deg[y]--;
        }
      }

      if (!bad && go(len + 1, need, new_min_free))
        return true;

      for (u8 j = 0; j < near_size[x]; j++) {
        u16 y = near[x][j];
        if (value[y] > 0) {
          value[y]++;
          if (value[y] == 5)
            value[y] = 1;
          deg[y]++;
        }
      }

      value[x] = 1;

      return false;
    };

    for (u16 x = 0; x < n; x++) {
      u16 v = value[x];
      if (v > 0 && v == deg[x] + 1) {
        result[need - 1] = x;
        value[x] = 0;
        bool bad = false;
        for (u8 j = 0; j < near_size[x]; j++) {
          u16 y = near[x][j];
          if (value[y] > 0) {
            deg[y]--;
            if (value[y] > deg[y] + 1)
              bad = true;
          }
        }
        if (!bad && go(len, need - 1, min_free))
          return true;
        for (u8 j = 0; j < near_size[x]; j++) {
          u16 y = near[x][j];
          if (value[y] > 0)
            deg[y]++;
        }
        value[x] = v;
        return false;
      }
    }
    for (u16 x = 0; x < n; x++) {
      if (value[x] == 1 && deg[x] < 4)
        return take(x, min_free);
    }

    for (u16 x = 0; x < n; x++) {
      if (value[x] == 1) {
        for (u8 j = 0; j < near_size[x]; j++) {
          u16 y = near[x][j];
          if (value[y] == 1) {
            if (take(x, min_free))
              return true;
            return take(y, min_free);
          }
        }
      }
    }

    for (u16 x = min_free; x < n; x++) {
      if (value[x] == 1) {
        if (take(x, x + 1))
          return true;
      }
    }
    return false;
  };

  bool is_bad = false;
  for (u16 x = 0; x < n; x++) {
    if (value[x] > deg[x] + 1)
      is_bad = true;
  }

  // out(w);
  if (!is_bad && go(0, n, 0)) {
    out("Yes");
    for (int i = n - 1; i >= 0; i--) {
      int r = result[i] / cols;
      int c = result[i] % cols;
      out(r + 1, c + 1);
    }
  } else {
    out("No");
  }
}
