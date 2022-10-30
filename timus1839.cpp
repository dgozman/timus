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

int dot(int x1, int y1, int x2, int y2) {
  return x1 * x2 + y1 * y2;
}

int cross(int x1, int y1, int x2, int y2) {
  return x1 * y2 - x2 * y1;
}

int norm_square(int x, int y) {
  return x * x + y * y;
}

void solve(Input& in, Output& out) {
  int n = in.ni();
  int m = in.ni();
  vec<int> px(n), py(n), qx(n), qy(n), rx(n), ry(n);
  for (int i = 0; i < n; i++) {
    px[i] = in.ni();
    py[i] = in.ni();
    qx[i] = in.ni();
    qy[i] = in.ni();
    rx[i] = in.ni();
    ry[i] = in.ni();
  }
  vec<int> ax(m), ay(m);
  for (int i = 0; i < m; i++) {
    ax[i] = in.ni();
    ay[i] = in.ni();
  }

  vec<int> x(m), y(m), left(m), right(m), left_back(m), tree(2 * m + 1);
  i64 result = 0;
  for (int arc = 0; arc < n; arc++) {
    int count = 0;
    int a = py[arc] - ry[arc];
    int b = rx[arc] - px[arc];
    int c = -(a * px[arc] + b * py[arc]);
    int qv = a * qx[arc] + b * qy[arc] + c;
    i64 pr = norm_square(px[arc] - rx[arc], py[arc] - ry[arc]);
    i64 prab = 4 * pr * norm_square(a, b);

    for (int i = 0; i < m; i++) {
      int iv = a * ax[i] + b * ay[i] + c;
      // Same side as Q.
      if ((qv > 0 && iv >= 0) || (qv < 0 && iv <= 0))
        continue;
      // d2 = iv * iv / (a * a + b * b) < (2*d)^2 = 4 * pr;
      // Less than 2*PR distance.
      if ((i64) iv * (i64) iv >= prab)
        continue;
      // APR is less than pi/2.
      if (dot(ax[i] - px[arc], ay[i] - py[arc], rx[arc] - px[arc], ry[arc] - py[arc]) <= 0)
        continue;
      // ARP is less than pi/2.
      if (dot(ax[i] - rx[arc], ay[i] - ry[arc], px[arc] - rx[arc], py[arc] - ry[arc]) <= 0)
        continue;
      x[count] = ax[i];
      y[count] = ay[i];
      left[count] = count;
      right[count] = count;
      count++;
    }

    auto sort = [&](vec<int>& p, int x1, int y1, int x2, int y2) {
      auto compare = [&] (const int& i, const int& j) -> bool {
        int s1 = cross(x2 - x1, y2 - y1, x[i] - x1, y[i] - y1);
        int s2 = cross(x[i] - x1, y[i] - y1, x[j] - x1, y[j] - y1);
        if (s2 == 0) {
          int d1 = norm_square(x[i] - x1, y[i] - y1);
          int d2 = norm_square(x[j] - x1, y[j] - y1);
          return d1 > d2;
        }
        return (s1 < 0 && s2 > 0) || (s1 > 0 && s2 < 0);
      };

      std::sort(p.begin(), p.begin() + count, compare);
    };

    sort(left, px[arc], py[arc], rx[arc], ry[arc]);
    sort(right, rx[arc], ry[arc], px[arc], py[arc]);
    for (int i = 0; i < count; i++)
      left_back[left[i]] = i;

    int bad = 0;
    for (int i = 0; i <= 2 * count; i++)
      tree[i] = 0;
    for (int i = 0; i < count; i++) {
      int value = left_back[right[i]];

      int l = 1 + (count - 1);
      int r = (value + 1) + (count - 1);
      int one = 0;
      while (l <= r) {
        if (l % 2)
          one += tree[l];
        if (!(r % 2))
          one += tree[r];
        l = (l + 1) / 2;
        r = (r - 1) / 2;
      }
      bad += one;

      r = (value + 1) + (count - 1);
      while (r >= 1) {
        tree[r]++;
        r /= 2;
      }
    }

    result += count * (count - 1) / 2 - bad;
  }

  out(result);
}
