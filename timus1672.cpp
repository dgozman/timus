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

using D = double;
const D eps = 1e-10;
const D PI = 3.1415926535897932384626433832795028841971;
const D phi1 = (3 - sqrtf((D) 5)) / 2;
const D phi2 = (sqrtf((D) 5) - 1) / 2;

void solve(Input& in, Output& out) {
  out.setPrecision(18);
  int n = in.ni();
  vec<int> x(n), y(n), z(n);
  for (int i = 0; i < n; i++) {
    x[i] = in.ni();
    y[i] = in.ni();
    z[i] = in.ni();
  }

  auto rotate = [&] (D x, D y, D z, D cosa, D sina, D cosb, D sinb, D cosc, D sinc, D& rx, D& ry, D& rz) {
    rx = x * cosa - y * sina;
    ry = x * sina + y * cosa;
    rz = z;
    x = rx * sinb + rz * cosb;
    y = ry;
    z = rx * cosb - rz * sinb;
    rx = x;
    ry = y * cosc - z * sinc;
    rz = y * sinc + z * cosc;
  };

  D ax, ay, az;
  D bx, by, bz;
  D cx, cy, cz;
  auto find_vectors = [&] (D a, D b, D c) {
    D cosa = cos(a);
    D sina = sin(a);
    D cosb = cos(b);
    D sinb = sin(b);
    D cosc = cos(c);
    D sinc = sin(c);
    rotate(1, 0, 0, cosa, sina, cosb, sinb, cosc, sinc, ax, ay, az);
    rotate(0, 1, 0, cosa, sina, cosb, sinb, cosc, sinc, bx, by, bz);
    rotate(0, 0, 1, cosa, sina, cosb, sinb, cosc, sinc, cx, cy, cz);
  };

  auto find_side = [&] (D vx, D vy, D vz) {
    D vmin = 1e100;
    D vmax = -1e100;
    for (int i = 0; i < n; i++) {
      D v = vx * x[i] + vy * y[i] + vz * z[i];
      vmin = fmin(vmin, v);
      vmax = fmax(vmax, v);
    }
    return (vmax - vmin) / sqrtf(vx * vx + vy * vy + vz * vz);
  };

  int iter = 0;
  auto fit = [&] (D a, D b, D c) {
    ++iter;
    find_vectors(a, b, c);
    D da = find_side(ax, ay, az);
    D db = find_side(bx, by, bz);
    D dc = find_side(cx, cy, cz);
    D dmin = fmin(da, fmin(db, dc));
    D dmax = fmax(da, fmax(db, dc));
    return dmax - dmin;
  };

  // Finds minimum.
  auto ternary = [&] (D left, D right, function<D(D)> calc) {
    D a, va, b, vb;
    bool hasa = false, hasb = false;
    while (right - left > eps) {
      if (!hasa) {
        a = left + (right - left) * phi1;
        va = calc(a);
        hasa = true;
      }
      if (!hasb) {
        b = left + (right - left) * phi2;
        vb = calc(b);
        hasb = true;
      }
      if (va < vb) {
        right = b;
        b = a;
        vb = va;
        hasa = false;
      } else {
        left = a;
        a = b;
        va = vb;
        hasb = false;
      }
    }
    D mid = (left + right) / 2;
    return make_pair(mid, calc(mid));
  };
  // auto ternary = [&] (D left, D right, function<D(D)> calc) {
  //   while (right - left > eps) {
  //     D a = (2 * left + right) / 3;
  //     D b = (left + 2 * right) / 3;
  //     if (calc(a) < calc(b))
  //       right = b;
  //     else
  //       left = a;
  //   }
  //   D mid = (left + right) / 2;
  //   return make_pair(mid, calc(mid));
  // };

  const int dnum = 30;
  const D dang = PI / dnum;

  auto search_ang = [&] (function<D(D)> calc) {
    D best = 0;
    D best_value = 1e100;
    for (int i = 0; i <= dnum; i++) {
      D ang = dang * i;
      D value = calc(ang);
      if (value < best_value) {
        best_value = value;
        best = ang;
      }
    }
    return ternary(best - dang, best + dang, calc);
  };

  D temp_a, temp_b, temp_c;
  auto calc_c = [&] (D c) {
    temp_c = c;
    return fit(temp_a, temp_b, temp_c);
  };
  auto calc_b = [&] (D b) {
    temp_b = b;
    return search_ang(calc_c).second;
  };
  auto calc_a = [&] (D a) {
    temp_a = a;
    return search_ang(calc_b).second;
  };

  D best_a = search_ang(calc_a).first;
  temp_a = best_a;
  D best_b = search_ang(calc_b).first;
  temp_b = best_b;
  D best_c = search_ang(calc_c).first;

  find_vectors(best_a, best_b, best_c);
  out(ax, ay, az);
  out(bx, by, bz);
  out(cx, cy, cz);

  // out("===");
  // out(fit(best_a, best_b, best_c));
  // out(ax, ay, az);
  // out(bx, by, bz);
  // out(cx, cy, cz);

  // out("===");
  // out(fit(0, 0, 0));
  // out(ax, ay, az);
  // out(bx, by, bz);
  // out(cx, cy, cz);

  // out("===");
  // out(fit(0, 1, 2));
  // out(ax, ay, az);
  // out(bx, by, bz);
  // out(cx, cy, cz);

  // out(iter);
}
