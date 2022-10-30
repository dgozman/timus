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

using D = int;

int inv(int x) {
  if (x == 0)
    panic();
  return x;
}

// Returns the number of non-free variables. These are first in the w and perm vectors.
int gauss(int m, vec2d<D>& w, vec<int>& perm) {
  perm.resize(m);
  for (int i = 0; i < m; i++)
    perm[i] = i;

  int i = 0;
  while (i < m && i < w.size()) {
    int maxr = i;
    int maxc = i;
    for (int r = i; r < w.size(); r++) {
      for (int c = i; c < m; c++) {
        if (abs(w[r][c]) > abs(w[maxr][maxc])) {
          maxr = r;
          maxc = c;
        }
      }
    }
    if (abs(w[maxr][maxc]) == 0)
      break;
    for (int j = 0; j < w[i].size(); j++) {
      D tmp = w[i][j];
      w[i][j] = w[maxr][j];
      w[maxr][j] = tmp;
    }
    int tperm = perm[i];
    perm[i] = perm[maxc];
    perm[maxc] = tperm;
    for (int j = 0; j < w.size(); j++) {
      D tmp = w[j][i];
      w[j][i] = w[j][maxc];
      w[j][maxc] = tmp;
    }

    for (int j = 0; j < w.size(); j++) {
      if (j == i)
        continue;
      D t = w[j][i] * inv(w[i][i]);
      for (int k = 0; k < w[j].size(); k++) {
        w[j][k] = (w[j][k] - w[i][k] * t) % 3;
        w[j][k] = (w[j][k] + 3) % 3;
      }
    }

    i++;
  }

  return i;
}

void solve(Input& in, Output& out) {
  int k = in.ni();
  int n = in.ni();
  vec2d<int> w = newVec2d(k, n + k, 0);
  vec<int> b(k);
  for (int i = 0; i < k; i++) {
    for (int j = 0; j < n; j++)
      w[i][j] = in.ni();
    b[i] = in.ni();
    w[i][n + i] = 1;
  }

  vec<int> perm;
  int bound = gauss(n, w, perm);

  auto apply = [&] (int mask, int offset, int count, bool negate) {
    u64 value = 0;
    for (int j = bound; j < k; j++) {
      int v = 0;
      for (int i = 0; i < count; i++) {
        int bit = (mask >> i) & 1;
        if (b[offset + i] == 0)
          bit++;
        else if (b[offset + i] == 1)
          bit = bit ? bit + 1 : bit;
        v = v + w[j][n + offset + i] * bit;
      }
      v = v % 3;
      if (negate)
        v = (3 - v) % 3;
      value = 3 * value + v;
    }
    return value;
  };

  int first = k / 2;
  int second = k - first;
  vec<u64> values;
  for (int mask = 0; mask < (1 << first); mask++) {
    u64 value = apply(mask, 0, first, true);
    values.push_back(value);
  }
  sort(values.begin(), values.end());

  u64 result = 0;
  for (int mask = 0; mask < (1 << second); mask++) {
    u64 value = apply(mask, first, second, false);
    auto low = std::lower_bound(values.begin(), values.end(), value);
    if (low == values.end() || *low != value)
      continue;
    auto high = std::upper_bound(values.begin(), values.end(), value);
    result += (high - low);
  }

  for (int i = 0; i < n - bound; i++)
    result = result * 3;
  out(result);
}
