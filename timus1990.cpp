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
  //  ifstream fin("input.txt");
  //  Input in(fin);
  //  ofstream fout("output.txt");
  //  Output out(fout);
  Input in(cin);
  Output out(cout);
  solve(in, out);
  return 0;
}

void solve(Input& in, Output& out) {
  set<int> ys;

  int n = in.ni();
  vec<int> nx(n), ny(n);
  for (int i = 0; i < n; i++) {
    nx[i] = in.ni();
    ny[i] = in.ni();
    ys.insert(ny[i]);
  }

  int m = in.ni();
  vec<int> mx(m), my(m);
  for (int i = 0; i < m; i++) {
    mx[i] = in.ni();
    my[i] = in.ni();
    ys.insert(my[i]);
  }

  int q = in.ni();
  vec<pair<int, int>> qs(q);
  for (int i = 0; i < q; i++) {
    int qx = in.ni();
    int qy = in.ni();
    qs[i] = make_pair(qy, qx);
    ys.insert(qy);
  }
  sort(qs.begin(), qs.end());

  int ni = 0;
  int mi = 0;
  int qi = 0;
  double res = 1e10;
  for (int y : ys) {
    if (y < ny[0] || y > ny[n - 1])
      continue;

    double lx;
    if (ny[ni] == y) {
      lx = nx[ni];
    } else if (ny[ni + 1] == y) {
      ni++;
      lx = nx[ni];
    } else {
      lx = nx[ni] + 1.0 * (nx[ni + 1] - nx[ni]) * (y - ny[ni]) / (ny[ni + 1] - ny[ni]);
    }

    double rx;
    if (my[mi] == y) {
      rx = mx[mi];
    } else if (my[mi + 1] == y) {
      mi++;
      rx = mx[mi];
    } else {
      rx = mx[mi] + 1.0 * (mx[mi + 1] - mx[mi]) * (y - my[mi]) / (my[mi + 1] - my[mi]);
    }

    double max_width = 0.0;
    double last = lx;
    while (qi < q && qs[qi].first == y) {
      int qx = qs[qi].second;
      if (qx > lx && qx < rx) {
        max_width = max(max_width, qx - last);
        last = qx;
      }
      qi++;
    }
    max_width = max(max_width, rx - last);
    res = min(res, max_width);
  }

  out.setPrecision(10);
  out(res);
}
