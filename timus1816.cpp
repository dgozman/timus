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

  vec<int> divisors;
  vec<int> divisors_big;
  for (int i = 2; i * i <= n; i++) {
    if (n % i == 0) {
      int o = n / i;
      divisors.push_back(i);
      if (o > i)
        divisors_big.push_back(o);
    }
  }
  for (int i = divisors_big.size() - 1; i >= 0; i--)
    divisors.push_back(divisors_big[i]);
  divisors.push_back(n);

  vec<int> primes;
  for (int i = 0; i < divisors.size(); i++) {
    bool is_prime = true;
    for (int j = 0; j < i; j++) {
      if ((divisors[i] % divisors[j]) == 0)
        is_prime = false;
    }
    if (is_prime)
      primes.push_back(divisors[i]);
  }

  function<i64(i64, i64, int)> coprimes = [&](i64 m, i64 max, int i) {
    while (i < primes.size() && (m % primes[i]) != 0)
      i++;
    if (i == primes.size())
      return (i64) 0;
    i64 up = max / primes[i];
    return up + coprimes(m, max, i + 1) - coprimes(m, up, i + 1);
  };

  i64 MAX = ((i64) 1000000000) * ((i64) 1000000000);

  // out(divisors);
  vec<double> r(divisors.size());
  for (int i = 0; i < divisors.size(); i++) {
    // out("i", divisors[i]);
    r[i] = 0;
    i64 total_sum = 0;
    for (int j = 0; j < i; j++) {
      if ((divisors[i] % divisors[j]) != 0)
        continue;
      int remainder = divisors[i] / divisors[j];
      int other = std::lower_bound(divisors.begin(), divisors.end(), remainder) - divisors.begin();
      // out("j", divisors[i], divisors[j], remainder, other);
      i64 up = MAX / divisors[j];
      i64 count = up - coprimes(remainder, up, 0);
      total_sum += count;
      r[i] += 1.0 * count * (r[j] + r[other] + 1);
      // out("here we go", divisors[j], count, remainder, MAX/divisors[j], r[j], r[other]);
    }
    if (total_sum > 0) {
      r[i] /= MAX;

      // X = (MAX - total_sum) / MAX * (X + 1) + R
      // X * total_sum / MAX = R + (MAX - total_sum) / MAX
      // p = total_sum / MAX:
      // X * p = R + (1-p)

      double p = 1.0 * total_sum / MAX;
      // out(divisors[i], total_sum, p, r[i], (r[i] + 1 - p) / p);
      r[i] = (r[i] + 1 - p) / p;
    }
  }


  // out("===");
  // out(divisors);
  // out(r);

  out.setPrecision(10);
  out(r[divisors.size() - 1]);
}
