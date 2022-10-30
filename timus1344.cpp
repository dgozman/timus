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

using D = long double;

struct Rope {
  int other;
  int index;
  bool taken;
};

void solve(Input& in, Output& out) {
  // https://www.ropelab.com.au/pulley-system-analysis/

  int n = in.ni();
  vec<Rope> x1(n), x2(n), y(n);

  auto count = [&] (int value) {
    return value == -1 ? 0 : value < 0 ? 2 : 1;
  };

  int m = 0;
  for (int i = 0; i < n; i++) {
    in.ni();
    x1[i].other = in.ni() - 1;
    x2[i].other = in.ni() - 1;
    y[i].other = in.ni() - 1;
    if (x1[i].other == i || x2[i].other == i || y[i].other == i)
      panic();
    m += count(x1[i].other) + count(x2[i].other) + count(y[i].other);
  }
  if (m % 2)
    panic();
  m /= 2;

  vec2d<D> w;
  int taken = 0;
  int target = -1;

  auto allocate = [&] (Rope& rope, int i) {
    if (rope.other == -1)
      return;
    if (rope.other < -1 || rope.other > i) {
      if (taken == m)
        panic();
      rope.index = taken++;
      return;
    }
    rope.taken = true;
    if (x1[rope.other].other == i && !x1[rope.other].taken) {
      x1[rope.other].taken = true;
      rope.index = x1[rope.other].index;
    } else if (x2[rope.other].other == i && !x2[rope.other].taken) {
      x2[rope.other].taken = true;
      rope.index = x2[rope.other].index;
    } else if (y[rope.other].other == i && !y[rope.other].taken) {
      y[rope.other].taken = true;
      rope.index = y[rope.other].index;
    } else {
      panic();
    }
  };

  auto handle = [&] (Rope& rope) {
    if (rope.other == -2) {
      vec<D> v(m + 1, (D) 0.0);
      v[rope.index] = 1;
      v[m] = 1;
      w.push_back(std::move(v));
      return;
    }
    if (rope.other == -3) {
      if (target != -1)
        panic();
      target = rope.index;
      return;
    }
  };

  for (int i = 0; i < n; i++) {
    allocate(x1[i], i);
    allocate(x2[i], i);
    allocate(y[i], i);
    if (x1[i].other != -1 && x2[i].other != -1) {
      vec<D> v(m + 1, (D) 0.0);
      v[x1[i].index] = 1;
      v[x2[i].index] = -1;
      w.push_back(std::move(v));
    }
    if (y[i].other != -1) {
      if (x1[i].other != -1) {
        vec<D> v(m + 1, (D) 0.0);
        v[y[i].index] = 1;
        v[x1[i].index] = -2;
        w.push_back(std::move(v));
      } else if (x2[i].other != -1) {
        vec<D> v(m + 1, (D) 0.0);
        v[y[i].index] = 1;
        v[x2[i].index] = -2;
        w.push_back(std::move(v));
      }
    }
    handle(x1[i]);
    handle(x2[i]);
    handle(y[i]);
  }
  if (target == -1)
    panic();
  if (taken != m)
    panic();

  vec<int> perm(m);
  for (int i = 0; i < m; i++)
    perm[i] = i;

  D eps = 1e-20;
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

    // D s = 0;
    // for (int j = 0; j < 65; j++) {
    //   for (int k = 0; k < 64; k++)
    //     s += w[j][k];
    // }
    // if (fabs(s) > eps) {
    //   for (int j = 0; j < 65; j++) {
    //     for (int k = 0; k < 65; k++)
    //       w[j][k] /= s;
    //   }
    // }
    i++;
  }

  bool solution = true;
  for (int r = i; r < w.size(); r++) {
    if (fabs(w[r][m]) > eps)
      solution = false;
  }
  if (!solution) {
    out("No solution");
    return;
  }

  vec<bool> any(m, false);
  vec<D> result(m, (D) 0.0);
  for (int r = i; r < m; r++)
    any[r] = true;

  for (int r = 0; r < i; r++) {
    for (int j = i; j < m; j++) {
      if (fabs(w[r][j]) > eps)
        any[r] = true;
    }
    result[r] = w[r][m] / w[r][r];
  }

  int target_p = -1;
  for (int r = 0; r < m; r++) {
    if (perm[r] == target)
      target_p = r;
  }
  if (target_p == -1)
    panic();

  if (any[target_p]) {
    out("Any");
  } else {
    out.setPrecision(4);
    out(result[target_p]);
  }
}

/*

4
2 0 2 -1
2 0 3 1
2 0 4 2
1 3 -2 0

0.125

*/