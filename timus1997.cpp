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

void solve(Input& in, Output& out) {
  int a = in.ni();
  int b = in.ni();
  int n = in.ni() / 2;
  vec<int> t1(n * 2);
  vec<int> t2(n * 2);
  int ct1 = 0;
  int ct2 = 0;
  for (int i = 0; i < 2 * n; i++) {
    int t = in.ni();
    int d = in.ni();
    if (d == 0)
      t1[ct1++] = t;
    else
      t2[ct2++] = t;
  }

  if (ct1 != n || ct2 != n) {
    out("Liar");
    return;
  }

  vec<int> p1(n, -1);
  vec<int> p2(n, -1);
  vec<int> visited(n);

  function<bool(int)> dfs = [&] (int u) {
    visited[u] = 1;
    for (int v = 0; v < n; v++) {
      if (t2[v] > t1[u] && (t2[v] <= t1[u] + b || t2[v] >= t1[u] + a)) {
        if (p2[v] == -1 || !visited[p2[v]] && dfs(p2[v])) {
          p1[u] = v;
          p2[v] = u;
          return true;
        }
      }
    }
    return false;
  };

  int total = 0;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++)
      visited[j] = 0;
    for (int j = 0; j < n; j++) {
      if (p1[j] == -1 && !visited[j]) {
        if (dfs(j))
          total++;
      }
    }
  }
  if (total == n) {
    out("No reason");
    for (int i = 0; i < n; i++)
      out(t1[i], t2[p1[i]]);
  } else {
    out("Liar");
  }
}
