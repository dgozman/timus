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

u64 gcd(u64 a, u64 b) {
  return b > 0 ? gcd(b, a % b) : a;
}

void solve(Input& in, Output& out) {
  u64 r1 = in.ni();
  u64 r2 = in.ni();
  int n = in.ni();
  int i = in.ni();
  if (i == 1) {
    out(to_string(r1) + "/" + to_string(1));
    return;
  }
  if (i == (1 << n) + 1) {
    out(to_string(r2) + "/" + to_string(1));
    return;
  }

  u64 g = gcd(r1, r2);
  u64 a = r1 * r2 / g;
  u64 b1 = round(sqrt(a / r1));
  u64 b2 = round(sqrt(a / r2));
  if (b1 * b1 != a / r1 || b2 * b2 != a / r2) {
    out("Irrational");
    return;
  }

  function<bool(int, int, u64&)> get_b = [&] (int n, int i, u64& b) {
    if (n == 0) {
      b = i == 1 ? b1 : b2;
      return true;
    }

    if (i % 2 == 1)
      return get_b(n - 1, (i + 1) / 2, b);
    u64 br;
    if (!get_b(n - 1, i / 2, b))
      return false;
    if (!get_b(n - 1, i / 2 + 1, br))
      return false;
    b = b + br;
    return true;
  };

  u64 b;
  if (!get_b(n, i, b)) {
    out("Irrational");
    return;
  }
  g = gcd(a, b * b);
  a = a / g;
  b = b * b / g;
  out(to_string(a) + "/" + to_string(b));
}
