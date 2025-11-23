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
  int inf = 10000;
  int max_allowed = in.ni();
  int n = in.ni();
  auto c = newVec2d<int>(2 * n, 2 * n, 0);
  int edges = in.ni();
  int s = 2 * (in.ni() - 1) + 1;
  int t = 2 * (in.ni() - 1);

  for (int i = 0; i < n; i++)
    c[2 * i][2 * i + 1] = in.ni();
  for (int i = 0; i < edges; i++) {
    int u = in.ni() - 1;
    int v = in.ni() - 1;
    c[2 * u + 1][2 * v] = inf;
    c[2 * v + 1][2 * u] = inf;
  }

  vec<int> ptr(2 * n, 0);
  vec<int> q(2 * n, 0);
  vec<int> dist(2 * n, 0);
  auto f = newVec2d<int>(2 * n, 2 * n, 0);

  auto bfs = [&]() {
    int head = 0, tail = 0;
    q[head++] = s;
    for (int i = 0; i < 2 * n; i++)
      dist[i] = -1;
    dist[s] = 0;
    while (head > tail) {
      int u = q[tail++];
      for (int v = 0; v < 2 * n; v++) {
        if (dist[v] == -1 && f[u][v] < c[u][v]) {
          q[head++] = v;
          dist[v] = dist[u] + 1;
        }
      }
    }
    return dist[t] != -1;
  };

  function<int(int, int)> dfs = [&](int u, int flow) {
    if (flow == 0)
      return 0;
    if (u == t)
      return flow;
    for (int& v = ptr[u]; v < 2 * n; v++) {
      if (dist[v] != dist[u] + 1)
        continue;
      int pushed = dfs(v, min(flow, c[u][v] - f[u][v]));
      if (pushed > 0) {
        f[u][v] += pushed;
        f[v][u] -= pushed;
        return pushed;
      }
    }
    return 0;
  };

  auto dinic = [&]() {
    int flow = 0;
    while (true) {
      if (!bfs())
        break;
      for (int i = 0; i < 2 * n; i++)
        ptr[i] = 0;
      while (int pushed = dfs(s, inf * inf))
        flow += pushed;
    }
    return flow;
  };

  if (s == t + 1) {
    out("NO");
    return;
  }

  if (dinic() <= max_allowed)
    out("YES");
  else
    out("NO");
}
