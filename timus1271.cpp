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

struct Point {
  i64 x, y;
  Point(i64 px, i64 py): x(px), y(py) {}
  Point operator+(const Point& o) const {
    return Point(x + o.x, y + o.y);
  }
  Point operator-(const Point& o) const {
    return Point(x - o.x, y - o.y);
  }
  double norm() const {
    return sqrt(x * x + y * y);
  }
  bool outside(const Point& D) const {
    return x < 0 || x > D.x || y < 0 || y > D.y;
  }
  i64 cross(const Point& o) const {
    return x * o.y - y * o.x;
  }
};

i64 area(const Point& a, const Point& b, const Point& c) {
  return abs((b - a).cross(c - a));
}

const int STRICT = 0;
const int NONSTRICT = 1;
const int OUTSIDE = 2;
struct Triangle {
  Point p1, p2, p3;
  i64 s;
  Triangle(Point pt1, Point pt2, Point pt3): p1(pt1), p2(pt2), p3(pt3) {
    s = area(pt1, pt2, pt3);
  }
  int contains(const Point& p) const {
    i64 s1 = area(p1, p2, p);
    i64 s2 = area(p1, p3, p);
    i64 s3 = area(p2, p3, p);
    if (s1 + s2 + s3 != s)
      return OUTSIDE;
    if (s1 > 0 && s2 > 0 && s3 > 0)
      return STRICT;
    return NONSTRICT;
  }
};

Point readPoint(Input& in) {
  return Point(in.ni64(), in.ni64());
}

Triangle readTriangle(Input& in) {
  return Triangle(readPoint(in), readPoint(in), readPoint(in));
}

bool triangleContainsAnother(const Triangle& t1, const Triangle& t2) {
  int r1 = t1.contains(t2.p1);
  int r2 = t1.contains(t2.p2);
  int r3 = t1.contains(t2.p3);
  if (t2.s > 0 && r1 != OUTSIDE && r2 != OUTSIDE && r3 != OUTSIDE)
    return true;
  if (r1 == STRICT || r2 == STRICT || r3 == STRICT)
    return true;
  return false;
}

bool trianglesContain(const Triangle& t1, const Triangle& t2) {
  return triangleContainsAnother(t1, t2) || triangleContainsAnother(t2, t1);
}

bool differentSides(const Point& p1, const Point& p2, const Point& q1, const Point& q2) {
  i64 a = p1.y - p2.y;
  i64 b = p2.x - p1.x;
  i64 c = -(a * p1.x + b * p1.y);
  i64 u = a * q1.x + b * q1.y + c;
  i64 v = a * q2.x + b * q2.y + c;
  return (u > 0 && v < 0) || (u < 0 && v > 0);
}

bool segmentsIntersect(const Point& p1, const Point& p2, const Point& q1, const Point& q2) {
  return differentSides(p1, p2, q1, q2) && differentSides(q1, q2, p1, p2);
}

bool triangleIntersectsWithSegment(const Triangle& t, const Point& p1, const Point& p2) {
  return segmentsIntersect(t.p1, t.p2, p1, p2) || segmentsIntersect(t.p2, t.p3, p1, p2) || segmentsIntersect(t.p3, t.p1, p1, p2);
}

bool canMove(Triangle t, Point from, Point to, vec<Triangle>& ts, Point D) {
  Triangle a(from, from + t.p2, from + t.p3);
  Triangle b(to, to + t.p2, to + t.p3);
  if (a.p1.outside(D) || a.p2.outside(D) || a.p3.outside(D))
    return false;
  if (b.p1.outside(D) || b.p2.outside(D) || b.p3.outside(D))
    return false;
  Triangle t12(a.p1, a.p2, b.p1);
  Triangle t21(a.p2, b.p1, b.p2);
  Triangle t23(a.p2, a.p3, b.p2);
  Triangle t32(a.p3, b.p2, b.p3);
  Triangle t31(a.p3, a.p1, b.p3);
  Triangle t13(a.p1, b.p3, b.p1);
  bool result = true;
  for (auto& tr : ts) {
    if (trianglesContain(tr, a) ||
        trianglesContain(tr, b) ||
        trianglesContain(tr, t12) ||
        trianglesContain(tr, t21) ||
        trianglesContain(tr, t23) ||
        trianglesContain(tr, t32) ||
        trianglesContain(tr, t31) ||
        trianglesContain(tr, t13)) {
      result = false;
      // return false;
    }
    if (triangleIntersectsWithSegment(tr, a.p1, a.p2) ||
        triangleIntersectsWithSegment(tr, a.p2, a.p3) ||
        triangleIntersectsWithSegment(tr, a.p3, a.p1) ||
        triangleIntersectsWithSegment(tr, b.p1, b.p2) ||
        triangleIntersectsWithSegment(tr, b.p2, b.p3) ||
        triangleIntersectsWithSegment(tr, b.p3, b.p1) ||
        triangleIntersectsWithSegment(tr, a.p1, b.p1) ||
        triangleIntersectsWithSegment(tr, a.p2, b.p2) ||
        triangleIntersectsWithSegment(tr, a.p3, b.p3) ||
        triangleIntersectsWithSegment(tr, a.p2, b.p1) ||
        triangleIntersectsWithSegment(tr, a.p3, b.p2) ||
        triangleIntersectsWithSegment(tr, a.p1, b.p3)) {
      result = false;
      // return false;
    }
  }
  return result;
}

void solve(Input& in, Output& out) {
  Point D(in.ni64(), in.ni64());
  Triangle t = readTriangle(in);
  Point end = readPoint(in);
  int n = in.ni();
  vec<Triangle> ts;
  for (int i = 0; i < n; i++)
    ts.push_back(readTriangle(in));

  vec<Point> v;
  v.push_back(t.p1);
  v.push_back(end);

  t.p2 = t.p2 - t.p1;
  t.p3 = t.p3 - t.p1;
  t.p1 = t.p1 - t.p1;

  for (int i = 0; i < n; i++) {
    v.push_back(ts[i].p1);
    v.push_back(ts[i].p2);
    v.push_back(ts[i].p3);

    v.push_back(ts[i].p1 - t.p2);
    v.push_back(ts[i].p2 - t.p2);
    v.push_back(ts[i].p3 - t.p2);

    v.push_back(ts[i].p1 - t.p3);
    v.push_back(ts[i].p2 - t.p3);
    v.push_back(ts[i].p3 - t.p3);
  }

  int m = v.size();
  vec2d<double> d = newVec2d(m, m, -2.0);
  for (int i = 0; i < m; i++) {
    for (int j = i + 1; j < m; j++) {
      bool can = canMove(t, v[i], v[j], ts, D);
      if (can)
        d[i][j] = d[j][i] = (v[i] - v[j]).norm();
    }
  }
  for (int k = 0; k < m; k++) {
    for (int i = 0; i < m; i++) {
      for (int j = 0; j < m; j++) {
        if (d[i][k] > -1.0 && d[k][j] > -1.0) {
          double dij = d[i][k] + d[k][j];
          if (d[i][j] < -1.0 || d[i][j] > dij)
            d[i][j] = dij;
        }
      }
    }
  }
  *out.out << std::setprecision(3) << std::fixed;
  if (d[0][1] < -1.0)
    out("-1");
  else
    out(d[0][1]);
}

/*
10 10
1 5 0 4 2 4
2 5
1
0 4 10 4 10 3

10 10
1 1 0 0 2 0
7 7
2
6 9 4 0 6 0
7 7 8 7 8 5

10 10
1 1 0 0 2 0
7 7
1
7 7 8 7 8 5
*/

