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


using D = double;
const D eps = 1e-8;

D sqr(D x) {
  return x * x;
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
  D norm() const {
    return sqrt(sqr(x) + sqr(y));
  }
  void out(Output& out) const {
    out.println(x, y);
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
  D norm() const {
    return sqrt(norm2());
  }
  D norm2() const {
    return sqr(a) + sqr(b);
  }
  Line shift(D d, const Point& towards) const {
    return apply(towards) < 0 ? Line(a, b, c + d * norm()) : Line(a, b, c - d * norm());
  }
};

struct Circle {
  Point c;
  D r;
  Circle(): r(0) {}
  Circle(const Point& c, D r): c(c), r(r) {}
  void out(Output& out) const {
    out.println(c.x, c.y, r);
  }
  vec<Point> intersect(const Line& line) const {
    vec<Point> result;
    D norm = line.norm();
    D t = -line.apply(c) / (norm * norm);
    D r2 = r * r - t * t * norm * norm;
    if (r2 < -eps)
      return result;
    if (r2 < 0)
      r2 = 0;
    r2 = sqrt(r2) / norm;
    Point p(c.x + line.a * t, c.y + line.b * t);
    result.push_back(Point(p.x + line.b * r2, p.y - line.a * r2));
    result.push_back(Point(p.x - line.b * r2, p.y + line.a * r2));
    return result;
  }
};

void solve(Input& in, Output& out) {
  D width = in.next<D>();
  D height = in.next<D>();
  vec<D> r = in.nvec<D>(3);
  vec<Point> corners({
    Point(0, 0),
    Point(width, 0),
    Point(width, height),
    Point(0, height)
  });
  vec<Line> lines({
    Line(corners[0], corners[1]),
    Line(corners[1], corners[2]),
    Line(corners[2], corners[3]),
    Line(corners[3], corners[0])
  });

  vec<Point> result;
  bool result_found = false;
  auto check = [&] (const vec<Point>& ps) {
    for (int i = 0; i < 3; i++) {
      if (ps[i].x - r[i] < -eps || ps[i].x + r[i] > width + eps)
        return;
      if (ps[i].y - r[i] < -eps || ps[i].y + r[i] > height + eps)
        return;
    }
    for (int i = 0; i < 3; i++) {
      for (int j = i + 1; j < 3; j++) {
        D d = (ps[i] - ps[j]).norm();
        if (d < r[i] + r[j] - eps)
          return;
      }
    }
    result_found = true;
    result = ps;
  };

  vec<Point> ps(3);
  for (int i = 0; i < 3; i++) {
    ps[i] = Point(r[i], r[i]);
    for (int j = 0; j < 3; j++) {
      if (j == i)
        continue;
      for (int lj = 0; lj < 4; lj++) {
        Line line_j = lines[lj].shift(r[j], Point(width / 2, height / 2));
        auto pjs = Circle(ps[i], r[i] + r[j]).intersect(line_j);
        for (auto& pj : pjs) {
          ps[j] = pj;
          int k = 3 - i - j;
          for (int lk = 0; lk < 4; lk++) {
            Line line_k = lines[lk].shift(r[k], Point(width / 2, height / 2));
            auto pks1 = Circle(ps[i], r[i] + r[k]).intersect(line_k);
            for (auto& pk : pks1) {
              ps[k] = pk;
              check(ps);
            }
            auto pks2 = Circle(ps[j], r[j] + r[k]).intersect(line_k);
            for (auto& pk : pks2) {
              ps[k] = pk;
              check(ps);
            }
          }
        }
      }
    }
  }

  if (result_found) {
    out.setPrecision(10);
    for (int i = 0; i < 3; i++) {
      out.print(result[i].x);
      out.print(" ");
      out.print(result[i].y);
      out.print(" ");
    }
    out.println("");
  } else {
    out(0);
  }
}
