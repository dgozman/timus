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
  string top = in.ns();
  string bottom = in.ns();

  vec<int> balance(2 * n + 1);
  balance[0] = 0;
  for (int i = 0; i < n; i++)
    balance[i + 1] = balance[i] + (top[i] == 'A' ? 1 : -1);
  for (int i = 0; i < n; i++)
    balance[n + i + 1] = balance[n + i] + (bottom[i] == 'A' ? 1 : -1);

  const int ALICE = 0;
  const int BOB = 1;
  const int DRAW = 2;
  map<pair<int, int>, int> cache_alice;
  map<pair<int, int>, int> cache_bob;

  function<int(int, int, int)> rec = [&](int from, int to, int who) {
    auto p = make_pair(from, to);

    map<pair<int, int>, int>& cache = who == ALICE ? cache_alice : cache_bob;
    auto it = cache.find(p);
    if (it != cache.end())
      return it->second;

    int result = DRAW;
    int len = (to - from) / 2;
    int mid = (from + to) / 2;
    if (len % 2 == 0) {
      result = 1 - who;

      int delta1 = balance[mid] - balance[from];
      int outcome1 = delta1 == len ? ALICE : (delta1 == -len ? BOB : DRAW);
      int result1 = outcome1 == who ? who : rec(from, mid, 1 - who);
      if (result1 == who)
        result = who;
      else if (result1 == DRAW && result != who)
        result = DRAW;

      int delta2 = balance[to] - balance[mid];
      int outcome2 = delta2 == len ? ALICE : (delta2 == -len ? BOB : DRAW);
      int result2 = outcome2 == who ? who : rec(mid, to, 1 - who);
      if (result2 == who)
        result = who;
      else if (result2 == DRAW && result != who)
        result = DRAW;
    }

    cache[p] = result;
    return result;
  };

  int winner = rec(0, 2 * n, ALICE);
  // if (balance[2 * n] == 2 * n)
  //   winner = ALICE;
  // else if (balance[2 * n] == -2 * n)
  //   winner = BOB;
  out(winner == ALICE ? "Alice" : (winner == BOB ? "Bob" : "Draw"));
}
