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

double dist(int x1, int y1, int x2, int y2) {
  return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

int sign(int x) {
  return x > 0 ? 1 : x < 0 ? -1 : 0;
}

void solve(Input& in, Output& out) {
  out.setPrecision(6);
  int tests = in.ni();
  for (int test = 0; test < tests; test++) {
    int xs = in.ni();
    int ys = in.ni();
    int xt = in.ni();
    int yt = in.ni();
    int xa = in.ni();
    int ya = in.ni();
    int xb = in.ni();
    int yb = in.ni();
    int xc = in.ni();
    int yc = in.ni();

    auto different_sides = [&] (int px1, int py1, int px2, int py2, int qx1, int qy1, int qx2, int qy2) {
      int a = py1 - py2;
      int b = px2 - px1;
      int c = - (a * px1 + b * py1);
      int v1 = a * qx1 + b * qy1 + c;
      int v2 = a * qx2 + b * qy2 + c;
      return (v1 < 0 && v2 > 0) || (v1 > 0 && v2 < 0);
    };
    auto intersects = [&] (int px1, int py1, int px2, int py2, int qx1, int qy1, int qx2, int qy2) {
      return different_sides(px1, py1, px2, py2, qx1, qy1, qx2, qy2) &&
        different_sides(qx1, qy1, qx2, qy2, px1, py1, px2, py2);
    };
    auto belongs = [&] (int x1, int y1, int x2, int y2, int x, int y) {
      int a = y1 - y2;
      int b = x2 - x1;
      int c = - (a * x1 + b * y1);
      if (a * x + b * y + c != 0)
        return false;
      if (x == x1 && y == y1)
        return false;
      if (x == x2 && y == y2)
        return false;
      return x >= min(x1, x2) && x <= max(x1, x2) && y >= min(y1, y2) && y <= max(y1, y2);
    };
    auto check = [&] (int x1, int y1, int x2, int y2) {
      return !intersects(x1, y1, x2, y2, xa, ya, xb, yb) &&
        !intersects(x1, y1, x2, y2, xb, yb, xc, yc) &&
        !belongs(x1, y1, x2, y2, xb, yb);
    };

    auto is_outside = [&] (int x, int y) {
      int v = sign((xa - xb) * (yc - yb) - (ya - yb) * (xc - xb));
      int v1 = sign((xa - xb) * (y - yb) - (ya - yb) * (x - xb));
      int v2 = sign((x - xb) * (yc - yb) - (y - yb) * (xc - xb));
      return v != 0 && (v != v1 || v != v2);
    };
    bool both_outside = is_outside(xs, ys) && is_outside(xt, yt);

    double r = 1e100;
    if (check(xs, ys, xt, yt))
      r = min(r, dist(xs, ys, xt, yt));
    if (check(xs, ys, xa, ya) && check(xa, ya, xt, yt))
      r = min(r, dist(xs, ys, xa, ya) + dist(xa, ya, xt, yt));
    if (check(xs, ys, xc, yc) && check(xc, yc, xt, yt))
      r = min(r, dist(xs, ys, xc, yc) + dist(xc, yc, xt, yt));
    if (check(xs, ys, xb, yb) && check(xb, yb, xt, yt) && both_outside)
      r = min(r, dist(xs, ys, xb, yb) + dist(xb, yb, xt, yt));
    if (check(xs, ys, xa, ya) && check(xb, yb, xt, yt))
      r = min(r, dist(xs, ys, xa, ya) + + dist(xa, ya, xb, yb) + dist(xb, yb, xt, yt));
    if (check(xs, ys, xc, yc) && check(xb, yb, xt, yt))
      r = min(r, dist(xs, ys, xc, yc) + + dist(xc, yc, xb, yb) + dist(xb, yb, xt, yt));
    if (check(xs, ys, xb, yb) && check(xa, ya, xt, yt))
      r = min(r, dist(xs, ys, xb, yb) + + dist(xb, yb, xa, ya) + dist(xa, ya, xt, yt));
    if (check(xs, ys, xb, yb) && check(xc, yc, xt, yt))
      r = min(r, dist(xs, ys, xb, yb) + + dist(xb, yb, xc, yc) + dist(xc, yc, xt, yt));
    if (check(xs, ys, xa, ya) && check(xc, yc, xt, yt))
      r = min(r, dist(xs, ys, xa, ya) + + dist(xa, ya, xc, yc) + dist(xc, yc, xt, yt));
    if (check(xs, ys, xc, yc) && check(xa, ya, xt, yt))
      r = min(r, dist(xs, ys, xc, yc) + + dist(xc, yc, xa, ya) + dist(xa, ya, xt, yt));
    out(r);
  }
}
