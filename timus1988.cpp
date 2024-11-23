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
using ld = long double;

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

long double norm_squared(long double x, long double y, long double z) {
  return x * x + y * y + z * z;
}

long double norm(long double x, long double y, long double z) {
  return sqrt(norm_squared(x, y, z));
}

void solve(Input& in, Output& out) {
  out.setPrecision(5);

  long double x1 = in.next<long double>();
  long double y1 = in.next<long double>();
  long double z1 = in.next<long double>();
  long double x2 = in.next<long double>();
  long double y2 = in.next<long double>();
  long double z2 = in.next<long double>();
  long double v = in.next<long double>();

  long double r2 = norm_squared(x1, y1, z1);
  long double r = norm(x1, y1, z1);
  long double d2 = norm_squared(x2, y2, z2);
  long double d = norm(x2, y2, z2);

  long double min_cosa = (r2 + d2 - norm_squared(x1 - x2, y1 - y2, z1 - z2)) / (2 * r * d);
  long double max_cosa = 1;
  long double tangent_cosa = r / d;
  long double tangent = sqrt(d2 - r2);

  long double max_a = acos(min_cosa);
  long double min_a = 0;
  long double tangent_a = acos(tangent_cosa);

  // out("r", r);
  // out("d", d);
  // out("min_cosa", min_cosa);
  // out("tangent_cosa", tangent_cosa);
  // out("max_a", max_a);
  // out("tangent", tangent);
  // out("tangent_a", tangent_a);

  auto get_time = [&] (long double a) {
    long double cosa = cos(a);
    long double sina2 = 1 - cosa * cosa;
    if (cosa > tangent_cosa) {
      // r * cosa - d, r * sina
      long double dist = sqrt(r2 * sina2 + r2 * cosa * cosa + d2 - 2 * r * d * cosa);
      // out("cosa", cosa, "a", a, "dist", dist, "ang", (max_a - a));
      return max(dist / v, (max_a - a) * r);
    } else {
      // out("cosa", cosa, "a", a, "ang1", (a - tangent_a), "ang2", (max_a - a));
      return max(tangent / v + (a - tangent_a) * r / v, (max_a - a) * r);
    }
  };

  long double result = 1e100;
  long double best_a = min_a;

  auto iterate = [&] (long double from, long double to, int steps) {
    long double delta = (to - from) / steps;
    for (int i = 0; i <= steps; i++) {
      long double a = from + delta * i;
      long double time = get_time(a);
      if (time < result) {
        result = time;
        best_a = a;
      }
    }
  };

  long double from = min_a;
  long double to = max_a;
  for (int i = 0; i < 10; i++) {
    int STEPS = 100000;
    iterate(from, to, STEPS);
    long double delta = (to - from) / (STEPS / 100);
    from = max(from, best_a - delta);
    to = min(to, best_a + delta);
  }

  out.setPrecision(15);
  out(result);
}
