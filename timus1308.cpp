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

struct Position {
  using Encoded = tuple<bool, int, int, int>;
  bool square;
  int dx, dy, index;
  Position(Encoded e): square(std::get<0>(e)), dx(std::get<1>(e)), dy(std::get<2>(e)), index(std::get<3>(e)) {}
  tuple<bool, int, int, int> encode() {
    return make_tuple(square, dx, dy, index);
  }
};

double sqr(double x) {
  return x * x;
}

void solve(Input& in, Output& out) {
  double rx = in.next<double>();
  double ry = in.next<double>();
  double result = sqrt(sqr(rx) + sqr(ry));

  vec<Position::Encoded> queue;
  map<Position::Encoded, int> d;
  auto relax = [&] (Position::Encoded e, int dist) {
    if (d.find(e) == d.end()) {
      d[e] = dist;
      if (dist < 70)
        queue.push_back(e);
    }
  };

  relax(make_tuple(true, 0, 0, 0), 0);
  const double big = 0.5 + sqrt(3) / 2;
  const double big2 = (3 + sqrt(3)) / 4;
  for (int tail = 0; tail < queue.size(); tail++) {
    Position p(queue[tail]);
    int current = d[queue[tail]];
    if (p.square) {
      double x = p.dx * big + p.dy * big;
      double y = p.dx * (big + 1) - p.dy * (big + 1);
      if (p.index == 1) {
        x += big / 2;
        y += big2;
      }
      if (p.index == 2) {
        x += 3 * big / 2;
        y += big2;
      }
      result = min(result, sqrt(sqr(x - rx) + sqr(y - ry)));
      result = min(result, sqrt(sqr(x - ry) + sqr(y - rx)));

      if (p.index == 0) {
        relax(make_tuple(false, p.dx, p.dy, 0), current + 1);
        relax(make_tuple(false, p.dx - 1, p.dy - 1, 3), current + 1);
      }
      if (p.index == 1) {
        relax(make_tuple(false, p.dx, p.dy, 1), current + 1);
        relax(make_tuple(false, p.dx, p.dy - 1, 4), current + 1);
      }
      if (p.index == 2) {
        relax(make_tuple(false, p.dx, p.dy, 2), current + 1);
        relax(make_tuple(false, p.dx + 1, p.dy, 5), current + 1);
      }
    } else {
      if (p.index == 0) {
        relax(make_tuple(true, p.dx, p.dy, 0), current + 1);
        relax(make_tuple(false, p.dx, p.dy, 1), current + 1);
        relax(make_tuple(false, p.dx, p.dy, 5), current + 1);
      }
      if (p.index == 1) {
        relax(make_tuple(true, p.dx, p.dy, 1), current + 1);
        relax(make_tuple(false, p.dx, p.dy, 2), current + 1);
        relax(make_tuple(false, p.dx, p.dy, 0), current + 1);
      }
      if (p.index == 2) {
        relax(make_tuple(true, p.dx, p.dy, 2), current + 1);
        relax(make_tuple(false, p.dx, p.dy, 3), current + 1);
        relax(make_tuple(false, p.dx, p.dy, 1), current + 1);
      }
      if (p.index == 3) {
        relax(make_tuple(true, p.dx + 1, p.dy + 1, 0), current + 1);
        relax(make_tuple(false, p.dx, p.dy, 4), current + 1);
        relax(make_tuple(false, p.dx, p.dy, 2), current + 1);
      }
      if (p.index == 4) {
        relax(make_tuple(true, p.dx, p.dy + 1, 1), current + 1);
        relax(make_tuple(false, p.dx, p.dy, 5), current + 1);
        relax(make_tuple(false, p.dx, p.dy, 3), current + 1);
      }
      if (p.index == 5) {
        relax(make_tuple(true, p.dx - 1, p.dy, 2), current + 1);
        relax(make_tuple(false, p.dx, p.dy, 0), current + 1);
        relax(make_tuple(false, p.dx, p.dy, 4), current + 1);
      }
    }
  }

  out.setPrecision(4);
  out(result);
}
