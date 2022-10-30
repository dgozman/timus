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
  template<typename T> inline void printVec(const vec<T>& v) { for (int i = 0; i < v.size(); i++) { if (i) *out << ' '; print(v[i]); } }
  template<typename T> inline void printlnVec(const vec<T>& v) { printVec(v); *out << '\n'; }
  template<typename...Ts> inline void operator() (const Ts&... args) { println(args...); }
  template<typename T> inline void operator() (const vec<T>& v) { printlnVec(v); }
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
  int m = in.ni();
  int k = n + m;

  vec<int> first(k, -1);
  vec<int> next, end, w;

  auto add_edge = [&] (int a, int b, int weight) {
    w.push_back(weight);
    next.push_back(first[a]);
    first[a] = end.size();
    end.push_back(b);
  };

  for (int i = 0; i < n; i++) {
    int c = in.ni();
    for (int j = 0; j < c; j++) {
      int x = in.ni() - 1;
      add_edge(x + n, i, 1);
      add_edge(i, x + n, 0);
    }
  }

  int start = in.ni() - 1 + n;
  int finish = in.ni() - 1 + n;

  vec<int> d(k, k + 1);
  vec<int> from(k, -1);
  auto compare = [&] (const int& u, const int& v) -> bool {
    return d[u] != d[v] ? d[u] < d[v] : u < v;
  };
  set<int, decltype(compare)> queue(compare);

  d[start] = 0;
  queue.insert(start);
  while (queue.size() > 0) {
    auto it = queue.begin();
    int u = *it;
    queue.erase(it);
    int e = first[u];
    while (e != -1) {
      int v = end[e];
      if (d[v] > d[u] + w[e]) {
        queue.erase(v);
        d[v] = d[u] + w[e];
        from[v] = u;
        queue.insert(v);
      }
      e = next[e];
    }
  }

  if (d[finish] == k + 1) {
    out(-1);
  } else {
    out(d[finish]);
    vec<int> result;
    while (finish != start) {
      if (finish >= n)
        result.push_back(finish - n + 1);
      finish = from[finish];
    }
    result.push_back(start - n + 1);
    std::reverse(result.begin(), result.end());
    out(result);
  }
}
