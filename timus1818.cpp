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

struct Big {
  vec<int> digits;

  Big() {}
  Big(int x) {
    while (x) {
      digits.push_back(x % 10);
      x /= 10;
    }
  }

  Big mul(int x) {
    Big r;
    if (x == 0)
      return r;
    r.digits.reserve(digits.size());
    int c = 0;
    for (int d : digits) {
      c = c + d * x;
      r.digits.push_back(c % 10);
      c /= 10;
    }
    while (c) {
      r.digits.push_back(c % 10);
      c /= 10;
    }
    return r;
  }

  int mod(int x) {
    int r = 0;
    for (int i = digits.size() - 1; i >= 0; i--)
      r = (r * 10 + digits[i]) % x;
    return r;
  }

  Big div(int x) {
    Big r;
    r.digits.reserve(digits.size());
    int c = 0;
    for (int i = digits.size() - 1; i >= 0; i--) {
      c = c * 10 + digits[i];
      int next = c / x;
      if (!r.digits.empty() || next > 0)
        r.digits.push_back(next);
      c = c % x;
    }
    std::reverse(r.digits.begin(), r.digits.end());
    return r;
  }

  Big add(Big o) {
    Big r;
    r.digits.reserve(max(digits.size(), o.digits.size()));
    int c = 0;
    for (int i = 0; i < digits.size() || i < o.digits.size() || c > 0; i++) {
      int x = (i < digits.size() ? digits[i] : 0) + (i < o.digits.size() ? o.digits[i] : 0) + c;
      r.digits.push_back(x % 10);
      c = x / 10;
    }
    return r;
  }

  bool is_zero() {
    return digits.empty();
  }

  void print(Output& out) {
    for (int i = digits.size() - 1; i >= 0; i--)
      out.print(digits[i]);
    if (digits.empty())
      out.print("0");
  }
};

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
  vec<int> a = in.nvi(n);

  // int P = a[n - 1];
  // int N = n;
  Big P(a[n - 1]);
  Big N(n);
  for (int i = n - 2; i >= 0; i--) {
    int K = ((n - 1) - P.mod(n - 1)) % (n - 1);
    P = N.mul(K).add(P).div(n - 1).mul(n).add(Big(a[i]));
    N = N.mul(n);
    // int K = ((n - 1) - P % (n - 1)) % (n - 1);
    // P = (N * K + P) / (n - 1) * n + a[i];
    // N = N * n;
  }
  if (P.is_zero())
    N.print(out);
  else
    P.print(out);
  out.println();
}
