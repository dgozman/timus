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
  template<typename T, typename...Ts> inline void print(const T &f, const Ts&... args) {
    *out << f;
    if (sizeof...(args) != 0) { *out << ' '; print(args...); }
  }
  template<typename...Ts> inline void println(const Ts&... args) { print(args...); *out << '\n'; }
  template<typename A, typename B> inline void println(const pair<A, B>& pair) { print(pair.first); *out << ' '; print(pair.second); *out << '\n'; }
  template<typename T> inline void printVec(const vec<T>& v) { for (int i = 0; i < v.size(); i++) { if (i) *out << ' '; print(v[i]); } }
  template<typename T> inline void printlnVec(const vec<T>& v) { printVec(v); *out << '\n'; }
  template<typename...Ts> inline void operator() (const Ts&... args) { println(args...); }
  template<typename T> inline void operator() (const vec<T>& v) { printlnVec(v); }
  template<typename A, typename B> inline void operator() (const pair<A, B>& pair) { println(pair); }
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

void solve(Input& in, Output& out) {
  int n = in.ni();
  in.ni();
  vec<int> first(n, -1), next(2 * n), end(2 * n);
  for (int i = 0; i < n - 1; i++) {
    int a = in.ni() - 1;
    int b = in.ni() - 1;
    end[2 * i] = b;
    next[2 * i] = first[a];
    first[a] = 2 * i;
    end[2 * i + 1] = a;
    next[2 * i + 1] = first[b];
    first[b] = 2 * i + 1;
  }

  vec<int> taken(n, 0);
  vec<int> match(n, -1);
  vec<int> free(n, 0);

  function<void(int, int)> dfs1 = [&] (int u, int parent) {
    taken[u] = -(n + 1);
    int e = first[u];
    while (e != -1) {
      int v = end[e];
      if (v != parent) {
        dfs1(v, u);
        int any = max(free[v], taken[v]);
        free[u] += any;
        int diff = free[v] - any + 1;
        if (diff > taken[u]) {
          match[u] = v;
          taken[u] = diff;
        }
      }
      e = next[e];
    }
    taken[u] += free[u];
  };
  dfs1(0, -1);

  vec<pair<int, int>> result;

  function<void(int, int, int)> dfs2 = [&] (int u, int parent, bool take) {
    if (take)
      result.push_back(make_pair(u + 1, match[u] + 1));
    int e = first[u];
    while (e != -1) {
      int v = end[e];
      if (v != parent)
        dfs2(v, u, take && match[u] == v ? false : (free[v] > taken[v] ? false : true));
      e = next[e];
    }
  };

  dfs2(0, -1, taken[0] > free[0]);
  out(result.size());
  for (auto& pair : result)
    out(pair);
}
