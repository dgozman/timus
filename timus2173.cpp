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

i64 solve(Output&, int, int, vec<int>&);
i64 solve_slow(int, int, vec<int>&);

void stress(Output& out) {
  std::srand(17);
  int counter = 0;
  while (true) {
    int n = 1 + (std::rand() % 5);
    int m = 1 + (std::rand() % 5);
    vec<int> s(n, 0);
    int total = 0;
    for (int i = 0; i < n; i++) {
      s[i] = 1 + (std::rand() % m);
      total += s[i];
    }
    if (total > 2 * m)
      continue;
    i64 res = solve(out, n, m, s);
    i64 res2 = solve_slow(n, m, s);
    if (res != res2) {
      cout << "FAIL " << res << " " << res2 << endl;
      cout << n << " " << m << endl;
      for (int i = 0; i < n; i++)
        cout << s[i] << " ";
      cout << endl;
      return;
    }
    ++counter;
    if (!(counter % 100))
      cout << "OK " << counter << endl;
  }
}

int main() {
  // ifstream fin("input.txt");
  // Input in(fin);
  // ofstream fout("output.txt");
  // Output out(fout);
  Input in(cin);
  Output out(cout);

  // stress(out);
  // return 0;

  int n = in.ni();
  int m = in.ni();
  auto s = in.nvi(n);
  i64 result = solve(out, n, m, s);
  out(result);
  // i64 result2 = solve_slow(n, m, s);
  // out("slow", result2);
  return 0;
}

i64 solve_slow(int n, int m, vec<int>& s) {
  i64 res = 0;
  function<void(int)> rec = [&](int p) {
    if (p == m) {
      for (int i = 0; i < n; i++) {
        if (s[i] > 0)
          return;
      }
      res++;
      return;
    }
    for (int i = 0; i < n; i++) {
      for (int j = i + 1; j < n; j++) {
        if (s[i] > 0 && s[j] > 0) {
          s[i]--;
          s[j]--;
          rec(p + 1);
          s[i]++;
          s[j]++;
        }
      }
      if (s[i] > 0) {
        s[i]--;
        rec(p + 1);
        s[i]++;
      }
    }
    rec(p + 1);
  };
  rec(0);
  return res;
}

i64 solve(Output& out, int n, int m, vec<int>& si) {
  const i64 mod = 1000000007;

  auto c = newVec2d(m + 1, m + 1, (i64)0);
  for (int i = 0; i <= m; i++) {
    c[i][0] = c[i][i] = 1;
    for (int j = 1; j < i; j++)
      c[i][j] = (c[i - 1][j - 1] + c[i - 1][j]) % mod;
  }

  // i, x full, y=s-2*x halves, z=n-x-y zeros
  // a=0..s[i],  a<=y,  s[i]-a<=z
  // i+1, x+a full, s+s[i]-2*x-2*a halves

  auto r = newVec2d(2, m + 1, (i64)0);
  int cur = 0;
  r[cur][0] = 1;

  int sum = 0;
  for (int i = 0; i < n; i++) {
    int next = 1 - cur;
    vec<i64>& p = r[cur];
    vec<i64>& q = r[next];
    memset(q.data(), 0, sizeof(q[0]) * q.size());

    // out("before i", i);
    // out(p);
    // out(q);

    int s = si[i];
    for (int fulls = 0; fulls <= m; fulls++) {
      if (p[fulls] == 0)
        continue;
      int halves = sum - 2 * fulls;
      if (halves < 0 || halves > m - fulls)
        continue;
      int empties = m - fulls - halves;
      // s - add <= empties
      int from = max(0, s - empties);
      // add <= halves
      int to = min(s, halves);
      // out("i", i, "fulls", fulls, "halves", halves, "empties", empties, "from", from, "to", to);
      for (int add = from; add <= to; add++) {
        i64 tmp = p[fulls] * c[halves][add] % mod;
        q[fulls + add] = (q[fulls + add] + tmp * c[empties][s - add]) % mod;
      }
    }
    // out("i", i, "s", s, "sum", sum);
    // out(p);
    // out(q);
    // out("----");

    sum += s;
    cur = next;
  }

  int res = 0;
  for (int fulls = 0; fulls <= m; fulls++)
    res = (res + r[cur][fulls]) % mod;
  return res;
}
