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
  template<typename T, typename...Ts> inline void print(const T &f, const Ts&... args) {
    *out << f;
    if (sizeof...(args) != 0) { *out << ' '; print(args...); }
  }
  template<typename...Ts> inline void println(const Ts&... args) { print(args...); *out << '\n'; }
  template<typename T> inline void printVec(const vec<T>& v) { for (int i = 0; i < v.size(); i++) { if (i) *out << ' '; print(v[i]); } }
  template<typename T> inline void printlnVec(const vec<T>& v) { printVec(v); *out << '\n'; }
  template<typename...Ts> inline void operator() (const Ts&... args) { println(args...); }
  template<typename T> inline void operator() (const vec<T>& v) { printlnVec(v); }
};

template<typename T> vec2d<T> newVec2d(int n, int m, const T& init) {
  vec2d<T> v(n, vec<T>(m, init));
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

using Point = vec<double>;
const double eps = 1e-6;

vec<Point> rays(Point p1, double r1, Point p2, double r2) {
  vec<Point> result;
  // Scale the second sphere in such a way that intersection
  // plane of the two spheres passes through origin.
  double t =
      (r1 * r1 - p1[0] * p1[0] - p1[1] * p1[1] - p1[2] * p1[2]) /
      (r2 * r2 - p2[0] * p2[0] - p2[1] * p2[1] - p2[2] * p2[2]);
  if (t < -eps)
    return result;
  if (t < 0)
    t = 0;
  t = sqrt(t);
  r2 = r2 * t;
  p2[0] = p2[0] * t;
  p2[1] = p2[1] * t;
  p2[2] = p2[2] * t;

  // Find the intersection plane.
  double a = p2[0] - p1[0];
  double b = p2[1] - p1[1];
  double c = p2[2] - p1[2];
  double d = a * a + b * b + c * c;
  if (abs(d) < eps)
    return result;

  // Find the center of intersection circle by passing
  // a ray orthogonal to the plane from p1.
  t = - (a * p1[0] + b * p1[1] + c * p1[2]) / d;
  Point p0({ p1[0] + a * t, p1[1] + b * t, p1[2] + c * t});

  // Find the radius of intersection circle.
  double rs = r1 * r1 - d * t * t;
  if (rs < eps)
    rs = 0;
  if (rs < 0)
    return result;
  rs = sqrt(rs);

  // Cross-product p0 and (a, b, c) to get a vector from
  // the intersection center towards the interesting ray.
  double dx = p0[1] * c - p0[2] * b;
  double dy = p0[2] * a - p0[0] * c;
  double dz = p0[0] * b - p0[1] * a;
  double dlen = dx * dx + dy * dy + dz * dz;
  if (dlen < eps)
    return result;
  dlen = sqrt(dlen);
  double ratio = rs / dlen;

  // Two rays.
  result.push_back(Point({ p0[0] + dx * ratio, p0[1] + dy * ratio, p0[2] + dz * ratio }));
  result.push_back(Point({ p0[0] - dx * ratio, p0[1] - dy * ratio, p0[2] - dz * ratio }));
  return result;
}

double dist(const Point& p1, const Point& p2) {
  double d =
      (p1[0] - p2[0]) * (p1[0] - p2[0]) +
      (p1[1] - p2[1]) * (p1[1] - p2[1]) +
      (p1[2] - p2[2]) * (p1[2] - p2[2]);
  if (d < 0)
    d = 0;
  return sqrt(d);
}

void solve(Input& in, Output& out) {
  int n = in.ni();
  vec<Point> ps(n);
  vec<double> rs(n);
  for (int i = 0; i < n; i++) {
    ps[i] = in.nvec<double>(3);
    rs[i] = in.next<double>();
  }

  vec<int> result;
  vec<int> list;
  list.reserve(n);

  auto relax = [&] (Point p) {
    list.resize(0);
    for (int i = 0; i < n; i++) {
      double t = (p[0] * ps[i][0] + p[1] * ps[i][1] + p[2] * ps[i][2]) / (p[0] * p[0] + p[1] * p[1] + p[2] * p[2]);
      double d = dist(ps[i], Point({ p[0] * t, p[1] * t, p[2] * t }));
      if (d < rs[i] + eps)
        list.push_back(i + 1);
    }
    if (list.size() > result.size())
      result = list;
  };

  for (int i = 0; i < n; i++)
    relax(ps[i]);

  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      auto points = rays(ps[i], rs[i], ps[j], rs[j]);
      for (auto& point : points)
        relax(point);
    }
  }

  out(result.size());
  out(result);
}
