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

using ld = long double;
const ld eps = 1e-8;
const int RED = 6;

ld sqr(ld x) {
  return x * x;
}

struct Signal {
  int cell;
  int level;
  Signal(int cell, int level): cell(cell), level(level) {}
};

void solve(Input& in, Output& out) {
  int n = in.ni();
  vec<int> xc(n), yc(n), rc(n);
  vec2d<ld> rs2 = newVec2d(n, RED, (ld)0.0);
  vec<string> sc(n);
  for (int i = 0; i < n; i++) {
    xc[i] = in.ni();
    yc[i] = in.ni();
    rc[i] = in.ni();
    sc[i] = in.ns();
    for (int j = 0; j < RED; j++)
      rs2[i][j] = rc[i] * rc[i] * pow((ld) 10.0, 0.4 * j);
  }

  auto get_signal2 = [&] (ld x, ld y, int cell) {
    ld dist2 = sqr(x - xc[cell]) + sqr(y - yc[cell]);
    int level = 0;
    while (level < RED && dist2 > rs2[cell][level] - eps)
      level++;
    return make_pair(dist2 / (rc[cell] * rc[cell]), Signal(cell, level));
  };

  auto choose = [&] (ld x, ld y) {
    pair<ld, Signal> best(1e100, Signal(0, 100));
    for (int cell = 0; cell < n; cell++) {
      auto s = get_signal2(x, y, cell);
      if (s.first < best.first - eps || (s.first < best.first + eps && sc[s.second.cell] < sc[best.second.cell]))
        best = s;
    }
    return best.second;
  };

  auto find_change = [&] (ld x1, ld y1, ld x2, ld y2, int cell, int level, ld min_t) -> ld {
    ld dx = x2 - x1;
    ld dy = y2 - y1;
    ld dd = dx * dx + dy * dy;
    ld tnorm = (dx * (xc[cell] - x1) + dy * (yc[cell] - y1)) / dd;
    ld d2 = sqr(x1 + dx * tnorm - xc[cell]) + sqr(y1 + dy * tnorm - yc[cell]);
    if (d2 > rs2[cell][level] - eps)
      return -2.0;
    ld dt = sqrt((rs2[cell][level] - d2) / dd);
    ld t = (tnorm - dt > min_t + eps) ? tnorm - dt : tnorm + dt;
    if (t < min_t + eps || t > 1 - eps)
      return -2.0;
    return t;
  };

  vec<string> levels({ "VIOLET", "INDIGO", "BLUE", "GREEN", "YELLOW", "ORANGE", "RED" });
  Signal current(0, 100);

  int moves = in.ni();
  for (int move = 0; move < moves; move++) {
    int xs = in.ni();
    int ys = in.ni();
    int xe = in.ni();
    int ye = in.ni();

    if (move == 0) {
      current = choose(xs, ys);
      out("Power on. CELL_ID:" + sc[current.cell] + ", SIGNAL_LEVEL:" + levels[current.level]);
    }

    ld t = 0;
    while (t < 1 - eps) {
      if (current.level < RED) {
        ld t1 = find_change(xs, ys, xe, ye, current.cell, current.level, t);
        ld t2 = current.level == 0 ? -2 : find_change(xs, ys, xe, ye, current.cell, current.level - 1, t);
        if (t1 < -1 && t2 < -1) {
          t = 1;
        } else if (t2 < -1 || (t1 > -1 && t1 < t2)) {
          current.level++;
          t = t1;
          out("Signal changed. SIGNAL_LEVEL:" + levels[current.level]);
        } else {
          current.level--;
          t = t2;
          out("Signal changed. SIGNAL_LEVEL:" + levels[current.level]);
        }
      } else {
        Signal s = choose(xs + (xe - xs) * t, ys + (ye - ys) * t);
        if (s.level < RED && s.cell != current.cell) {
          current = s;
          out("Cell changed. CELL_ID:" + sc[current.cell] + ", SIGNAL_LEVEL:" + levels[current.level]);
        } else {
          ld t_next = -2.0;
          int cell_next = current.cell;
          for (int cell = 0; cell < n; cell++) {
            ld t_cell = find_change(xs, ys, xe, ye, cell, RED - 1, t);
            if (t_cell < -1)
              continue;
            if (t_next < -1 || t_cell < t_next - eps || (t_cell < t_next + eps && sc[cell] < sc[cell_next])) {
              t_next = t_cell;
              cell_next = cell;
            }
          }
          if (t_next < -1) {
            t = 1;
          } else if (cell_next == current.cell) {
            current.level--;
            t = t_next;
            out("Signal changed. SIGNAL_LEVEL:" + levels[current.level]);
          } else {
            t = t_next;
            current.cell = cell_next;
            current.level = RED - 1;
            out("Cell changed. CELL_ID:" + sc[current.cell] + ", SIGNAL_LEVEL:" + levels[current.level]);
          }
        }
      }
    }
  }
}
