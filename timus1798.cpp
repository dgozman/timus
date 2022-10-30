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
using uint128_t = __uint128_t;

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

i64 ceil2(i64 P) {
  i64 x = round(sqrt(P * 1.0));
  if (x > 0 && (x - 1) * (x - 1) >= P)
    return x - 1;
  if (x * x >= P)
    return x;
  return x + 1;
}

i64 solve2(i64 R) {
  i64 result = 0;
  for (i64 i = 0; i < R; i++)
    result += ceil2(R * R - i * i);
  return result;
}

void solve(Input& in, Output& out) {
  i64 R = in.ni();

  i64 i = 0;
  i64 result = 0;
  i64 iter = 0;
  while (i < R) {
    ++iter;
    i64 x = ceil2(R * R - i * i);
    i64 x2 = ceil2(R * R - (i + 1) * (i + 1));
    i64 d = x - x2;

    i64 maxi = i;
    auto check = [&] (i64 k) {
      i64 ik = i + k;
      if (ik >= R)
        ik = R - 1;
      if (ik < i)
        ik = i;
      if (ik < maxi)
        return;
      i64 xk = ceil2(R * R - ik * ik);
      if (xk == x - (ik - i) * d)
        maxi = ik;
    };

    auto check2 = [&] (i64 tx) {
      i64 A = 1 + d * d;
      i64 B = 2 * i - 2 * tx * d;
      i64 C = i * i + tx * tx - R * R;
      double D = 1.0 * B * B - 4.0 * A * C;
      if (D < -0.01)
        return;
      if (D < 0)
        D = 0;
      double d = sqrt(D);
      double v1 = round((-B + d) / (2 * A));
      check(v1 - 1);
      check(v1);
      check(v1 + 1);
      double v2 = round((-B - d) / (2 * A));
      check(v2 - 1);
      check(v2);
      check(v2 + 1);
    };

    i64 cnt = maxi - i + 1;
    result += (2 * x - d * (cnt - 1)) * cnt / 2;
    i = maxi + 1;
  }

  out("iter", iter);
  out(result * 4);
  out(solve2(R) * 4);
}
