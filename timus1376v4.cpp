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

template<class T> inline void hash_combine(size_t &seed, T const &v) {
  static std::hash<T> h;
  seed ^= h(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

template<int I, class... Ts> struct tuple_hash_helper {
  inline void operator()(size_t& seed, tuple<Ts...> const& t) {
    hash_combine(seed, get<I - 1>(t));
    tuple_hash_helper<I - 1, Ts...>()(seed, t);
  }
};

template<class... Ts> struct tuple_hash_helper<0, Ts...> {
  inline void operator()(size_t& seed, tuple<Ts...> const& t) {}
};

namespace std {
  template<class... Ts> struct hash<tuple<Ts...>> {
    std::size_t operator()(tuple<Ts...> const& t) const {
      size_t seed = 0;
      tuple_hash_helper<sizeof...(Ts), Ts...>()(seed, t);
      return seed;
    }
  };
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

const int max_half = 18;
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

  int large_len = min(maxn, max_half);
  vec<pair<D, Half>> large;
  large.reserve(1000000);
  int small_len = min(maxn, 16);
  vec<pair<D, Half>> small;
  small.reserve(1000000);
  std::array<u8, max_half> sides;

  D result = 1e100;
  int result_len = 0;
  vec<int> result_sequence(maxn + 1);
  vec<int> sequence(maxn + 1);

  int counter = 0;
  function<void(int, int, const Transform&)> go = [&] (int len, int prev, const Transform& transform) {
    ++counter;
    // if (!(counter % 1000000))
    //   out(counter);

    Point center = transform.apply(Point(0, 0));
    D d = (center - target).norm();
    Half half;
    half.sides = sides;
    half.len = len;
    half.transform = transform;
    large.push_back(make_pair(d, half));
    if (len <= small_len)
      small.push_back(make_pair(d, half));

    if (d < result - eps || (d < result + eps && len < result_len)) {
      // out(d);
      result_len = len;
      result = d;
      result_sequence = sequence;
    }

    for (int side = 1; side <= 4; side++) {
      for (int next = max(3, prev); len + next <= maxn; next++) {
        int rolls = next - 2;
        for (int dir_index = 0; dir_index < 2; dir_index++) {
          int dir = dir_index * 2 - 1;
          int s = side;
          sequence[len] = s;
          for (int i = 0; i < rolls; i++) {
            s = 1 + (s + 3 + dir) % 4;
            sequence[len + 1 + i] = s;
          }
          sequence[len + next - 1] = 0;
          go(len + next, next, transform * transforms[side][dir_index][rolls]);
        }
      }
    }
  };

  go(0, 0, Transform());

  out(counter);
  out("=======");

  out.setPrecision(3);
  out(result);
  out(result_len);
  out.print("0");
  for (int i = 0; i < result_len; i++) {
    out.print(" ");
    out.print(result_sequence[i]);
  }
  out("");
}
