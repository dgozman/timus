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
  template<typename T, typename...Ts> inline void print(const T &f, const Ts&... args) {
    *out << f;
    if (sizeof...(args) != 0) { *out << ' '; print(args...); }
  }
  template<typename...Ts> inline void println(const Ts&... args) { print(args...); *out << '\n'; }
  template<typename T> inline void printVec(const vec<T>& v) { for (int i = 0; i < v.size(); i++) { if (i) *out << ' '; print(v[i]); } }
  template<typename T> inline void printlnVec(const vec<T>& v) { printVec(v); *out << '\n'; }
  template<typename...Ts> inline void operator() (const Ts&... args) { println(args...); }
  template<typename T> inline void operator() (const vec<T>& v) { printlnVec(v); }
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

const double eps = 1e-8;

double sqr(double x) {
  return x * x;
}

void solve(Input& in, Output& out) {
  int n = in.ni();
  double R = in.ni();
  double R2 = sqr(R);
  vec<double> x(n), y(n), norm(n), norm2(n);
  for (int i = 0; i < n; i++) {
    x[i] = in.ni();
    y[i] = in.ni();
    norm2[i] = sqr(x[i]) + sqr(y[i]);
    norm[i] = sqrt(norm2[i]);
  }
  vec2d<double> d2 = newVec2d(n, n, 0.0);
  vec2d<double> dx = newVec2d(n, n, 0.0);
  vec2d<double> dy = newVec2d(n, n, 0.0);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      d2[i][j] = sqr(x[i] - x[j]) + sqr(y[i] - y[j]);
      if (d2[i][j] > 0) {
        double d = sqrt(d2[i][j]);
        dx[i][j] = (x[j] - x[i]) / d;
        dy[i][j] = (y[j] - y[i]) / d;
      }
    }
  }

  auto covered = [&] (double px, double py, double r2) {
    if (sqr(px) + sqr(py) > R2 + eps)
      return true;
    for (int k = 0; k < n; k++) {
      double dist = sqr(x[k] - px) + sqr(y[k] - py);
      if (dist < r2 - eps)
        return true;
    }
    return false;
  };

  auto covers = [&] (double r) {
    double r2 = r * r;

    for (int i = 0; i < n; i++) {
      for (int j = i + 1; j < n; j++) {
        if (d2[i][j] < eps)
          continue;
        // double a = x[j] - x[i];
        // double b = y[j] - y[i];
        double x0 = (x[i] + x[j]) / 2;
        double y0 = (y[i] + y[j]) / 2;
        // double dr = sqrt(r * r - d / 2 * d / 2);
        // x0 + b * dr / d;
        double dr = r2 - d2[i][j] / 4;
        if (dr < 0) {
          if (!covered(x0, y0, r2))
            return false;
        } else {
          dr = sqrt(dr);
          if (!covered(x0 + dy[i][j] * dr, y0 - dx[i][j] * dr, r2))
            return false;
          if (!covered(x0 - dy[i][j] * dr, y0 + dx[i][j] * dr, r2))
            return false;
        }
      }
    }

    for (int i = 0; i < n; i++) {
      if (norm2[i] < eps) {
        if (!covered(R, 0, r2))
          return false;
        continue;
      }
      // double a = 2 * x[i];
      // double b = 2 * y[i];
      // double c = r2 - R2 - x[i] * x[i] - y[i] * y[i];
      // double t = -c / (a * a + b * b);
      double t = -(r2 - R2 - norm2[i]) / (4 * norm2[i]);
      double x0 = 2 * x[i] * t;
      double y0 = 2 * y[i] * t;
      double dr = R2 - t * t * 4 * norm2[i];
      if (dr < 0) {
        if (!covered(x[i] * R / norm[i], y[i] * R / norm[i], r2))
          return false;
        if (!covered(-x[i] * R / norm[i], -y[i] * R / norm[i], r2))
          return false;
      } else {
        dr = sqrt(dr);
        if (!covered(x0 + y[i] * dr / norm[i], y0 - x[i] * dr / norm[i], r2))
          return false;
        if (!covered(x0 - y[i] * dr / norm[i], y0 + x[i] * dr / norm[i], r2))
          return false;
      }
    }

    return true;
  };

  double left = 0;
  double right = R * 3;
  for (int iter = 0; iter < 50; iter++) {
    double r = (left + right) / 2;
    if (covers(r))
      right = r;
    else
      left = r;
  }
  *out.out << std::setprecision(12) << std::fixed;
  out((left + right) / 2);
}
