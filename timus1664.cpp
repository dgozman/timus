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

struct Edge {
  i16 end;
  i32 cap, flow;
  u16 next;
};

const i32 INF = 2000000001;

void solve(Input& in, Output& out) {
  int n = in.ni();
  for (int i = 0; i < n; i++) {
    in.ni();
    in.ni();
  }

  int m = in.ni();
  u16 NONE = 2 * m + 1;
  vec<Edge> e(2 * m);
  vec<u16> first(n, NONE);
  for (int i = 0; i < m; i++) {
    int u = in.ni() - 1;
    int v = in.ni() - 1;
    int cap = in.ni();
    e[2 * i].end = v;
    e[2 * i].cap = cap;
    e[2 * i].flow = 0;
    e[2 * i].next = first[u];
    first[u] = 2 * i;
    e[2 * i + 1].end = u;
    e[2 * i + 1].cap = cap;
    e[2 * i + 1].flow = 0;
    e[2 * i + 1].next = first[v];
    first[v] = 2 * i + 1;
  }

  int s = 0;
  int t = n - 1;

  vec<i16> level(n);
  vec<i16> q(n);
  auto bfs = [&] () {
    std::fill(level.begin(), level.end(), -1);
    level[s] = 0;
    int qc = 0;
    q[qc++] = s;
    for (int i = 0; i < qc; i++) {
      i16 u = q[i];
      u16 j = first[u];
      while (j != NONE) {
        i16 v = e[j].end;
        if (level[v] == -1 && e[j].flow < e[j].cap) {
          level[v] = level[u] + 1;
          q[qc++] = v;
        }
        j = e[j].next;
      }
    }
    return level[t] != -1;
  };

  vec<u16> ptr(n);
  function<i32(i16, i32)> dfs = [&] (i16 u, i32 flow) {
    if (u == t)
      return flow;
    i32 total = 0;
    u16& j = ptr[u];
    while (j != NONE) {
      if (level[e[j].end] == level[u] + 1 && e[j].cap - e[j].flow > 0) {
        i32 pushed = dfs(e[j].end, min(flow, e[j].cap - e[j].flow));
        if (pushed) {
          e[j].flow += pushed;
          e[j ^ 1].flow -= pushed;
          total += pushed;
          flow -= pushed;
          if (flow == 0)
            break;
        }
      }
      j = e[j].next;
    }
    return total;
  };

  i32 flow = 0;
  while (bfs()) {
    std::copy(first.begin(), first.end(), ptr.begin());
    while (i32 pushed = dfs(s, INF))
      flow += pushed;
  }

  out(flow);
  for (int i = 0; i < m; i++) {
    i16 u = e[2 * i].end;
    i16 v = e[2 * i + 1].end;
    if (e[2 * i].flow > 0)
      out(v + 1, u + 1, e[2 * i].flow);
    else
      out(u + 1, v + 1, e[2 * i + 1].flow);
  }
}
