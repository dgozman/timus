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
using pt = pair<int, int>;

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

void solve(Input& in, Output& out) {
  int n = in.ni();
  vec2d<int> rx(n);
  vec2d<int> ry(n);
  for (int i = 0; i < n; i++) {
    int k = in.ni();
    for (int j = 0; j < k; j++) {
      double x = in.next<double>();
      double y = in.next<double>();
      rx[i].push_back(round(x * 100));
      ry[i].push_back(round(y * 100));
    }
  }

  vec<int> parent(n);
  for (int i = 0; i < n; i++)
    parent[i] = i;

  function<int(int)> getp = [&](int v) {
    if (parent[v] != v)
      parent[v] = getp(parent[v]);
    return parent[v];
  };

  auto unite = [&](int a, int b) {
    a = getp(a);
    b = getp(b);
    if (a != b)
      parent[a] = b;
  };

  auto between = [](int v, int a, int b) {
    return (a <= v && v <= b) || (b <= v && v <= a);
  };

  auto inside = [&](int px, int py, int x1, int y1, int x2, int y2) {
    int64_t cross = (int64_t)(x2 - x1) * (py - y1) - (int64_t)(y2 - y1) * (px - x1);
    return between(px, x1, x2) && between(py, y1, y2) && cross == 0;
  };

  auto cmp = [&](pt a, pt b) {
	  return a.first < b.first || a.first == b.first && a.second < b.second;
  };

  auto clock_wise = [&](pt a, pt b, pt c) {
	  return (int64_t) a.first * (b.second - c.second) + (int64_t) b.first * (c.second - a.second) + (int64_t) c.first * (a.second - b.second) < 0;
  };

  auto counter_clock_wise = [&](pt a, pt b, pt c) {
	  return (int64_t) a.first * (b.second - c.second) + (int64_t) b.first * (c.second - a.second) + (int64_t) c.first * (a.second - b.second) > 0;
  };

  auto convex_hull = [&](vec<pt>& a) {
    if (a.size() == 1)
      return;
    sort(a.begin(), a.end(), cmp);
    pt p1 = a[0];
    pt p2 = a.back();
    vector<pt> up({p1});
    vector<pt> down({p1});
    for (int i = 1; i < a.size(); ++i) {
      if (i == a.size() - 1 || clock_wise(p1, a[i], p2)) {
        while (up.size() >= 2 && !clock_wise(up[up.size() - 2], up[up.size() - 1], a[i]))
          up.pop_back();
        up.push_back(a[i]);
      }
      if (i == a.size() - 1 || counter_clock_wise(p1, a[i], p2)) {
        while (down.size() >= 2 && !counter_clock_wise(down[down.size() - 2], down[down.size() - 1], a[i]))
          down.pop_back();
        down.push_back(a[i]);
      }
    }
    a.clear();
    for (int i = 0; i < up.size(); ++i)
      a.push_back(up[i]);
    for (int i = down.size() - 2; i > 0; --i)
      a.push_back(down[i]);
  };

  auto area = [&](const vec<pt>& p) {
    int64_t result = 0;
    for (int i = 0; i < p.size(); i++) {
      int j = (i + 1) % p.size();
      result += (int64_t) p[i].first * p[j].second - (int64_t) p[j].first * p[i].second;
    }
    return abs(result);
  };

  for (int i = 0; i < n; i++) {
    int sx = rx[i][0];
    int sy = ry[i][0];
    int ex = rx[i][rx[i].size() - 1];
    int ey = ry[i][ry[i].size() - 1];
    for (int j = 0; j < n; j++) {
      if (i == j)
        continue;
      for (int c = 0; c < rx[j].size() - 1; c++) {
        if (inside(sx, sy, rx[j][c], ry[j][c], rx[j][c + 1], ry[j][c + 1]) || inside(ex, ey, rx[j][c], ry[j][c], rx[j][c + 1], ry[j][c + 1])) {
          unite(i, j);
          break;
        }
      }
    }
  }

  int64_t result = 0;
  for (int i = 0; i < n; i++) {
    if (getp(i) != i)
      continue;

    vec<pt> p;
    for (int j = 0; j < n; j++) {
      if (getp(j) == i) {
        for (int c = 0; c < rx[j].size(); c++)
          p.push_back({rx[j][c], ry[j][c]});
      }
    }

    convex_hull(p);
    result = max(result, area(p));
  }

  out.setPrecision(2);
  out(result * 1.0 / 20000);
}
