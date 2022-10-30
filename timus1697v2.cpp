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
  D from, to;
  Range(): from(0), to(0) {}
  Range(D l, D r): from(min(l, r)), to(max(l, r)) {}
  bool contains_inclusive(D x) const {
    return (x > from - eps) && (x < to + eps);
  }
  bool contains_exclusive(D x) const {
    return (x > from + eps) && (x < to - eps);
  }
};

struct Line {
  D a, b, c;
  Line(): a(1), b(1), c(0) {}
  Line(D a, D b, D c): a(a), b(b), c(c) {}
  Line(D x1, D y1, D x2, D y2): Line(Point(x1, y1), Point(x2, y2)) {}
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
  Segment(D x1, D y1, D x2, D y2): p(x1, y1), q(x2, y2) {}
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
  D get_t(const Point& r) const {
    if (fabs(p.x - q.x) > fabs(p.y - q.y))
      return (r.x - p.x) / (q.x - p.x);
    return (r.y - p.y) / (q.y - p.y);
  }
  bool intersect_inclusive(const Segment& s, Point& result) const {
    if (!line().intersect(s.line(), result))
      return false;
    return contains_inclusive(result) && s.contains_inclusive(result);
  }
  bool intersect_inclusive(const Line& l, Point& result) const {
    if (!line().intersect(l, result))
      return false;
    return contains_inclusive(result);
  }
};

void solve(Input& in, Output& out) {
  int sx = in.ni();
  int sy = in.ni();
  int sz = in.ni();
  int ax = in.ni();
  int ay = in.ni();
  int bx = in.ni();
  int by = in.ni();

  auto relax = [&] (int x1, int y1, int x2, int y2, int h, pair<D, D>& res) {
    int a = y1 - y2;
    int b = x2 - x1;
    int c = -(a * x1 + b * y1);
    if (a * sx + b * sy + c == 0)
      return;
    D vx1 = sx + (x1 - sx) * 1000000000.0;
    D vy1 = sy + (y1 - sy) * 1000000000.0;
    D vx2 = sx + (x2 - sx) * 1000000000.0;
    D vy2 = sy + (y2 - sy) * 1000000000.0;
    if (sz > h) {
      vx1 = sx + (x1 - sx) * sz / (1.0 * sz - h);
      vy1 = sy + (y1 - sy) * sz / (1.0 * sz - h);
      vx2 = sx + (x2 - sx) * sz / (1.0 * sz - h);
      vy2 = sy + (y2 - sy) * sz / (1.0 * sz - h);
    }
    D mint = 1e100;
    D maxt = -1e100;

    Point r;
    if (Segment(Point(x1, y1), Point(x2, y2)).intersect_inclusive(Line(ax, ay, bx, by), r)) {
      D t = Segment(ax, ay, bx, by).get_t(r);
      mint = fmin(mint, t);
      maxt = fmax(maxt, t);
    }
    if (Segment(Point(x1, y1), Point(vx1, vy1)).intersect_inclusive(Line(ax, ay, bx, by), r)) {
      D t = Segment(ax, ay, bx, by).get_t(r);
      mint = fmin(mint, t);
      maxt = fmax(maxt, t);
    }
    if (Segment(Point(x2, y2), Point(vx2, vy2)).intersect_inclusive(Line(ax, ay, bx, by), r)) {
      D t = Segment(ax, ay, bx, by).get_t(r);
      mint = fmin(mint, t);
      maxt = fmax(maxt, t);
    }
    if (Segment(Point(vx1, vy1), Point(vx2, vy2)).intersect_inclusive(Line(ax, ay, bx, by), r)) {
      D t = Segment(ax, ay, bx, by).get_t(r);
      mint = fmin(mint, t);
      maxt = fmax(maxt, t);
    }
    if (mint > maxt)
      return;
    // out(x1, y1, x2, y2, mint, maxt);
    res.first = fmin(res.first, mint);
    res.second = fmax(res.second, maxt);
  };

  auto check = [&] (int lx, int ly, int rx, int ry, int h) {
    pair<D, D> res(1e100, -1e100);
    relax(lx, ly, rx, ly, h, res);
    relax(rx, ly, rx, ry, h, res);
    relax(rx, ry, lx, ry, h, res);
    relax(lx, ry, lx, ly, h, res);
    return res;
  };

  int n = in.ni();
  vec<pair<D, bool>> points;
  points.push_back({ -1e100, true });
  points.push_back({ -eps, false });
  points.push_back({ 1 + eps, true });
  points.push_back({ 1e100, false });
  for (int i = 0; i < n; i++) {
    int lx = in.ni();
    int ly = in.ni();
    int rx = in.ni();
    int ry = in.ni();
    int h = in.ni();

    auto range = check(lx, ly, rx, ry, h);
    if (range.first > range.second)
      continue;
    points.push_back({ range.first + eps, true });
    points.push_back({ range.second - eps, false });
  }
  sort(points.begin(), points.end());

  int overlap = 0;
  for (int i = 0; i < points.size() - 1; i++) {
    if (points[i].second)
      overlap++;
    else
      overlap--;
    if (!overlap) {
      out.setPrecision(12);
      D t = points[i].first + eps;
      out(ax + (bx - ax) * t, ay + (by - ay) * t);
      return;
    }
  }
  out("Impossible");
}
