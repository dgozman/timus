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

void solve(Input& in, Output& out) {
  int n = in.ni();
  int m = in.ni();
  int a = in.ni();
  int b = in.ni();

  int rl = in.ni() - 1;
  int s = in.ni();

  vec<int> mx(m + 1, n);
  vec<int> mn(m + 1, 0);
  mn[m] = n;
  mx[0] = 0;
  for (int i = 0; i < s; i++) {
    int r = in.ni() - 1;
    int f = in.ni() - 1;
    mx[f] = min(mx[f], r);
    mn[f + 1] = max(mn[f + 1], r + 1);
  }

  // out(mn);
  // out(mx);

  for (int i = m - 1; i >= 0; i--)
    mx[i] = min(mx[i], mx[i + 1] - a);
  for (int i = 1; i <= m; i++)
    mx[i] = min(mx[i], mx[i - 1] + b);
  for (int i = m - 1; i >= 0; i--)
    mx[i] = min(mx[i], mx[i + 1] - a);

  for (int i = 1; i <= m; i++)
    mn[i] = max(mn[i], mn[i - 1] + a);
  for (int i = m - 1; i >= 0; i--)
    mn[i] = max(mn[i], mn[i + 1] - b);
  for (int i = 1; i <= m; i++)
    mn[i] = max(mn[i], mn[i - 1] + a);

  // out(mn);
  // out(mx);

  bool impossible = mn[0] > 0 || mx[m] < n;
  vec<int> result;
  for (int i = 0; i <= m; i++) {
    impossible |= mn[i] > mx[i];
    if (i < m && mx[i + 1] > rl && mn[i] <= rl)
      result.push_back(i + 1);
  }
  if (impossible) {
    out("IMPOSSIBLE");
  } else {
    out(result.size());
    out(result);
  }
}

/*

16 4 4 4
3
1
8 1

*/