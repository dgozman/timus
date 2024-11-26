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

using D = long double;

void solve(Input& in, Output& out) {
  int n = in.ni();
  vec<i64> x(n);
  vec<i64> y(n);
  for (int i = 0; i < n; i++) {
    x[i] = in.ni();
    y[i] = in.ni();
  }

  D result = 1e100;
  for (int center = 0; center < n; center++) {
    vec<pair<i64, i64>> pa;
    for (int i = 0; i < n; i++) {
      if (x[i] != x[center] || y[i] != y[center]) {
        i64 dx = x[i] - x[center];
        i64 dy = y[i] - y[center];
        pa.push_back(make_pair(dx, dy));
      }
    }

    auto compare = [](const pair<i64, i64>& a, const pair<i64, i64>& b) -> bool {
      bool a_top = a.second > 0 || a.second == 0 && a.first > 0;
      bool b_top = b.second > 0 || b.second == 0 && b.first > 0;
      if (a_top && !b_top)
        return true;
      if (!a_top && b_top)
        return false;
      i64 v = a.first * b.second - a.second * b.first;
      if (v == 0) {
        i64 da = a.first * a.first + a.second * a.second;
        i64 db = b.first * b.first + b.second * b.second;
        return da < db;
      }
      return v > 0;
    };

    // out("before here", pa);
    sort(pa.begin(), pa.end(), [&](const auto& a, const auto& b) {
      bool res = compare(a, b);
      // out("cmp", a, " vs ", b, " => ", res);
      return res;
    });

    // out("here", pa);

    if (!pa.size()) {
      result = 0;
      break;
    }

    vec<pair<pair<i64, i64>, int>> p;
    int m = 0;
    i64 sum_x = 0;
    i64 sum_y = 0;
    for (int i = 0; i < pa.size(); i++) {
      sum_x += pa[i].first;
      sum_y += pa[i].second;
      if (m > 0 && pa[i] == p[m - 1].first) {
        p[m - 1].second++;
      } else {
        p.push_back(make_pair(pa[i], 1));
        m++;
      }
    }
    for (int i = 0; i < m; i++)
      p.push_back(p[i]);

    auto same_side = [&](pair<i64, i64>& left, pair<i64, i64>& right) {
      i64 s = left.first * right.second - left.second * right.first;
      // out("same_side", left, right, s);
      if (s != 0)
        return s > 0;
      i64 c = left.first * right.first + left.second * right.second;
      if (c > 0) {
        i64 d1 = left.first * left.first + left.second * left.second;
        i64 d2 = right.first * right.first + right.second * right.second;
        return d2 >= d1;
      }
      return true;
    };

    auto iabs = [](i64 x) { return x < 0 ? -x : x; };

    // out("p", p);

    int right = 0;
    i64 top_x = p[0].first.first * p[0].second;
    i64 top_y = p[0].first.second * p[0].second;

    for (int left = 0; left < m; left++) {
      // out("before", left, right);
      while (right + 1 < left + m && same_side(p[left].first, p[right + 1].first)) {
        right++;
        top_x += p[right].first.first * p[right].second;
        top_y += p[right].first.second * p[right].second;
      }
      // out("after", left, right);

      // a * x + b * y + c = 0
      // a * x[left] + b * y[left] = 0
      i64 a = p[left].first.second;
      i64 b = -p[left].first.first;
      D dist = iabs(a * top_x + b * top_y) + iabs(a * (sum_x - top_x) + b * (sum_y - top_y));
      D den = a * a + b * b;
      result = min(result, dist / sqrtl(den));

      // out(center, left, result);

      top_x -= p[left].first.first * p[left].second;
      top_y -= p[left].first.second * p[left].second;
    }
  }

  out.setPrecision(15);
  out(result);
}
