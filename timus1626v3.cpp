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

int dot(int x1, int y1, int x2, int y2) {
  return x1 * y2 - x2 * y1;
}

void solve(Input& in, Output& out) {
  int n = in.ni();
  vec<int> x(n * 2), y(n * 2);
  for (int i = 0; i < n; i++) {
    x[i] = in.ni();
    y[i] = in.ni();
  }
  int xs = in.ni();
  int ys = in.ni();
  int xt = in.ni();
  int yt = in.ni();

  auto area = [&] () {
    int a = 0;
    for (int i = 0; i < n; i++)
      a += x[i] * y[i + 1] - x[i + 1] * y[i];
    return a;
  };

  bool reversed = area() < 0;
  if (reversed) {
    for (int i = 0; i < n - 1 - i; i++) {
      int tmp = x[i]; x[i] = x[n - 1 - i]; x[n - 1 - i] = tmp;
      tmp = y[i]; y[i] = y[n - 1 - i]; y[n - 1 - i] = tmp;
    }
  }

  for (int i = 0; i < n; i++) {
    x[n + i] = x[i];
    y[n + i] = y[i];
  }

  int pow2 = 64;

  int m = (n + pow2 - 1) / pow2;
  u64 one = 1;
  vec2d<u64> to_right = newVec2d(n, m, (u64) 0);
  vec2d<u64> in_right = newVec2d(n, m, (u64) 0);

  for (int i = 0; i < n; i++) {
    int last = i;
    int circles = 0;
    for (int j = i + 1; j < i + n; j++) {
      bool candidate = false;
      if (j == i + 1) {
        last = j;
        circles = 0;
        candidate = true;
      } else {
        int prev = dot(x[last] - x[i], y[last] - y[i], x[j - 1] - x[i], y[j - 1] - y[i]);
        int next = dot(x[last] - x[i], y[last] - y[i], x[j] - x[i], y[j] - y[i]);
        int rel = dot(x[j - 1] - x[i], y[j - 1] - y[i], x[j] - x[i], y[j] - y[i]);
        if (prev >= 0) {
          if (next > 0) {
            // Nothing to do.
          } else {
            // next <= 0
            if (rel < 0)
              circles++;
          }
        } else {
          // prev < 0
          if (next < 0) {
            // Nothing to do.
          } else {
            // next >= 0
            if (rel > 0)
              circles--;
          }
        }
        if (circles == 0 && next > 0) {
          last = j;
          candidate = true;
        }
      }
      if (candidate) out(i, j % n);
      if (candidate &&
          dot(x[j] - x[i], y[j] - y[i], xs - x[i], ys - y[i]) > 0 &&
          dot(x[j] - x[i], y[j] - y[i], xt - x[i], yt - y[i]) > 0) {
        int j_proper = j % n;
        to_right[i][j_proper / pow2] |= one << (j_proper % pow2);
        in_right[j_proper][i / pow2] |= one << (i % pow2);
      }
    }
  }

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      u64 bit = (to_right[i][j / pow2] >> (j % pow2)) & 1;
      if (!bit)
        continue;
      for (int chunk = 0; chunk < m; chunk++) {
        u64 both = to_right[j][chunk] & in_right[i][chunk];
        if (!both)
          continue;

        for (int k_rem = 0; k_rem < pow2; k_rem++) {
          if ((both >> k_rem) & 1) {
            int k = chunk * pow2 + k_rem;

            if (reversed) {
              i = n - 1 - i;
              j = n - 1 - j;
              k = n - 1 - k;
            }
            // out(i + 1, j + 1, k + 1);
            out(i, j, k);
            return;
          }
        }
        panic();
      }
    }
  }

  out("Impossible");
}
