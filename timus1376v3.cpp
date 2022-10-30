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
    out.print(x);
    out.print(" ");
    out.println(y);
  }
};

struct Transform {
  D a, b, c;
  D d, e, f;
  Transform(): a(1), b(0), c(0), d(0), e(1), f(0) {}
  Transform(D a, D b, D c, D d, D e, D f): a(a), b(b), c(c), d(d), e(e), f(f) {}

  Transform(const Point& p1, const Point& p2, const Point& q1, const Point& q2) {
    Point p = p2 - p1;
    Point q = q2 - q1;
    D pn = p.norm();
    D qn = q.norm();
    D qp = qn / pn;
    D cosa = (p.x * q.x + p.y * q.y) / (pn * qn);
    D sina = (p.x * q.y - p.y * q.x) / (pn * qn);
    // x; y
    // x - p1.x; y - p1.y
    // (x - p1.x) * cosa - (y - p1.y) * sina; (y - p1.y) * cosa + (x - p1.x) * sina
    // qp * (x - p1.x) * cosa - qp * (y - p1.y) * sina; qp * (y - p1.y) * cosa + qp * (x - p1.x) * sina
    // qp * (x - p1.x) * cosa - qp * (y - p1.y) * sina + q1.x; qp * (y - p1.y) * cosa + qp * (x - p1.x) * sina + q1.y
    a = qp * cosa;
    b = -qp * sina;
    c = -qp * p1.x * cosa + qp * p1.y * sina + q1.x;
    d = qp * sina;
    e = qp * cosa;
    f = -qp * p1.y * cosa - qp * p1.x * sina + q1.y;
  }

  Transform operator*(const Transform& other) const {
    return Transform(
      other.a * a + other.b * b, other.a * b + other.b * e, other.a * c + other.b * f + other.c,
      other.d * a + other.e * b, other.d * b + other.e * e, other.d * c + other.e * f + other.f
    );
  }

  Point apply(const Point& p) const {
    return Point(a * p.x + b * p.y + c, d * p.x + e * p.y + f);
  }
};

const int max_half = 20;
struct Half {
  Transform transform;
  std::array<u8, max_half> sides;
  int len;
  bool operator<(const Half& other) const {
    return len < other.len;
  }
};

const D eps = 1e-4;

void solve(Input& in, Output& out) {
  D a1 = in.next<D>();
  D a2 = in.next<D>();
  D ha1 = sqrt(sqr(a2) - sqr(a1 / 2)) / a1;
  Point target(in.next<D>(), in.next<D>());
  int maxn = in.ni();

  auto mul = [&] (const Transform& t1, const Transform& t2) {
    Point p1 = t1.apply(Point(-a1 / 2, a1 / 2));
    Point p2 = t1.apply(Point(a1 / 2, a1 / 2));
  };

  vec3d<Transform> transforms = newVec3d(5, 2, maxn - 1, Transform());

  auto gen_transforms = [&] () {
    vec<Point> points({ Point(-a1 / 2, a1 / 2), Point(a1 / 2, a1 / 2), Point(a1 / 2, -a1 / 2), Point(-a1 / 2, -a1 / 2) });
    D cosa = (2 * a2 * a2 - a1 * a1) / (2 * a2 * a2);
    D sina = sqrt(1.0 - sqr(cosa));
    for (int side = 1; side <= 4; side++) {
      Point& pi = points[side - 1];
      Point& pj = points[side % 4];
      Point center = (pi + pj) / 2 + (pj - points[(side + 1) % 4]) * ha1;
      for (int dir_index = 0; dir_index < 2; dir_index++) {
        Point q1 = pi - center;
        Point q2 = pj - center;
        int dir = dir_index * 2 - 1;
        for (int rolls = 1; rolls <= maxn - 2; rolls++) {
          q1 = q1.rotate(cosa, sina * dir);
          q2 = q2.rotate(cosa, sina * dir);
          transforms[side][dir_index][rolls] = Transform(pi, pj, q1 + center, q2 + center);
        }
      }
    }
  };
  gen_transforms();

  int large_len = min(maxn / 2 + 3, max_half);
  unordered_set<u32> visited;
  visited.reserve(1000000);
  vec<pair<D, Half>> large;
  large.reserve(1000000);
  int small_len = min(maxn / 2, 16);
  vec<pair<D, Half>> small;
  small.reserve(1000000);
  std::array<u8, max_half> sides;

  function<void(int, const Transform&)> go = [&] (int len, const Transform& transform) {
    Point center = transform.apply(Point(0, 0));
    Point second = transform.apply(Point(1, 1));
    D d = center.norm();

    u32 seed = len;
    seed ^= (int) round(center.x * 1000) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    seed ^= (int) round(center.y * 1000) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    seed ^= (int) round(second.x * 1000) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    seed ^= (int) round(second.y * 1000) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    // if (visited.find(seed) == visited.end()) {
    //   visited.insert(seed);
      Half half;
      half.sides = sides;
      half.len = len;
      half.transform = transform;
      large.push_back(make_pair(d, half));
      if (len <= small_len)
        small.push_back(make_pair(d, half));
    // }

    for (int side = 1; side <= 4; side++) {
      for (int next = 3; len + next <= large_len; next++) {
        int rolls = next - 2;
        for (int dir_index = 0; dir_index < 2; dir_index++) {
          int dir = dir_index * 2 - 1;
          int s = side;
          sides[len] = s;
          for (int i = 0; i < rolls; i++) {
            s = 1 + (s + 3 + dir) % 4;
            sides[len + 1 + i] = s;
          }
          sides[len + next - 1] = 0;
          go(len + next, transform * transforms[side][dir_index][rolls]);
        }
      }
    }
  };

  go(0, Transform());

  out("========");
  out(large_len);
  out(large.size());
  out(small_len);
  out(small.size());
  sort(large.begin(), large.end());
  out("========");
  transforms[4][0][2].apply(Point(0, 0)).out(out);
  transforms[2][1][2].apply(Point(0, 0)).out(out);
  (transforms[4][0][2] * transforms[2][1][2]).apply(Point(0, 0)).out(out);
  out("========");

  pair<D, Half> needle;
  needle.second.len = 0;
  D result_dist = 1e100;
  int result_len = 0;
  std::array<u8, 32> result_sides;
  int iterations = 0;

  auto relax = [&] (const Half& first, const Half& second, D dist) {
    // out.println(dist, first.len, second.len);
    // vec<int> v(first.len);
    // std::copy(first.sides.begin(), first.sides.begin() + first.len, v.begin());
    // out(v);
    // first.transform.apply(Point(0, 0)).out(out);

    // vec<int> w(second.len);
    // std::copy(second.sides.begin(), second.sides.begin() + second.len, w.begin());
    // out(w);
    // second.transform.apply(Point(0, 0)).out(out);

    std::copy(first.sides.begin(), first.sides.begin() + first.len, result_sides.begin());
    std::copy(second.sides.begin(), second.sides.begin() + second.len, result_sides.begin() + first.len);
    result_dist = dist;
    result_len = first.len + second.len;
  };

  for (auto& pair : small) {
    Half& half = pair.second;
    Point center = half.transform.apply(Point(0, 0));
    D target_dist = (center - target).norm();
    needle.first = target_dist - result_dist - eps;
    auto it = std::lower_bound(large.begin(), large.end(), needle);
    while (it != large.end()) {
      ++iterations;
      if (it->first > target_dist + result_dist + eps)
        break;
      Half& other = it->second;
      int len = half.len + other.len;
      if (len <= maxn) {
        Point end = (half.transform * other.transform).apply(Point(0, 0));
        D dist = (end - target).norm();
        if (dist < result_dist - eps || (dist < result_dist + eps && len < result_len))
          relax(half, other, dist);
        end = (other.transform * half.transform).apply(Point(0, 0));
        dist = (end - target).norm();
        if (dist < result_dist - eps || (dist < result_dist + eps && len < result_len))
          relax(other, half, dist);
      }
      ++it;
    }
  }

  out(iterations);
  out("======");

  out.setPrecision(3);
  out(result_dist);
  out(result_len);
  out.print("0");
  for (int i = 0; i < result_len; i++) {
    out.print(" ");
    out.print((int) result_sides[i]);
  }
  out("");
}
