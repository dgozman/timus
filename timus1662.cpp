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


using D = long double;
const D eps = 1e-10;

D sqr_(D x) {
  return x * x;
}

D sqrt_(D x) {
  if (x > -eps && x < 0)
    return 0;
  return sqrt(x);
}

struct Point {
  D x, y;
  Point(): x(0), y(0) {}
  Point(D x, D y): x(x), y(y) {}
  Point operator-(const Point& other) const {
    return Point(x - other.x, y - other.y);
  }
  Point operator+(const Point& other) const {
    return Point(x + other.x, y + other.y);
  }
  Point operator*(const D& d) const {
    return Point(x * d, y * d);
  }
  Point operator/(const D& d) const {
    return Point(x / d, y / d);
  }
  Point rotate(D cosa, D sina) const {
    return Point(x * cosa - y * sina, y * cosa + x * sina);
  }
  Point rotate(D ang) const {
    return rotate(cos(ang), sin(ang));
  }
  D norm2() const {
    return sqr_(x) + sqr_(y);
  }
  D norm() const {
    return sqrt_(norm2());
  }
  void out(Output& out) const {
    out.println(x, y);
  }
};

struct Range {
  D a, b;
  Range(): a(0), b(0) {}
  Range(D l, D r): a(min(l, r)), b(max(l, r)) {}
  bool contains_inclusive(D x) const {
    return (x > a - eps) && (x < b + eps);
  }
  bool contains_exclusive(D x) const {
    return (x > a + eps) && (x < b - eps);
  }
};

struct Line {
  D a, b, c;
  Line(): a(1), b(1), c(0) {}
  Line(D a, D b, D c): a(a), b(b), c(c) {}
  Line(const Point& p1, const Point& p2) {
    a = p1.y - p2.y;
    b = p2.x - p1.x;
    c = -a * p1.x - b * p1.y;
  }
  void out(Output& out) const {
    out.println(a, b, c);
  }
  D apply(const Point& p) const {
    return a * p.x + b * p.y + c;
  }
  bool contains(const Point& p) const {
    return fabs(apply(p)) < eps;
  }
  D norm() const {
    return sqrt_(norm2());
  }
  D norm2() const {
    return sqr_(a) + sqr_(b);
  }
  Line shift_to(const Point& to, D d) const {
    return apply(to) < 0 ? Line(a, b, c + d * norm()) : Line(a, b, c - d * norm());
  }
  Line shift_from(const Point& from, D d) const {
    return apply(from) > 0 ? Line(a, b, c + d * norm()) : Line(a, b, c - d * norm());
  }
  Point orth(const Point& p) const {
    D t = -apply(p) / norm2();
    return Point(a, b) * t + p;
  }
  bool same_side_inclusive(const Point& p, const Point& q) const {
    D pv = apply(p);
    D qv = apply(q);
    return (pv > -eps && qv > -eps) || (pv < eps && qv < eps);
  }
  bool same_side_exclusive(const Point& p, const Point& q) const {
    D pv = apply(p);
    D qv = apply(q);
    return (pv > eps && qv > eps) || (pv < -eps && qv < -eps);
  }
};

struct Segment {
  Point p, q;
  Segment() {}
  Segment(const Point& p, const Point& q): p(p), q(q) {}
  D contains_inclusive(const Point& r) const {
    return Line(p, q).contains(r) &&
      Range(p.x, q.x).contains_inclusive(r.x) &&
      Range(p.y, q.y).contains_inclusive(r.y);
  }
  D contains_exclusive(const Point& r) const {
    return Line(p, q).contains(r) &&
      Range(p.x, q.x).contains_exclusive(r.x) &&
      Range(p.y, q.y).contains_exclusive(r.y);
  }
  Point closest(const Point& r) const {
    Point o = Line(p, q).orth(r);
    if (contains_inclusive(o))
      return o;
    return (p - r).norm2() < (q - r).norm2() ? p : q;
  }
  D distance(const Point& r) const {
    return (r - closest(r)).norm();
  }
};

struct Circle {
  Point c;
  D r;
  Circle(): r(0) {}
  Circle(const Point& c, D r): c(c), r(r) {}
  Circle(D x, D y, D r): c(x, y), r(r) {}
  void out(Output& out) const {
    out.println(c.x, c.y, r);
  }
  bool intersect(const Line& line, Segment& result) const {
    D norm2 = line.norm2();
    D t = -line.apply(c) / norm2;
    D r2 = r * r - t * t * norm2;
    if (r2 < -eps)
      return false;
    r2 = sqrt_(r2 / norm2);
    Point p(c.x + line.a * t, c.y + line.b * t);
    result = Segment(
      Point(p.x + line.b * r2, p.y - line.a * r2),
      Point(p.x - line.b * r2, p.y + line.a * r2)
    );
    return true;
  }
  bool intersect(const Circle& circle, Segment& result) const {
    D la = 2 * (circle.c.x - c.x);
    D lb = 2 * (circle.c.y - c.y);
    if (fabs(la) < eps && fabs(lb) < eps)
      return false;
    D lc = sqr_(circle.r) - sqr_(r) + sqr_(c.x) - sqr_(circle.c.x) + sqr_(c.y) - sqr_(circle.c.y);
    return intersect(Line(la, lb, lc), result);
  }
};


void solve(Input& in, Output& out) {
  int n = in.ni();
  vec<Point> p(n + 1);
  Point center;
  for (int i = 0; i < n; i++) {
    p[i].x = in.ni();
    p[i].y = in.ni();
    center = center + p[i];
  }
  p[n] = p[0];
  center = center / n;
  D r = in.ni();
  D R = in.ni();

  auto check = [&] (const Point& q) {
    bool inside = true;
    for (int i = 0; i < n; i++) {
      D d = (p[i] - q).norm();
      if (d > R + eps)
        return false;
      if (d < r - eps)
        return false;
      if (Segment(p[i], p[i + 1]).distance(q) < r - eps)
        return false;
      if (!Line(p[i], p[i + 1]).same_side_inclusive(center, q))
        inside = false;
    }
    if (inside)
      return false;
    out.setPrecision(6);
    out(q.x, q.y);
    return true;
  };

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      Segment s;
      if (Circle(p[i], R).intersect(Circle(p[j], r), s)) {
        if (check(s.p))
          return;
        if (check(s.q))
          return;
      }
      Line shifted = Line(p[j], p[j + 1]).shift_from(center, r);
      if (check(shifted.orth(p[i])))
        return;
      if (Circle(p[i], R).intersect(shifted, s)) {
        if (check(s.p))
          return;
        if (check(s.q))
          return;
      }
    }
  }
  out("No solution");
}
