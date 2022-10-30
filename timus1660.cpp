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
  ifstream fin("input.txt");
  Input in(fin);
  // ofstream fout("output.txt");
  // Output out(fout);
  // Input in(cin);
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

void solve(Input& in, Output& out) {
  D R = in.ni();
  D r = in.ni();
  D d = in.ni();
  out.setPrecision(10);

  auto check = [&] (D rs) {
    D xs = 0;
    D ys = 0;
    if (fabs(d) < eps) {
      ys = 0;
      xs = R - rs;
    } else {
      Segment ss;
      if (!Circle(Point(0, 0), R - rs).intersect(Circle(Point(d, 0), r + rs), ss))
        panic();
      xs = ss.p.x;
      ys = ss.p.y;
    }
    D as = atan2(ys, xs);

    D xp = xs;
    D yp = ys;
    D rp = rs;
    D ap = as;
    D dp = 0;

    while (true) {
      D xn1 = 0;
      D yn1 = 0;
      D rn1 = 0;
      D xn2 = 0;
      D yn2 = 0;
      D rn2 = 0;

      if (fabs(yp) > eps && fabs(d) > eps) {
        D a = (-2*R - 2*r) / (2*d);
        D b = (d*d + R*R - r*r) / (2*d);

        D e = (-2*R - 2*rp) / (2*yp) - 2*xp*a / (2*yp);
        D f = (xp*xp + yp*yp + R*R - rp*rp) / (2*yp) - 2*xp*b/(2*yp);

        D sa = a*a + e*e - 1;
        D sb = 2*a*b + 2*e*f + 2*R;
        D sc = b*b + f*f - R*R;

        if (fabs(sa) < eps) {
          if (fabs(sb) < eps)
            break;
          rn1 = -sc / sb;
          xn1 = a*rn1 + b;
          yn1 = e*rn1 + f;
          rn2 = rn1;
          xn2 = xn1;
          yn2 = yn1;
        } else {
          D d = sb*sb - 4*sa*sc;
          if (d < -eps)
            break;
          if (d < 0)
            d = 0;
          d = sqrt_(d);

          rn1 = (-sb + d) / (2*sa);
          xn1 = a*rn1 + b;
          yn1 = e*rn1 + f;

          rn2 = (-sb - d) / (2*sa);
          xn2 = a*rn2 + b;
          yn2 = e*rn2 + f;
        }
      } else if (fabs(d) > eps) {
        D a = (-2*R - 2*r) / (2*d);
        D b = (d*d + R*R - r*r) / (2*d);

        D e = (-2*R - 2*rp) / (2*xp);
        D f = (xp*xp + R*R - rp*rp) / (2*xp);

        if (fabs(a - e) < eps)
          break;

        rn1 = (f - b) / (a - e);
        xn1 = a * rn1 + b;
        yn1 = sqrt_((R - rn1) * (R - rn1) - xn1 * xn1);

        rn2 = (f - b) / (a - e);
        xn2 = a * rn2 + b;
        yn2 = -sqrt_((R - rn2) * (R - rn2) - xn2 * xn2);
      } else {
        rn1 = (R - r) / 2;
        rn2 = rn1;
        Segment s;
        if (!Circle(Point(0, 0), r + rn1).intersect(Circle(Point(xp, yp), rp + rn1), s))
          break;
        xn1 = s.p.x;
        yn1 = s.p.y;
        xn2 = s.q.x;
        yn2 = s.q.y;
      }

      D an1 = atan2(yn1, xn1);
      D an2 = atan2(yn2, xn2);
      D a1 = (an1 - ap);
      if (a1 < 0)
        a1 += 2 * PI;

      D a2 = (an2 - ap);
      if (a2 < 0)
        a2 += 2 * PI;

      D xn = xn2;
      D yn = yn2;
      D rn = rn2;
      D an = an2;
      if (a1 < a2) {
        rn = rn1;
        xn = xn1;
        yn = yn1;
        an = an1;
      }

      D dn = sqrt_((xn - xs) * (xn - xs) + (yn - ys) * (yn - ys)) - rs - rn;
      if (dn < -eps)
        break;

      xp = xn;
      yp = yn;
      rp = rn;
      dp = dn;
      ap = an;
//      out(xp, yp, rp, dp);
    }
    return dp;
  };

  D min_rs = (R - d - r) / 2;
  D max_rs = (R + d - r) / 2;

  D result = R + r;
  int iters = 100000;
  D iter_step = (max_rs - min_rs) / iters;
  for (int iter = 0; iter <= iters; iter++) {
    D rs = min_rs + iter_step * iter;
    D cur = check(rs);
//    D step = iter_step / 2;
//    while (step > eps) {
//      D l = rs - step >= min_rs ? check(rs - step) : cur + 1;
//      D r = rs + step <= max_rs ? check(rs + step) : cur + 1;
//      if (cur <= l && cur <= r)
//        break;
//      if (l < r)
//        rs = rs - step;
//      else
//        rs = rs + step;
//      step = step / 2;
//    }
    result = min(result, cur);
  }

  out.setPrecision(10);
  out(result);
}

