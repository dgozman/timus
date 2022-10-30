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
const D eps = 1e-14;

struct Point {
  D x, y, z;
  Point(): x(0), y(0), z(0) {}
  Point(D x, D y, D z): x(x), y(y), z(z) {}

  D norm2() {
    return x * x + y * y + z * z;
  }

  D norm() {
    return sqrt(norm2());
  }

  Point operator/(const D d) const {
    return Point(x / d, y / d, z / d);
  }

  Point operator-(const Point& p) const {
    return Point(x - p.x, y - p.y, z - p.z);
  }

  D dot(const Point& p) const {
    return x * p.x + y * p.y + z * p.z;
  }

  Point cross(const Point& p) const {
    return Point(y * p.z - z * p.y, z * p.x - x * p.z, x * p.y - y * p.x);
  }
};

struct Matrix {
  Point a, b, c;
  Matrix() {}
  Matrix(Point a, Point b, Point c): a(a), b(b), c(c) {}

  Point apply(const Point& p) const {
    return Point(a.dot(p), b.dot(p), c.dot(p));
  }

  static Matrix rotation(Point u, D cosa, D sina) {
    // https://en.wikipedia.org/wiki/Rotation_matrix#Rotation_matrix_from_axis_and_angle
    return Matrix(
      Point(
        cosa + u.x * u.x * (1 - cosa),
        u.x * u.y * (1 - cosa) - u.z * sina,
        u.x * u.z * (1 - cosa) + u.y * sina
      ),
      Point(
        u.y * u.x * (1 - cosa) + u.z * sina,
        cosa + u.y * u.y * (1 - cosa),
        u.y * u.z * (1 - cosa) - u.x * sina
      ),
      Point(
        u.z * u.x * (1 - cosa) - u.y * sina,
        u.z * u.y * (1 - cosa) + u.x * sina,
        cosa + u.z * u.z * (1 - cosa)
      )
    );
  }
};

D clamp(D x, D mn, D mx) {
  return max(mn, min(x, mx));
}

void solve(Input& in, Output& out) {
  // https://polyhedra.tessera.li/
  string s = in.ns();
  vec<Point> p;
  if (s == "tetrahedron") {
    D s3 = sqrt((D) 3);
    D s2 = sqrt((D) 2);
    D s23 = sqrt((D) 2.0 / 3);
    p = {
      Point(0, 0, s3),
      Point(sqrt(3 - (D) 1.0 / 3), 0, -1 / s3),
      Point(-s23, s2, -1 / s3),
      Point(-s23, -s2, -1 / s3)
    };
    D dist = (p[0] - p[1]).norm();
    for (auto edge : vec2d<int>({{0,1},{0,2},{0,3},{1,2},{1,3},{2,3}})) {
      D dist2 = (p[edge[0]] - p[edge[1]]).norm();
      if (abs(dist - dist2) > eps)
        panic();
    }
  } else if (s == "hexahedron") {
    D s32 = sqrt((D) 3.0 / 2);
    D s3 = sqrt((D) 3);
    D s13 = sqrt((D) 4.0 / 3);
    D s16 = sqrt((D) 1.0 / 6);
    p = {
      Point(0, 0, s32),
      Point(s13, 0, s16),
      Point(-1 / s3, 1, s16),
      Point(-1 / s3, -1, s16),
      Point(1 / s3, 1, -s16),
      Point(1 / s3, -1, -s16),
      Point(-s13, 0, -s16),
      Point(0, 0, -s32)
    };
    D dist = (p[0] - p[1]).norm();
    for (auto edge : vec2d<int>({{0,1},{0,2},{0,3},{1,4},{1,5},{2,4},{2,6},{3,5},{3,6},{4,7},{5,7},{6,7}})) {
      D dist2 = (p[edge[0]] - p[edge[1]]).norm();
      if (abs(dist - dist2) > eps)
        panic();
    }
  } else if (s == "octahedron") {
    D s2 = sqrt((D) 2.0);
    p = {
      Point(0, 0, s2),
      Point(s2, 0, 0),
      Point(0, s2, 0),
      Point(-s2, 0, 0),
      Point(0, -s2, 0),
      Point(0, 0, -s2)
    };
    D dist = (p[0] - p[1]).norm();
    for (auto edge : vec2d<int>({{0,1},{0,2},{0,3},{0,4},{1,2},{1,4},{1,5},{2,3},{2,5},{3,4},{3,5},{4,5}})) {
      D dist2 = (p[edge[0]] - p[edge[1]]).norm();
      if (abs(dist - dist2) > eps)
        panic();
    }
  } else if (s == "dodecahedron") {
    D s25 = (D) 2.0 / (1 + sqrt((D) 5));
    D s23 = (D) 2.0 / (3 + sqrt((D) 5));
    D s043335 = 4 * sqrt((D) 3) / (3 + 3 * sqrt((D) 5));
    D s023335 = 2 * sqrt((D) 3) / (3 + 3 * sqrt((D) 5));
    D s115023 = (1 + sqrt((D) 5)) / (2 * sqrt((D) 3));
    D s013635 = (sqrt((D) 3)) / (6 + 3 * sqrt((D) 5));
    D norm = s013635 * s013635 + 1 * 1 + s023335 * s023335;
    D f1 = sqrt(norm);
    D f2 = sqrt(norm - s043335 * s043335);
    p = {
      Point(0, 0, f1),
      Point(s043335, 0, f2),
      Point(-s023335, s25, f2),
      Point(-s023335, -s25, f2),
      Point(f2, s25, s023335),
      Point(f2, -s25, s023335),
      Point(-s115023, s23, s023335),
      Point(s013635, 1, s023335),
      Point(s013635, -1, s023335),
      Point(-s115023, -s23, s023335),
      Point(s115023, s23, -s023335),
      Point(s115023, -s23, -s023335),
      Point(-f2, s25, -s023335),
      Point(-s013635, 1, -s023335),
      Point(-s013635, -1, -s023335),
      Point(-f2, -s25, -s023335),
      Point(s023335, s25, -f2),
      Point(s023335, -s25, -f2),
      Point(-s043335, 0, -f2),
      Point(0, 0, -f1)
    };
    D dist = (p[0] - p[1]).norm();
    for (auto edge : vec2d<int>({{0,1},{0,2},{0,3},{1,4},{1,5},{2,6},{2,7},{3,8},{3,9},{4,7},{4,10},{5,8},{5,11},{6,9},{6,12},{7,13},{8,14},{9,15},{10,11},{10,16},{11,17},{12,13},{12,18},{13,16},{14,15},{14,17},{15,18},{16,19},{17,19},{18,19}})) {
      D dist2 = (p[edge[0]] - p[edge[1]]).norm();
      if (abs(dist - dist2) > eps)
        panic();
    }
  } else if (s == "icosahedron") {
    D s25 = (D) 2.0 / (1 + sqrt((D) 5));
    D s22510 = (D) 2.0 / sqrt((D) 2 * sqrt((D) 5) + 10);
    D s28520 = (D) 2.0 / sqrt((D) 8 * sqrt((D) 5) + 20);
    D s2m2510 = (D) 2.0 / sqrt((D) -2 * sqrt((D) 5) + 10);
    D norm = s28520 * s28520 + 1 * 1 + s22510 * s22510;
    D f1 = sqrt(norm);
    D f2 = sqrt(norm - s22510 * s22510);
    p = {
      Point(0, 0, f1),
      Point(f2, 0, s22510),
      Point(s28520, 1, s22510),
      Point(-s2m2510, s25, s22510),
      Point(-s2m2510, -s25, s22510),
      Point(s28520, -1, s22510),
      Point(s2m2510, s25, -s22510),
      Point(s2m2510, -s25, -s22510),
      Point(-s28520, 1, -s22510),
      Point(-f2, 0, -s22510),
      Point(-s28520, -1, -s22510),
      Point(0, 0, -f1)
    };
    D dist = (p[0] - p[1]).norm();
    for (auto edge : vec2d<int>({{0,1},{0,2},{0,3},{0,4},{0,5},{1,2},{1,5},{1,6},{1,7},{2,3},{2,6},{2,8},{3,4},{3,8},{3,9},{4,5},{4,9},{4,10},{5,7},{5,10},{6,7},{6,8},{6,11},{7,10},{7,11},{8,9},{8,11},{9,10},{9,11},{10,11}})) {
      D dist2 = (p[edge[0]] - p[edge[1]]).norm();
      if (abs(dist - dist2) > eps)
        panic();
    }
  } else {
    panic();
  }

  int m = p.size();
  D pnorm = p[0].norm();
  for (int i = 0; i < m; i++) {
    if (abs(p[i].norm() - pnorm) > eps)
      panic();
    p[i] = p[i] / pnorm;
  }

  int n = in.ni();
  D result = 0;
  for (int i = 0; i < n; i++) {
    Point r;
    r.x = -in.next<D>();
    r.y = -in.next<D>();
    r.z = -in.next<D>();
    r = r / r.norm();

    int best = -1;
    D bestcos = -2;
    for (int j = 0; j < m; j++) {
      D cosa = r.dot(p[j]);
      if (cosa > bestcos) {
        bestcos = cosa;
        best = j;
      }
    }
    for (int j = 0; j < m; j++) {
      D cosa = r.dot(p[j]);
      if (j != best && abs(cosa - bestcos) < eps) {
        // out("hey");
        panic();
      }
    }

    D bestang = acos(clamp(bestcos, -1, 1));
    // out(bestang);
    result += bestang;
    if (bestcos < 1 - eps) {
      // https://math.stackexchange.com/questions/2754627/rotation-matrices-between-two-points-on-a-sphere
      Point cross = p[best].cross(r);
      D cosa = p[best].dot(r);
      D sina = cross.norm();
      cross = cross / cross.norm();

      Matrix R = Matrix::rotation(cross, cosa, sina);
      for (int j = 0; j < m; j++) {
        p[j] = R.apply(p[j]);
        p[j] = p[j] / p[j].norm(); // This is very important to not accumulate the error.
      }
      D dist = (p[best] - r).norm();
      if (dist > eps) {
        // out("hey2", dist);
        panic();
      }
    }
  }
  out.setPrecision(5);
  out(result);
}
