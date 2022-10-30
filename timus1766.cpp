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
  template<typename A, typename B> inline void printOne(const pair<A, B>& pair) { print(pair.first, pair.second); }
  template<typename T, typename...Ts> inline void print(const T &f, const Ts&... args) {
    *out << f;
    if (sizeof...(args) != 0) { *out << ' '; print(args...); }
  }
  template<typename...Ts> inline void println(const Ts&... args) { print(args...); *out << '\n'; }
  template<typename...Ts> inline void operator() (const Ts&... args) { println(args...); }
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

void solve(Input& in, Output& out) {
  vec2d<int> z = newVec2d(8, 8, 0);
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++)
      z[i][j] = in.ni();
  }

  vec2d<D> w = newVec2d(65, 65, (D) 0);
  for (int i = 0; i < 64; i++) {
    w[i][i] = -1;
    w[64][i] = 1;
  }
  w[64][64] = 1;

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      int sum = 0;
      for (int di = -1; di <= 1; di++) {
        for (int dj = -1; dj <= 1; dj++) {
          if (di == 0 && dj == 0)
            continue;
          if (i + di < 0 || i + di >= 8 || j + dj < 0 || j + dj >= 8)
            continue;
          sum += z[i + di][j + dj];
        }
      }
      for (int di = -1; di <= 1; di++) {
        for (int dj = -1; dj <= 1; dj++) {
          if (di == 0 && dj == 0)
            continue;
          if (i + di < 0 || i + di >= 8 || j + dj < 0 || j + dj >= 8)
            continue;
          int to = (i + di) * 8 + (j + dj);
          int from = i * 8 + j;
          w[to][from] = ((D) 1) * z[i + di][j + dj] / sum;
        }
      }
    }
  }

  D eps = 1e-20;
  for (int i = 0; i < 64; i++) {
    int max = i;
    for (int j = i + 1; j < 65; j++) {
      if (fabs(w[j][i]) > fabs(w[max][i]))
        max = j;
    }
    if (fabs(w[max][i]) < eps)
      panic();
    for (int j = 0; j < 65; j++) {
      D tmp = w[i][j];
      w[i][j] = w[max][j];
      w[max][j] = tmp;
    }
    for (int j = 0; j < 65; j++) {
      if (j == i)
        continue;
      D t = w[j][i] / w[i][i];
      for (int k = 0; k < 65; k++)
        w[j][k] = w[j][k] - w[i][k] * t;
    }
    D s = 0;
    for (int j = 0; j < 65; j++) {
      for (int k = 0; k < 64; k++)
        s += w[j][k];
    }
    if (fabs(s) > eps) {
      for (int j = 0; j < 65; j++) {
        for (int k = 0; k < 65; k++)
          w[j][k] /= s;
      }
    }
  }

  vec<D> r(64);
  for (int i = 0; i < 64; i++)
    r[i] = w[i][64] / w[i][i];
  *out.out << std::setprecision(15) << std::fixed;
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (j > 0)
        out.print(" ");
      out.print(r[8 * i + j]);
    }
    out.println();
  }
}
