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

using ld = long double;
const ld eps = 1e-10;

ld dist2(ld x1, ld y1, ld x2, ld y2) {
  return (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
}

bool intersect(Output& out, ld x1, ld y1, ld x2, ld y2, ld tmin, ld xc, ld yc, ld rc2, ld& t) {
  // x1 + t * dx, y1 + t * dy
  // dx * (xc - x1 - tnorm * dx) + dy * (yc - y1 - tnorm * dy) = 0
  // dt is between norm and intersection
  // dt^2 * (dx * dx + dy * dy) = r * r - d * d
  ld dx = x2 - x1;
  ld dy = y2 - y1;
  ld dd = dx * dx + dy * dy;
  ld tnorm = (dx * (xc - x1) + dy * (yc - y1)) / dd;
  ld d2 = dist2(xc, yc, x1 + dx * tnorm, y1 + dy * tnorm);
  if (d2 > rc2 - eps) {
    // out ("intersect. false1", x1, y1, x2, y2, tmin, xc, yc, rc2);
    return false;
  }
  ld dt = sqrtl((rc2 - d2) / dd);
  t = (tnorm - dt > tmin + eps) ? tnorm - dt : tnorm + dt;
  if (t < tmin + eps || t > 1 - eps) {
    // out ("intersect. false2", x1, y1, x2, y2, tmin, xc, yc, rc2, "t=", t);
    return false;
  }
  // out ("intersect. true", x1, y1, x2, y2, tmin, xc, yc, rc2, "d2=", d2, "t=", t);
  return true;
}

void solve(Input& in, Output& out) {
  vec<string> signals({ "VIOLET", "INDIGO", "BLUE", "GREEN", "YELLOW", "ORANGE", "RED" });
  const int RED = 6;
  out.setPrecision(3);

  int n = in.ni();
  vec<i64> xc(n);
  vec<i64> yc(n);
  vec<i64> rc(n);
  auto rs2 = newVec2d(n, RED, (ld)0);
  vec<string> sc(n);
  for (int i = 0; i < n; i++) {
    xc[i] = in.ni();
    yc[i] = in.ni();
    rc[i] = in.ni();
    sc[i] = in.ns();
    rs2[i][0] = rc[i] * rc[i];
    for (int j = 1; j < RED; j++)
      rs2[i][j] = rc[i] * rc[i] * powl((ld) 10.0, (ld) 0.4 * j);
  }
  // out ("rs2", rs2);

  auto find_best = [&](ld x, ld y, ld towards_x, ld towards_y) {
    int best = -1;
    ld best_power = 0;
    for (int i = 0; i < n; i++) {
      ld d2 = dist2(xc[i], yc[i], x, y);
      ld inv_power = max((ld) 1, d2 / (rc[i] * rc[i]));
      // out.setPrecision(15);
      // out ("find_best", x, y, i, "d2=", d2, "inv_power=", inv_power);
      // out.setPrecision(3);
      if (best == -1 || (inv_power < best_power - eps) || (inv_power < best_power + eps && sc[i] < sc[best])) {
        best = i;
        best_power = inv_power;
      }
    }
    // out ("find_best", x, y, "best=", best, "best_power=", best_power);
    ld d2 = dist2(xc[best], yc[best], towards_x, towards_y);
    for (int level = 0; level < RED; level++) {
      if (d2 < rs2[best][level])
        return make_pair(best, level);
    }
    return make_pair(best, RED);
  };

  // // out ("rs2", rs2);

  int cur = -1;
  int level = -1;

  int m = in.ni();
  for (int index = 0; index < m; index++) {
    i64 xs = in.ni();
    i64 ys = in.ni();
    i64 xe = in.ni();
    i64 ye = in.ni();

    if (index == 0) {
      tie(cur, level) = find_best(xs, ys, xs, ys);
      out("Power on. CELL_ID:" + sc[cur] + ", SIGNAL_LEVEL:" + signals[level]);
    }

    ld t = 0;
    bool must_find_not_red = false;
    while (t < 1 - eps) {
      if (level < RED) {
        ld it1, it2;
        bool has1, has2;
        has1 = intersect(out, xs, ys, xe, ye, t, xc[cur], yc[cur], rs2[cur][level], it1);
        has2 = level > 0 && intersect(out, xs, ys, xe, ye, t, xc[cur], yc[cur], rs2[cur][level-1], it2);
        if (has1 && (!has2 || it1 < it2)) {
          t = it1;
          level++;
          out("Signal changed. SIGNAL_LEVEL:" + signals[level]);
        } else if (has2) {
          t = it2;
          level--;
          out("Signal changed. SIGNAL_LEVEL:" + signals[level]);
        } else {
          t = 1;
        }
        must_find_not_red = false;
      } else {
        auto p = find_best(xs + (xe - xs) * t, ys + (ye - ys) * t, xs + (xe - xs) * (t + eps), ys + (ye - ys) * (t + eps));
        if (p.first != -1 && p.second == RED && must_find_not_red)
          panic();
        if (p.first != -1 && p.second != RED) {
          if (cur == p.first) {
            cur = p.first;
            level = p.second;
            out("Signal changed. SIGNAL_LEVEL:" + signals[level]);
          } else {
            cur = p.first;
            level = p.second;
            out("Cell changed. CELL_ID:" + sc[cur] + ", SIGNAL_LEVEL:" + signals[level]);
          }
          must_find_not_red = false;
        } else {
          ld mint = 1;
          for (int i = 0; i < n; i++) {
            ld it;
            if (intersect(out, xs, ys, xe, ye, t, xc[i], yc[i], rs2[i][RED - 1], it))
              mint = min(mint, it);
          }
          t = mint;
          must_find_not_red = true;
        }
      }
    }
  }
}
