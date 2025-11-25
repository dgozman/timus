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


using D = long double;
const D eps = 1e-12;
const D PI = 3.1415926535897932384626433832795028841971;

D sqrt_(D x) {
  if (x > -eps && x < 0)
    return 0;
  return sqrt(x);
}

D abs_(D x) {
  return fabsl(x);
}

bool less_(D x, D y) {
  return x <= y - eps;
}

bool less_eq_(D x, D y) {
  return x < y + eps;
}

bool eq_(D x, D y) {
  return fabsl(x - y) < eps;
}

using Num = D;
const Num zero = 0;
const Num one = 1;

Num sqr_(Num x) {
  return x * x;
}

bool more_(Num x, Num y) {
  return !less_eq_(x, y);
}

bool more_eq_(Num x, Num y) {
  return !less_(x, y);
}

struct Point {
  Num x, y;
  Point(): x(zero), y(zero) {}
  Point(Num x, Num y): x(x), y(y) {}
  Point operator-(const Point& other) const {
    return Point(x - other.x, y - other.y);
  }
  Point operator+(const Point& other) const {
    return Point(x + other.x, y + other.y);
  }
  Point operator*(const Num& d) const {
    return Point(x * d, y * d);
  }
  Point operator/(const Num& d) const {
    return Point(x / d, y / d);
  }
  bool operator<(const Point& p) const {
    return less_(x, p.x) || (eq_(x, p.x) && less_(y, p.y));
  }
  bool eq(const Point& p) const {
    return eq_(x, p.x) && eq_(y, p.y);
  }
  Point rotate(Num cosa, Num sina) const {
    return Point(x * cosa - y * sina, y * cosa + x * sina);
  }
  Point rotate(Num ang) const {
    return rotate(cos(ang), sin(ang));
  }
  Num norm2() const {
    return sqr_(x) + sqr_(y);
  }
  Num norm() const {
    return sqrt_(norm2());
  }
  Num dot(const Point& p) const {
    return x * p.y - y * p.x;
  }
  void out(Output& out) const {
    out.println(x, y);
  }
};

struct Range {
  Num a, b;
  Range(): a(zero), b(zero) {}
  Range(Num l, Num r): a(min(l, r)), b(max(l, r)) {}
  bool contains_inclusive(D x) const {
    return less_eq_(a, x) && less_eq_(x, b);
  }
  bool contains_exclusive(D x) const {
    return less_(a, x) && less_(x, b);
  }
};

struct Line {
  Num a, b, c;
  Line(): a(one), b(one), c(zero) {}
  Line(D a, D b, D c): a(a), b(b), c(c) {}
  Line(const Point& p1, const Point& p2) {
    a = p1.y - p2.y;
    b = p2.x - p1.x;
    c = -a * p1.x - b * p1.y;
  }
  void out(Output& out) const {
    out.println(a, b, c);
  }
  Num apply(const Point& p) const {
    return a * p.x + b * p.y + c;
  }
  bool contains(const Point& p) const {
    return eq_(apply(p), zero);
  }
  Num norm() const {
    return sqrt_(norm2());
  }
  Num norm2() const {
    return sqr_(a) + sqr_(b);
  }
  Line shift_to(const Point& to, D d) const {
    return apply(to) < zero ? Line(a, b, c + d * norm()) : Line(a, b, c - d * norm());
  }
  Line shift_from(const Point& from, D d) const {
    return apply(from) > zero ? Line(a, b, c + d * norm()) : Line(a, b, c - d * norm());
  }
  Point orth(const Point& p) const {
    Num t = -apply(p) / norm2();
    return Point(a, b) * t + p;
  }
  bool same_side_inclusive(const Point& p, const Point& q) const {
    Num pv = apply(p);
    Num qv = apply(q);
    return (less_eq_(zero, pv) && less_eq_(zero, qv)) || (less_eq_(pv, zero) && less_eq_(qv, zero));
  }
  bool same_side_exclusive(const Point& p, const Point& q) const {
    Num pv = apply(p);
    Num qv = apply(q);
    return (less_(zero, pv) && less_(zero, qv)) || (less_(pv, zero) && less_(qv, zero));
  }
  bool intersect(const Line& line, Point& result) const {
    Num d = a * line.b - b * line.a;
    if (eq_(d, zero))
      return false;
    result.x = (-c * line.b + b * line.c) / d;
    result.y = (-a * line.c + c * line.a) / d;
    return true;
  }
};

struct Segment {
  Point p, q;
  Segment() {}
  Segment(const Point& p, const Point& q): p(p), q(q) {}
  Line line() const {
    return Line(p, q);
  }
  bool contains_inclusive(const Point& r) const {
    return line().contains(r) &&
      Range(p.x, q.x).contains_inclusive(r.x) &&
      Range(p.y, q.y).contains_inclusive(r.y);
  }
  bool contains_exclusive(const Point& r) const {
    return line().contains(r) &&
      Range(p.x, q.x).contains_exclusive(r.x) &&
      Range(p.y, q.y).contains_exclusive(r.y);
  }
  Point closest(const Point& r) const {
    Point o = line().orth(r);
    if (contains_inclusive(o))
      return o;
    return (p - r).norm2() < (q - r).norm2() ? p : q;
  }
  Num distance(const Point& r) const {
    return (r - closest(r)).norm();
  }
  bool intersect_inclusive(const Segment& s, Point& result) const {
    if (!line().intersect(s.line(), result))
      return false;
    return contains_inclusive(result) && s.contains_inclusive(result);
  }
};

bool is_double_intersect(Point p1, Point p2, Point q1, Point q2, Point q3) {
  vec<Point> points;
  auto check = [&](Point from, Point to) {
    Point r;
    if (Segment(from, to).intersect_inclusive(Segment(p1, p2), r))
      points.push_back(r);
  };
  check(q1, q2);
  check(q1, q3);
  check(q2, q3);
  if (points.size() >= 3 && points[1].eq(points[2]))
    points.pop_back();
  if (points.size() >= 3 && points[0].eq(points[2]))
    points.pop_back();
  if (points.size() >= 2 && points[0].eq(points[1]))
    points.erase(points.begin());
  return points.size() >= 2;
}

D area3(Point a, Point b, Point c) {
  return abs_((b - a).dot(c - a)) / 2;
}

bool is_inside3(Point p, Point a, Point b, Point c) {
  if (!more_eq_((b - a).dot(c - a), zero)) {
    Point temp = b;
    b = c;
    c = temp;
  }
  Num abp = (b - a).dot(p - a);
  Num bcp = (c - b).dot(p - b);
  Num cap = (a - c).dot(p - c);
  return more_eq_(abp, zero) && more_eq_(bcp, zero) && more_eq_(cap, zero);
}

void solve(Input& in, Output& out) {
  Point f(in.next<D>(), in.next<D>());
  Point a(0, 0);
  Point b(10, 0);
  Point c(10, 10);
  Point z(0, 0);

  out.setPrecision(20);

  D result = 0;

  for (int iter = 0; iter < 10000; iter++) {
    // out("a", a.x, a.y, "b", b.x, b.y, "c", c.x, c.y);

    Point d = (a + c) / 2;
    if (is_double_intersect(z, f, a, b, d)) {
      // out("case1");
      result += area3(a, b, d);
      a = b;
      b = d;
    } else if (is_double_intersect(z, f, c, b, d)) {
      // out("case2");
      result += area3(c, b, d);
      c = b;
      b = d;
    } else if (is_inside3(f, a, b, d)) {
      // out("case3");
      c = b;
      b = d;
    } else {
      // out("case4");
      a = b;
      b = d;
    }
  }

  out(result);
}
