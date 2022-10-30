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
using u128 = __uint128_t;

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
  template<typename A, typename B> inline void printOne(const pair<A, B>& pair) { print(pair.first, pair.second); }
  template<typename T, typename...Ts> inline void print(const T &f, const Ts&... args) {
    *out << f;
    if (sizeof...(args) != 0) { *out << ' '; print(args...); }
  }
  template<typename...Ts> inline void println(const Ts&... args) { print(args...); *out << '\n'; }
  template<typename...Ts> inline void operator() (const Ts&... args) { println(args...); }
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
  vec<u128> bits(128);
  vec<u128> mask(128);
  bits[0] = 1;
  for (int i = 1; i < 128; i++)
    bits[i] = bits[i - 1] * 2;
  mask[127] = bits[127];
  for (int i = 126; i >= 0; i--)
    mask[i] = mask[i + 1] + bits[i];

  vec<u128> tens(39);
  tens[0] = 1;
  for (int i = 1; i < 39; i++)
    tens[i] = tens[i - 1] * 10;

  // while (true) {
  int n = in.ni();
  // int n = rand() % 100;
  vec<u128> a(n);
  for (int i = 0; i < n; i++) {
    u128 x = 0;
    for (int j = 0; j < 4; j++) {
      u32 z = in.next<u32>();
      // int z = rand() % (1 << 30);
      x = x * bits[32] + z;
    }
    a[i] = x;
  }
  sort(a.begin(), a.end());

  auto slow = [&] () {
    int result = 0;
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        u128 x = a[i] ^ a[j];
        for (int k = 38; k >= 0; k--) {
          if (x >= tens[k]) {
            result += k;
            break;
          }
        }
      }
    }
    return result;
  };

  vec<u128> temp(n);

  auto count = [&] (u128 num) {
    int res = 0;
    for (int i = 127; i >= -1; i--) {
      u128 up = num;
      u128 mk = mask[0];
      if (i >= 0) {
        if ((num & bits[i]) > 0)
          continue;
        up = (num & mask[i]) | bits[i];
        mk = mask[i];
      }
      for (int j = 0; j < n; j++)
        temp[j] = (a[j] & mk) ^ up;
      sort(temp.begin(), temp.end());
      int j1 = 0;
      int k1 = 0;
      while (j1 < n) {
        u128 cur = a[j1] & mk;
        int j2 = j1;
        while (j2 < n && (a[j2] & mk) == cur)
          j2++;
        while (k1 < n && temp[k1] < cur)
          k1++;
        if (k1 < n && temp[k1] == cur) {
          int k2 = k1;
          while (k2 < n && temp[k2] == cur)
            k2++;
          res += (j2 - j1) * (k2 - k1);
          k1 = k2;
        }
        j1 = j2;
      }
    }
    return res;
  };

  int result = 0;
  int last = 0;
  for (int i = 38; i >= 0; i--) {
    int current = count(tens[i]);
    result += (current - last) * i;
    last = current;
  }
  // if (result != slow()) {
  //   out("====");
  //   out(n);
  //   for (int i = 0; i < n; i++)
  //     out((u64) a[i]);
  //   out(result);
  //   out(slow());
  //   break;
  // }

  out(result);

  // }
}
