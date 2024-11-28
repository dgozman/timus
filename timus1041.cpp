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
  const i64 mod = 999989899;

  auto inverse = [&] (i64 a) {
    i64 pow = mod - 2;
    i64 res = 1;
    while (pow) {
      if (pow % 2)
        res = (res * a) % mod;
      a = (a * a) % mod;
      pow /= 2;
    }
    return res;
  };

  int m = in.ni();
  int n = in.ni();
  auto a = newVec2d(m, n, (i64)0);
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++)
      a[i][j] = in.ni();
  }
  auto cost = in.nvi(m);

  vec<int> p(m);
  for (int i = 0; i < m; i++)
    p[i] = i;
  sort(p.begin(), p.end(), [&] (int a, int b) {
    if (cost[a] != cost[b])
      return cost[a] < cost[b];
    return a < b;
  });

  auto d = newVec2d(n, n, (i64)0);
  vec<i64> invs(n);
  vec<int> cols(n);
  for (int i = 0; i < n; i++)
    cols[i] = i;
  int r = 0;
  vec<int> result;
  int total = 0;
  for (int index = 0; index < m && r < n; index++) {
    int v = p[index];
    for (int j = 0; j < n; j++)
      d[r][j] = a[v][cols[j]];
    // out("try", r, "d", d);
    // out("===");

    for (int i = 0; i < r; i++) {
      i64 t = (d[r][i] * invs[i]) % mod;
      t = mod - t;
      for (int j = 0; j < n; j++)
        d[r][j] = (d[r][j] + d[i][j] * t) % mod;
    }
    for (int j = 0; j < r; j++) {
      if (d[r][j] != 0) {
        // cout << "HERE!" << endl;
        // out(d);
        panic();
      }
    }

    int col = -1;
    for (int j = r; j < n; j++) {
      if (d[r][j] != 0) {
        col = j;
        break;
      }
    }
    if (col == -1)
      continue;

    int tmp = cols[r];
    cols[r] = cols[col];
    cols[col] = tmp;
    for (int i = 0; i <= r; i++) {
      i64 t = d[i][r];
      d[i][r] = d[i][col];
      d[i][col] = t;
    }
    invs[r] = inverse(d[r][r]);
    r++;
    result.push_back(v);
    total += cost[v];

    // out("r", r, "d", d);
    // out("===");
  }

  if (r < n) {
    out("0");
    return;
  }

  sort(result.begin(), result.end());
  out(total);
  for (int i = 0; i < n; i++)
    out(result[i] + 1);
}
