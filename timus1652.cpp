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
  int n = in.ni();
  vec<int> country(n);
  vec<int> amount(n);
  for (int i = 0; i < n; i++) {
    country[i] = in.ni();
    amount[i] = in.ni();
  }

  int m = in.ni();
  vec<int> a(m);
  vec<int> b(m);
  vec<int> w(m);
  for (int i = 0; i < m; i++) {
    a[i] = in.ni() - 1;
    b[i] = in.ni() - 1;
    w[i] = in.ni();
  }

  // ...left, t, ...right, s
  int t = n;
  int s = n + m + 1;
  vec<int> taken(n, -1);
  vec<int> taken_by(m, -1);

  int V = n + m + 2;
  vec<int> ptr(V, 0);
  vec<int> ptr_end(V, 0);
  vec<int> q(V, 0);
  vec<int> dist(V, 0);

  auto can = [&](int u, int v) {
    if (u == s)
      return v >= 0 && v < t && taken[v] == -1;
    if (u == t)
      return v > t && v < s && taken_by[v - (t + 1)] != -1;
    if (u >= 0 && u < t) {
      if (v == s)
        return taken[v] != -1;
      if (v > t && v < s) {
        int e = v - (t + 1);
        return taken[u] != e && u != b[e] && country[b[e]] == country[u] && (a[e] == u || w[e] <= amount[u]);
      }
      return false;
    }
    int e = u - (t + 1);
    if (v == t)
      return taken_by[e] == -1;
    if (v < t)
      return taken[v] == e;
    return false;
  };

  auto mark = [&](int u, int v) {
    if (u >= 0 && u < t && v > t && v < s) {
      taken[u] = v - (t + 1);
      taken_by[v - (t + 1)] = u;
    }
  };

  auto bfs = [&]() {
    int head = 0, tail = 0;
    q[head++] = s;
    for (int i = 0; i < V; i++)
      dist[i] = -1;
    dist[s] = 0;
    while (head > tail) {
      int u = q[tail++];
      for (int v = ptr[u]; v < ptr_end[u]; v++) {
        if (dist[v] == -1 && can(u, v)) {
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
    for (int& v = ptr[u]; v < ptr_end[u]; v++) {
      if (dist[v] != dist[u] + 1)
        continue;
      int push = can(u, v) ? 1 : 0;
      int pushed = dfs(v, min(flow, push));
      if (pushed > 0) {
        mark(u, v);
        return pushed;
      }
    }
    return 0;
  };

  auto dinic = [&]() {
    int flow = 0;
    while (true) {
      ptr[s] = 0;
      ptr_end[s] = t;
      for (int i = 0; i < n; i++) {
        ptr[i] = t + 1;
        ptr_end[i] = s + 1;
      }
      for (int i = 0; i < m; i++) {
        ptr[i + (t + 1)] = 0;
        ptr_end[i + (t + 1)] = t + 1;
      }
      ptr[t] = t + 1;
      ptr_end[t] = s;
      if (!bfs())
        break;
      while (int pushed = dfs(s, 1))
        flow += pushed;
    }
    return flow;
  };

  out(dinic());
  // out(taken);
}
