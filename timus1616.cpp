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
const D PI = 3.1415926535897932384626433832795028841971;

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
  bool operator<(const Point& p) const {
    return x <= p.x - eps || (fabs(x - p.x) < eps && y <= p.y - eps);
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
  D dot(const Point& p) const {
    return x * p.y - y * p.x;
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
  bool intersect(const Line& line, Point& result) const {
    D d = a * line.b - b * line.a;
    if (fabs(d) < eps)
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
  D contains_inclusive(const Point& r) const {
    return line().contains(r) &&
      Range(p.x, q.x).contains_inclusive(r.x) &&
      Range(p.y, q.y).contains_inclusive(r.y);
  }
  D contains_exclusive(const Point& r) const {
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
  D distance(const Point& r) const {
    return (r - closest(r)).norm();
  }
  bool intersect_inclusive(const Segment& s, Point& result) const {
    if (!line().intersect(s.line(), result))
      return false;
    return contains_inclusive(result) && s.contains_inclusive(result);
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

struct Polygon {
  vec<Point> p;
  int n = 0;
  bool closed = false;

  Polygon(): closed(false) {}
  Polygon& add(const Point& point) {
    if (closed)
      panic();
    p.push_back(point);
    return *this;
  }
  Polygon& close() {
    if (closed)
      panic();
    n = p.size();
    if (n)
      p.push_back(p[0]);
    closed = true;
    return *this;
  }
  void ensure_closed() const {
    if (!closed)
      panic();
  }
  bool empty() const {
    return !n;
  }

  Polygon(const vec<Point>& points) {
    p = points;
    close();
  }
  Polygon(vec<Point>&& points) {
    p = std::move(points);
    close();
  }

  D convex_area() const {
    ensure_closed();
    D area = 0;
    for (int i = 0; i < n; i++)
      area += p[i].dot(p[i + 1]);
    return fabs(area) / 2;
  }
  bool convex_contains_inclusive(const Point& q) const {
    ensure_closed();
    if (empty())
      return 0;
    Point center;
    for (int i = 0; i < n; i++)
      center = center + p[i];
    center = center / n;
    for (int i = 0; i < n; i++) {
      if (!Line(p[i], p[i + 1]).same_side_inclusive(q, center))
        return false;
    }
    return true;
  }

  Polygon convex_intersect(const Polygon& poly) const {
    ensure_closed();
    poly.ensure_closed();
    if (empty() || poly.empty())
      return Polygon().close();
    vec<Point> points;
    for (int i = 0; i < n; i++) {
      if (poly.convex_contains_inclusive(p[i]))
        points.push_back(p[i]);
    }
    for (int i = 0; i < poly.n; i++) {
      if (convex_contains_inclusive(poly.p[i]))
        points.push_back(poly.p[i]);
    }
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < poly.n; j++) {
        Point r;
        if (Segment(p[i], p[i + 1]).intersect_inclusive(Segment(poly.p[j], poly.p[j + 1]), r))
          points.push_back(r);
      }
    }
    if (!points.size())
      return Polygon().close();
    Point mn = points[0];
    for (Point& ps : points)
      mn = ps < mn ? ps : mn;
    sort(points.begin(), points.end(), [&mn](const Point& a, const Point& b) -> bool {
      if (fabs(a.x - mn.x) < eps && fabs(a.y - mn.y) < eps)
        return true;
      if (fabs(b.x - mn.x) < eps && fabs(b.y - mn.y) < eps)
        return false;
      D dot = (a - mn).dot(b - mn);
      if (dot > eps)
        return true;
      if (dot < eps)
        return false;
      return (a - mn).norm2() < (b - mn).norm2();
    });
    Polygon result;
    result.add(points[0]);
    for (int i = 0; i < points.size(); i++) {
      Point& a = result.p[result.p.size() - 1];
      Point& b = points[i];
      if (fabs(a.x - b.x) > eps || fabs(a.y - b.y) > eps)
        result.add(b);
    }
    return result.close();
  }

  void bounding_box(Point& mn, Point& mx) {
    mn = p[0];
    mx = p[0];
    for (int k = 1; k < n; k++) {
      mn.x = min(mn.x, p[k].x);
      mx.x = max(mx.x, p[k].x);
      mn.y = min(mn.y, p[k].y);
      mx.y = max(mx.y, p[k].y);
    }
  }
};

void solve(Input& in, Output& out) {
  int n = in.ni();
  D ang = in.next<double>() * PI / 180;
  D cosa = cos(ang);
  D sina = sin(ang);
  vec<string> w(2 * n);
  for (int i = 0; i < 2 * n; i++)
    w[i] = in.ns();

  vec3d<D> areas = newVec3d(4 * n, 4 * n, 26, (D) 0);
  for (int i = -n; i < n; i++) {
    for (int j = -n; j < n; j++) {
      if (w[i + n][j + n] == '.')
        continue;
      int owner = w[i + n][j + n] - 'a';
      Polygon p;
      p.add(Point(i, j).rotate(cosa, sina));
      p.add(Point(i + 1, j).rotate(cosa, sina));
      p.add(Point(i + 1, j + 1).rotate(cosa, sina));
      p.add(Point(i, j + 1).rotate(cosa, sina));
      p.close();
      Point mn, mx;
      p.bounding_box(mn, mx);

      int lx = max(-2 * n, (int) floor(mn.x) - 1);
      int rx = min(2 * n - 1, (int) ceil(mx.x));
      int ly = max(-2 * n, (int) floor(mn.y) - 1);
      int ry = min(2 * n - 1, (int) ceil(mx.y));
      for (int x = lx; x <= rx; x++) {
        for (int y = ly; y <= ry; y++) {
          Polygon q;
          q.add(Point(x, y));
          q.add(Point(x + 1, y));
          q.add(Point(x + 1, y + 1));
          q.add(Point(x, y + 1));
          q.close();
          areas[x + 2 * n][y + 2 * n][owner] += p.convex_intersect(q).convex_area();
        }
      }
    }
  }

  for (int i = -2 * n; i < 2 * n; i++) {
    for (int j = -2 * n; j < 2 * n; j++) {
      bool found = false;
      for (int k = 0; k < 26; k++) {
        if (areas[i + 2 * n][j + 2 * n][k] > 0.5 + 1e-10) {
          if (found)
            panic();
          out.print(string(1, 'a' + k));
          found = true;
        }
      }
      if (!found)
        out.print(string(1, '.'));
    }
    out.println();
  }
}
