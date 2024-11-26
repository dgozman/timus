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
  vec<i64> r(n);
  for (int i = 0; i < n; i++) {
    x[i] = in.ni();
    y[i] = in.ni();
    r[i] = in.ni();
  }

  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      if (x[i] == x[j] && y[i] == y[j] && r[i] == r[j]) {
        x[j] = x[n - 1];
        y[j] = y[n - 1];
        r[j] = r[n - 1];
        n--;
        j--;
      }
    }
  }

  vec<int> parent(n);
  for (int i = 0; i < n; i++)
    parent[i] = i;
  function<int(int)> find = [&](int x) {
    if (parent[x] == x)
      return x;
    return parent[x] = find(parent[x]);
  };
  auto unite = [&](int x, int y) {
    x = find(x);
    y = find(y);
    if (x != y)
      parent[x] = y;
  };

  vec<pair<D, D>> p;
  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      i64 x2 = x[j] - x[i];
      i64 y2 = y[j] - y[i];

      if (x2 == 0 && y2 == 0)
        continue;

      i64 a = -2 * x2;
      i64 b = -2 * y2;
      i64 c = x2 * x2 + y2 * y2 + r[i] * r[i] - r[j] * r[j];
      i64 R = r[i];

      i64 d = (a * a + b * b);
      if (c * c > R * R * d)
        continue;

      unite(i, j);

      D x0 = -a * c / (D) d;
      D y0 = -b * c / (D) d;
      if (c * c == R * R * d) {
        p.emplace_back(x0 + x[i], y0 + y[i]);
      } else {
        D dist = R * R - c * c / (D) d;
        D mult = sqrtl(dist / d);
        p.emplace_back(x0 + b * mult + x[i], y0 - a * mult + y[i]);
        p.emplace_back(x0 - b * mult + x[i], y0 + a * mult + y[i]);
      }
    }
  }

  // out("p", p);

  sort(p.begin(), p.end());
  vec<pair<D, D>> u;
  for (int i = 0; i < p.size(); i++) {
    if (u.empty() || fabsl(u.back().first - p[i].first) > 1e-5 || fabsl(u.back().second - p[i].second) > 1e-5)
      u.push_back(p[i]);
  }

  // out("u", u);

  int V = u.size();

  int C = 0;
  for (int i = 0; i < n; i++) {
    if (find(i) == i)
      C++;
  }

  int E = 0;
  for (int i = 0; i < n; i++) {
    int count = 0;
    for (int j = 0; j < u.size(); j++) {
      D dx = x[i] - u[j].first;
      D dy = y[i] - u[j].second;
      D dist2 = dx * dx + dy * dy;
      if (fabsl(dist2 - r[i] * r[i]) < 1e-8)
        count++;
    }
    // out(i, x[i], y[i], count);
    if (count == 0) {
      V++;
      count = 1;
    }
    E += count;
  }

  // out("E", E, "V", V, "C", C);

  out(E - V + C + 1);
}
