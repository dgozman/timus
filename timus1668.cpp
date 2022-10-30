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

using D = long double;
const D eps = 1e-15;

// Returns the number of non-free variables. These are first in the w and perm vectors.
int gauss(vec2d<D>& w, vec<int>& perm) {
  int m = w[0].size() - 1;
  perm.resize(m);
  for (int i = 0; i < m; i++)
    perm[i] = i;

  int i = 0;
  while (i < m && i < w.size()) {
    int maxr = i;
    int maxc = i;
    for (int r = i; r < w.size(); r++) {
      for (int c = i; c < m; c++) {
        if (fabs(w[r][c]) > fabs(w[maxr][maxc])) {
          maxr = r;
          maxc = c;
        }
      }
    }
    if (fabs(w[maxr][maxc]) < eps)
      break;
    for (int j = 0; j <= m; j++) {
      D tmp = w[i][j];
      w[i][j] = w[maxr][j];
      w[maxr][j] = tmp;
    }
    int tperm = perm[i];
    perm[i] = perm[maxc];
    perm[maxc] = tperm;
    for (int j = 0; j < w.size(); j++) {
      D tmp = w[j][i];
      w[j][i] = w[j][maxc];
      w[j][maxc] = tmp;
    }

    for (int j = 0; j < w.size(); j++) {
      if (j == i)
        continue;
      D t = w[j][i] / w[i][i];
      for (int k = 0; k <= m; k++)
        w[j][k] = w[j][k] - w[i][k] * t;
    }

    D s = 0;
    for (int j = 0; j < w.size(); j++) {
      for (int k = 0; k < m; k++)
        s += w[j][k];
    }
    if (fabs(s) > eps) {
      for (int j = 0; j < w.size(); j++) {
        for (int k = 0; k < m + 1; k++)
          w[j][k] /= s;
      }
    }
    i++;
  }

  bool solution = true;
  for (int r = i; r < w.size(); r++) {
    if (fabs(w[r][m]) > eps)
      solution = false;
  }
  if (!solution)
    return -1;
  return i;
}

void restore(vec2d<D>& w, vec<D>& x, int dep) {
  int m = w[0].size() - 1;
  for (int i = 0; i < dep; i++) {
    x[i] = w[i][m];
    for (int j = dep; j < m; j++)
      x[i] -= w[i][j] * x[j];
    x[i] /= w[i][i];
  }
}

vec<D> permute(vec<D>& x, vec<int>& perm) {
  vec<D> result(x.size());
  for (int i = 0; i < x.size(); i++)
    result[perm[i]] = x[i];
  return result;
}

void solve(Input& in, Output& out) {
  int n = in.ni();
  int m = in.ni();
  // int n = 5;
  // int m = 3;
  // int iter = 0;
  // while (true) {
  //   if (!(++iter % 10000))
  //     out(iter);
  vec2d<D> a = newVec2d(m, n, (D) 0);
  vec<D> b(n);
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++)
      a[i][j] = in.ni();
      // a[i][j] = rand() % 10;
  }
  for (int i = 0; i < n; i++)
    b[i] = in.ni();
    // b[i] = rand() % 10;

  vec2d<D> w1 = newVec2d(m, m + 1, (D) 0);
  for (int k = 0; k < m; k++) {
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < m; j++) {
        w1[k][j] += a[j][i] * a[k][i];
      }
      w1[k][m] += b[i] * a[k][i];
    }
  }
  // out(w1);
  vec<int> perm1;
  int dep1 = gauss(w1, perm1);
  if (dep1 == -1)
    panic();
  // if (dep1 == -1) {
  //   out(a);
  //   out(b);
  //   break;
  // }
  // }

  vec<D> x1(m, (D) 0);
  // out(w1);
  if (dep1 < m) {
    int free = m - dep1;
    vec2d<D> w2 = newVec2d(free, free + 1, (D) 0);
    for (int p = 0; p < free; p++) {
      w2[p][p] = 1;
      for (int i = 0; i < dep1; i++) {
        D div = w1[i][i] * w1[i][i];
        for (int j = 0; j < free; j++)
          w2[p][j] += w1[i][p + dep1] * w1[i][j + dep1] / div;
        w2[p][free] += w1[i][p + dep1] * w1[i][m] / div;
      }
    }
    vec<int> perm2;
    int dep2 = gauss(w2, perm2);
    if (dep2 == -1)
      panic();
    vec<D> x2(free, (D) 0);
    restore(w2, x2, dep2);
    vec<D> y2 = permute(x2, perm2);
    for (int i = 0; i < free; i++)
      x1[i + dep1] = y2[i];
  }

  restore(w1, x1, dep1);
  vec<D> result = permute(x1, perm1);
  out.setPrecision(15);
  out(result);
}
