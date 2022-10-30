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
  template<typename T, typename...Ts> inline void print(const T &f, const Ts&... args) {
    *out << f;
    if (sizeof...(args) != 0) { *out << ' '; print(args...); }
  }
  template<typename...Ts> inline void println(const Ts&... args) { print(args...); *out << '\n'; }
  template<typename T> inline void printVec(const vec<T>& v) { for (int i = 0; i < v.size(); i++) { if (i) *out << ' '; print(v[i]); } }
  template<typename T> inline void printlnVec(const vec<T>& v) { printVec(v); *out << '\n'; }
  template<typename...Ts> inline void operator() (const Ts&... args) { println(args...); }
  template<typename T> inline void operator() (const vec<T>& v) { printlnVec(v); }
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

const u64 base1 = 923369;
const u64 base2 = 554269;
const u64 mod1 = 998244353;
const u64 mod2 = 1000000007;

struct Hash {
  u64 h1, h2;
  explicit Hash(): h1(0), h2(0) {}
  explicit Hash(u64 v1, u64 v2): h1(v1), h2(v2) {}
  bool operator==(const Hash& h) const {
    return h1 == h.h1 && h2 == h.h2;
  }
  bool operator<(const Hash& h) const {
    return make_tuple(h1, h2) < make_tuple(h.h1, h.h2);
  }
};

struct hash_fn {
  size_t operator() (const Hash& h) const {
    return std::hash<u64>()(h.h1) ^ std::hash<u64>()(h.h2);
  }
};

struct Pos {
  int r, c;
  Pos(int row, int col): r(row), c(col) {}
};

Hash mod_mul(const Hash& h1, const Hash& h2) {
  return Hash((h1.h1 * h2.h1) % mod1, (h1.h2 * h2.h2) % mod2);
}

Hash mod_val(u64 val) {
  return Hash(val % mod1, val % mod2);
}

Hash mod_add(const Hash& h1, const Hash& h2) {
  return Hash((h1.h1 + h2.h1) % mod1, (h1.h2 + h2.h2) % mod2);
}

Hash mod_sub(const Hash& h1, const Hash& h2) {
  return Hash((h1.h1 + mod1 - h2.h1) % mod1, (h1.h2 + mod2 - h2.h2) % mod2);
}

Hash mod_pow(Hash h, int pow) {
  Hash r(1, 1);
  while (pow > 0) {
    if (pow & 1)
      r = mod_mul(r, h);
    h = mod_mul(h, h);
    pow = pow >> 1;
  }
  return r;
}

unordered_map<Hash, Pos, hash_fn> found;
Hash base(base1, base2);

bool can(int n, int m, const vec2d<Hash>& w, int k, Pos& p1, Pos& p2) {
  found.clear();

  Hash pow_k = mod_pow(base, k);
  Hash pow_k1 = mod_pow(base, k - 1);
  Hash pow_kk1 = mod_pow(base, k * (k - 1));

  vec<Hash> column(m);
  for (int c = 0; c < m; c++) {
    for (int r = 0; r < k; r++)
      column[c] = mod_add(mod_mul(column[c], pow_k), w[r][c]);
  }

  for (int r = 0; r + k <= n; r++) {
    Hash matrix;
    for (int c = 0; c < k; c++)
      matrix = mod_add(mod_mul(matrix, base), column[c]);
    for (int c = 0; c + k <= m; c++) {
      auto it = found.find(matrix);
      if (it == found.end()) {
        found.insert(make_pair(matrix, Pos(r, c)));
      } else {
        p1 = it->second;
        p2 = Pos(r, c);
        return true;
      }
      if (c + k < m) {
        matrix = mod_sub(matrix, mod_mul(column[c], pow_k1));
        matrix = mod_add(mod_mul(matrix, base), column[c + k]);
      }
    }
    if (r + k < n) {
      for (int c = 0; c < m; c++) {
        column[c] = mod_sub(column[c], mod_mul(w[r][c], pow_kk1));
        column[c] = mod_add(mod_mul(column[c], pow_k), w[r + k][c]);
      }
    }
  }
  return false;
}

void solve(Input& in, Output& out) {
  int n = in.ni();
  int m = in.ni();
  // int n = 500;
  // int m = 500;
  vec2d<Hash> w = newVec2d(n, m, Hash());
  for (int i = 0; i < n; i++) {
    string s = in.ns();
    for (int j = 0; j < m; j++)
      w[i][j] = mod_val(s[j] - 'a');
      // w[i][j] = mod_val(i + j);
  }

  // found.reserve(n * m);
  int left = 0;
  int right = min(n, m);
  Pos p1(0, 0), p2(0, 0);
  while (left < right) {
    int k = (left + right + 1) / 2;
    if (can(n, m, w, k, p1, p2))
      left = k;
    else
      right = k - 1;
  }

  if (left > 0) {
    can(n, m, w, left, p1, p2);
    out(left);
    out.print(p1.r + 1);
    out.print(" ");
    out(p1.c + 1);
    out.print(p2.r + 1);
    out.print(" ");
    out(p2.c + 1);
  } else {
    out(0);
  }
}
