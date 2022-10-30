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

i64 gcd(i64 a, i64 b) {
  a = abs(a);
  b = abs(b);
  while (b > 0) {
    i64 c = a % b;
    a = b;
    b = c;
  }
  return a;
}

struct Rat {
  i64 p, q;
  Rat(): p(0), q(1) {}
  Rat(i64 x): p(x), q(1) {}
  Rat(i64 np, i64 nq): p(np), q(nq) {
    if (q == 0)
      panic();
    if (q < 0) {
      p = -p;
      q = -q;
    }
    i64 g = gcd(p, q);
    p /= g;
    q /= g;
  }

  Rat operator-() const {
    return Rat(-p, q);
  }
  Rat operator+(const Rat& other) const {
    return Rat(p * other.q + q * other.p, q * other.q);
  }
  Rat operator-(const Rat& other) const {
    return Rat(p * other.q - q * other.p, q * other.q);
  }
  Rat operator*(i64 d) const {
    return Rat(p * d, q);
  }
  Rat operator/(i64 d) const {
    return Rat(p, q * d);
  }
  Rat operator*(const Rat& other) const {
    return Rat(p * other.p, q * other.q);
  }
  Rat operator/(const Rat& other) const {
    return Rat(p * other.q, q * other.p);
  }
  bool operator<(const Rat& other) const {
    return p * other.q < q * other.p;
  }
  bool operator>(const Rat& other) const {
    return p * other.q > q * other.p;
  }
  bool operator==(const Rat& other) const {
    return p == other.p && q == other.q;
  }
  bool operator<=(const Rat& other) const {
    return p * other.q <= q * other.p;
  }
  bool operator>=(const Rat& other) const {
    return p * other.q >= q * other.p;
  }
};

struct Point {
  Rat x, y;
  Point() {}
  Point(Rat x, Rat y): x(x), y(y) {}
  Point operator-(const Point& other) const {
    return Point(x - other.x, y - other.y);
  }
  Point operator+(const Point& other) const {
    return Point(x + other.x, y + other.y);
  }
  Point operator*(const i64& d) const {
    return Point(x * d, y * d);
  }
  Point operator/(const i64& d) const {
    return Point(x / d, y / d);
  }
  Point operator*(const Rat& d) const {
    return Point(x * d, y * d);
  }
  Point operator/(const Rat& d) const {
    return Point(x / d, y / d);
  }
  bool operator<(const Point& p) const {
    return x < p.x || (x == p.x && y < p.y);
  }
  bool operator>(const Point& p) const {
    return x > p.x || (x == p.x && y > p.y);
  }
  bool operator==(const Point& p) const {
    return x == p.x && y == p.y;
  }
  Rat dot(const Point& p) const {
    return x * p.y - y * p.x;
  }
  Rat cross(const Point& p) const {
    return x * p.x + y * p.y;
  }
};

struct Range {
  Rat from, to;
  Range() {}
  Range(const Rat& l, const Rat& r): from(l < r ? l : r), to(l < r ? r : l) {}
  bool contains_inclusive(const Rat& x) const {
    return from <= x && x <= to;
  }
  bool contains_exclusive(const Rat& x) const {
    return from < x && x < to;
  }
};

struct Line {
  Rat a, b, c;
  Line(): a(Rat(1)), b(Rat(1)), c(0) {}
  Line(const Rat& a, const Rat& b, const Rat& c): a(a), b(b), c(c) {}
  Line(const Rat& x1, const Rat& y1, const Rat& x2, const Rat& y2): Line(Point(x1, y1), Point(x2, y2)) {}
  Line(const Point& p1, const Point& p2) {
    a = p1.y - p2.y;
    b = p2.x - p1.x;
    c = -a * p1.x - b * p1.y;
  }
  Rat apply(const Point& p) const {
    return a * p.x + b * p.y + c;
  }
  bool contains(const Point& p) const {
    return apply(p) == 0;
  }
  bool same_side_inclusive(const Point& p, const Point& q) const {
    Rat pv = apply(p);
    Rat qv = apply(q);
    Rat zero(0);
    return (pv >= zero && qv >= zero) || (pv <= zero && qv <= zero);
  }
  bool same_side_exclusive(const Point& p, const Point& q) const {
    Rat pv = apply(p);
    Rat qv = apply(q);
    Rat zero(0);
    return (pv > zero && qv > zero) || (pv < zero && qv < zero);
  }
  bool intersect(const Line& line, Point& result) const {
    Rat d = a * line.b - b * line.a;
    if (d == 0)
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
  Segment(const Rat& x1, const Rat& y1, const Rat& x2, const Rat& y2): p(x1, y1), q(x2, y2) {}
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

struct Ray {
  Point p, q;
  Ray() {}
  Ray(const Point& p, const Point& q): p(p), q(q) {}
  Ray(const Rat& x1, const Rat& y1, const Rat& x2, const Rat& y2): p(x1, y1), q(x2, y2) {}
  Line line() const {
    return Line(p, q);
  }
  bool contains_inclusive(const Point& r) const {
    return line().contains(r) && (q - p).cross(r - p) >= Rat(0);
  }
  bool contains_exclusive(const Point& r) const {
    return line().contains(r) && (q - p).cross(r - p) > Rat(0);
  }
  bool intersect_inclusive(const Line& l, Point& result) const {
    if (!line().intersect(l, result))
      return false;
    return contains_inclusive(result);
  }
};

const i64 inf2 = 1000000000;

void solve(Input& in, Output& out) {
  Point ps(Rat(in.ni()), Rat(in.ni()));
  Rat sz(in.ni());
  Point pa(Rat(in.ni()), Rat(in.ni()));
  Point pb(Rat(in.ni()), Rat(in.ni()));
  Point original_pb = pb;
  if (pa == pb)
    pb = Point(pa.x + Rat(1), pa.y);

  function<bool(const Point&, const Point&)> more = [&] (const Point& p1, const Point& p2) {
    return p1 > p2;
  };
  if (pa > pb) {
    more = [&] (const Point& p1, const Point& p2) {
      return p1 < p2;
    };
  }

  auto check_side = [&] (const Point& p1, const Point& p2, const Rat& h, Segment& s, bool& has) {
    if (Line(p1, p2).apply(ps) == 0)
      return;
    Line line(pa, pb);

    auto check_point = [&] (const Point& r) {
      if (!has) {
        has = true;
        s.p = s.q = r;
        return;
      }
      if (more(s.p, r))
        s.p = r;
      if (more(r, s.q))
        s.q = r;
    };
    Point r;

    if (sz > h) {
      Point v1 = ps + (p1 - ps) * sz / (sz - h);
      Point v2 = ps + (p2 - ps) * sz / (sz - h);
      if (Segment(p1, v1).intersect_inclusive(line, r))
        check_point(r);
      if (Segment(p2, v2).intersect_inclusive(line, r))
        check_point(r);
      if (Segment(p1, p2).intersect_inclusive(line, r))
        check_point(r);
      if (Segment(v1, v2).intersect_inclusive(line, r))
        check_point(r);
    } else {
      Point v1 = p1 + (p1 - ps);
      Point v2 = p2 + (p2 - ps);
      if (Ray(p1, v1).intersect_inclusive(line, r))
        check_point(r);
      if (Ray(p2, v2).intersect_inclusive(line, r))
        check_point(r);
      if (Segment(p1, p2).intersect_inclusive(line, r))
        check_point(r);
      if (Line(p1, v1).same_side_inclusive(p2, pa) &&
          Line(p2, v2).same_side_inclusive(p1, pa) &&
          Line(p1, p2).same_side_inclusive(v1, pa))
        check_point(pa + (pa - pb));
      if (Line(p1, v1).same_side_inclusive(p2, pb) &&
          Line(p2, v2).same_side_inclusive(p1, pb) &&
          Line(p1, p2).same_side_inclusive(v1, pb))
        check_point(pb + (pb - pa));
    }
  };

  auto check = [&] (const Rat& lx, const Rat& ly, const Rat& rx, const Rat& ry, const Rat& h, Segment& res) {
    bool has = false;
    check_side(Point(lx, ly), Point(rx, ly), h, res, has);
    check_side(Point(rx, ly), Point(rx, ry), h, res, has);
    check_side(Point(rx, ry), Point(lx, ry), h, res, has);
    check_side(Point(lx, ry), Point(lx, ly), h, res, has);
    return has;
  };

  int n = in.ni();
  const int kEnd = 0;
  const int kStart = 1;
  vec<pair<Point, int>> points;
  for (int i = 0; i < n; i++) {
    int lx = in.ni();
    int ly = in.ni();
    int rx = in.ni();
    int ry = in.ni();
    int h = in.ni();

    Segment s;
    if (!check(Rat(lx), Rat(ly), Rat(rx), Rat(ry), Rat(h), s))
      continue;
    points.push_back({ s.p, kStart });
    points.push_back({ s.q, kEnd });
  }

  if (pa == original_pb) {
    points.push_back({ pa + (pa - pb) * inf2, kStart });
    points.push_back({ pa, kEnd });
    points.push_back({ pa, kStart });
    points.push_back({ pa + (pb - pa) * inf2, kEnd });
  } else {
    points.push_back({ pa + (pa - pb) * inf2, kStart });
    points.push_back({ pa, kEnd });
    points.push_back({ pb, kStart });
    points.push_back({ pb + (pb - pa) * inf2, kEnd });
  }

  sort(points.begin(), points.end(), [&](const pair<Point, int>& a, const pair<Point, int>& b) -> bool {
    if (more(b.first, a.first))
      return true;
    if (a.first == b.first && a.second < b.second)
      return true;
    return false;
  });

  int overlap = 0;
  for (int i = 0; i < points.size() - 1; i++) {
    if (points[i].second == kStart)
      overlap++;
    else
      overlap--;
    if (overlap <= 0) {
      out.setPrecision(12);
      Point p = points[i].first;
      long double x = p.x.p;
      x /= p.x.q;
      long double y = p.y.p;
      y /= p.y.q;
      out(x, y);
      return;
    }
  }
  out("Impossible");
}
