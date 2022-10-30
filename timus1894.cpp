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

struct Vec {
  i64 x, y;
  Vec(): x(0), y(0) {}
  Vec(i64 x, i64 y): x(x), y(y) {}
  i64 dot(const Vec& v) const {
    return x * v.y - y * v.x;
  }
  Vec neg() const {
    return Vec(-x, -y);
  }
  i64 cross(const Vec& v) const {
    return x * v.x + y * v.y;
  }
  double norm() const {
    return sqrt(x * x + y * y);
  }
};

void solve(Input& in, Output& out) {
  int n = in.ni();
  int m = in.ni();
  vec<i64> xn(3 * n), yn(3 * n), xm(3 * m), ym(3 * m);
  for (int i = 0; i < n; i++) {
    xn[i] = xn[i + n] = xn[i + 2 * n] = in.ni();
    yn[i] = yn[i + n] = yn[i + 2 * n] = in.ni();
  }
  for (int i = 0; i < m; i++) {
    xm[i] = xm[i + m] = xm[i + 2 * m] = in.ni();
    ym[i] = ym[i + m] = ym[i + 2 * m] = in.ni();
  }
  vec<Vec> vn(3 * n);
  for (int i = 1; i < 3 * n -1; i++)
    vn[i] = Vec(xn[i] - xn[i - 1], yn[i] - yn[i - 1]);
  vec<Vec> vm(3 * m);
  for (int i = 1; i < 3 * m -1; i++)
    vm[i] = Vec(xm[i - 1] - xm[i], ym[i - 1] - ym[i]);

  auto get_dist = [&] (int i, int j, const Vec& v) {
    if (v.x == 0 && v.y == 0)
      return 1e100;
    double d = v.dot(Vec(xn[i] - xm[j], yn[i] - ym[j]));
    d = fabs(d / v.norm());
    return d;
  };

  auto more = [&] (const Vec& v1, const Vec& v2) {
    return v1.dot(v2) < 0;
  };

  auto eq = [&] (const Vec& v1, const Vec& v2) {
    return v1.dot(v2) == 0;
  };

  auto check = [&] (int i, int j) {
    double res = 1e100;
    Vec left = more(vn[i], vm[j]) ? vn[i] : vm[j];
    Vec right = more(vn[i + 1], vm[j + 1]) ? vm[j + 1] : vn[i + 1];
    res = min(res, get_dist(i, j, left));
    res = min(res, get_dist(i, j, right));
    {
      Vec d(xm[j] - xn[i], ym[j] - yn[i]);
      if (!more(left, d) && !more(d, right))
        res = min(res, get_dist(i, j, d));
    }
    {
      Vec d(-xm[j] + xn[i], -ym[j] + yn[i]);
      if (!more(left, d) && !more(d, right))
        res = min(res, get_dist(i, j, d));
    }
    // out(i, j, res);
    return res;
  };

  int i = 1;
  int sj = -1;
  for (int k = 1; k <= m; k++) {
    if (!more(vm[k + 1], vn[i + 1]))
      continue;
    if (sj == -1 || more(vm[sj + 1], vm[k + 1]))
      sj = k;
  }

  int j = sj;
  double result = 1e100;
  while (i <= 1 + n || j <= sj + m) {
    result = min(result, check(i, j));
    if (eq(vn[i + 1], vm[j + 1]))
      result = min(result, check(i, j + 1));
    if (more(vn[i + 1], vm[j + 1]))
      j++;
    else
      i++;
  }

  result = max((double) 0, result - 60);
  out.setPrecision(10);
  out(result);
}

/*


4 4
0 0
1000 0
1000 1000
0 1000
0 0
1 -1
1001 999
1000 1000
// 647.1067811865

4 4
0 0
1000 0
1000 1000
0 1000
300 -1000
500 -1000
500 2000
300 2000
// 440

4 4
0 0
2000 -2000
4800 0
2000 2000
4000 0
6000 -2000
8000 0
6000 2000
// 505.6854249492


4 4
400 400
600 400
600 600
400 600
0 0
1000 0
1000 1000
0 1000
// 540


*/
