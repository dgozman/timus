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
using u16 = uint16_t;
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

struct BitSet {
  int n;
  vec<u64> bits;

  BitSet(int n): n(n), bits((n + 63) / 64) {
  }

  void set(int i) {
    bits[i >> 6] |= ((u64) 1) << (i & 63);
  }

  void unset(int i) {
    bits[i >> 6] &= ~(((u64) 1) << (i & 63));
  }

  int get(int i) {
    return (bits[i >> 6] >> (i & 63)) & 1;
  }

  void shift(int s) {
    int w = s >> 6;
    int b = s & 63;
    if (b > 0) {
      u64 carry = 0;
      for (int i = 0; i < bits.size(); i++) {
        u64 c = bits[i] >> (64 - b);
        bits[i] = (bits[i] << b) | carry;
        carry = c;
      }
    }
    if (w > 0) {
      for (int i = bits.size() - 1; i >= 0; i--)
        bits[i] = i >= w ? bits[i - w] : 0;
    }
  }

  bool operator==(const BitSet& b) const {
    for (size_t i = 0; i < bits.size(); i++) {
      if (bits[i] != b.bits[i])
        return false;
    }
    return true;
  }

  // BitSet operator+(const BitSet& b) const {
  //   BitSet s(n);
  //   for (size_t i = 0; i < bits.size(); i++)
  //     s.bits[i] = bits[i] | b.bits[i];
  //   return s;
  // }

  // BitSet operator-(const BitSet& b) const {
  //   BitSet s(n);
  //   for (size_t i = 0; i < bits.size(); i++)
  //     s.bits[i] = bits[i] & (~b.bits[i]);
  //   return s;
  // }

  BitSet& operator+=(const BitSet& b) {
    for (size_t i = 0; i < bits.size(); i++)
      bits[i] = bits[i] | b.bits[i];
    return *this;
  }

  BitSet& operator-=(const BitSet& b) {
    for (size_t i = 0; i < bits.size(); i++)
      bits[i] = bits[i] & (~b.bits[i]);
    return *this;
  }

  u64 hash() {
    u64 h = 0;
    for (size_t i = 0; i < bits.size(); i++)
      h ^= bits[i] + 0x9e3779b9 + (h << 6) + (h >> 2);
    return h;
  }
};

// distance^2, index
using Entry = pair<i32, u16>;

void combine(int n, BitSet& result, BitSet& a, BitSet& b) {
  int last = -1;
  for (int i = 0; i < n; i++) {
    if (!a.get(i))
      continue;
    b.shift(i - last);
    result += b;
    last = i;
  }
}

BitSet rec(int n, BitSet& from, int size, vec2d<Entry>& e, vec2d<u64>& h) {
  BitSet result(n);

  int u = -1;
  for (int i = 0; i < n; i++) {
    if (from.get(i)) {
      u = i;
      break;
    }
  }
  if (u == -1)
    panic();

  for (int first = size; first >= 1; first--) {
    int outside = e[u][first].first;
    int inside = e[u][first - 1].first;
    u64 uhash = h[u][first - 1];

    bool hashesMatch = true;
    bool allFromTheSet = true;
    for (int j = 1; j < first; j++) {
      int v = e[u][j].second;
      if (!from.get(v)) {
        allFromTheSet = false;
        break;
      }
      u64 vhash = h[v][first - 1];
      if (vhash != uhash) {
        hashesMatch = false;
        break;
      }
      outside = min(outside, e[v][first].first);
      inside = max(inside, e[v][first - 1].first);
      if (inside >= outside)
        break;
    }
    if (!hashesMatch || !allFromTheSet || inside >= outside)
      continue;

    BitSet firstSet(n);
    for (int j = 0; j < first; j++) {
      int v = e[u][j].second;
      firstSet.set(v);
    }
    bool setsMatch = true;
    for (int i = 1; i < first && setsMatch; i++) {
      int v = e[u][i].second;
      for (int j = 0; j < first; j++) {
        int o = e[v][j].second;
        if (!firstSet.get(o)) {
          setsMatch = false;
          break;
        }
      }
    }
    if (!setsMatch)
      continue;

    if (first == size) {
      result.set(0);
      continue;
    }

    from -= firstSet;
    BitSet restResult = rec(n, from, size - first, e, h);
    from += firstSet;

    BitSet firstResult = rec(n, firstSet, first, e, h);
    combine(n, result, restResult, firstResult);
    break;
  }
  return result;
}

void rec2(int n, int i, BitSet& result, int parts, vec<int>& part, vec<int>& x, vec<int>& y) {
  if (i == n) {
    for (int p = 0; p < parts; p++) {
      int inside = 0;
      int outside = 2000000000;
      for (int a = 0; a < n; a++) {
        if (part[a] != p)
          continue;
        for (int b = 0; b < n; b++) {
          int dist = (x[a] - x[b]) * (x[a] - x[b]) + (y[a] - y[b]) * (y[a] - y[b]);
          if (part[b] == p)
            inside = max(inside, dist);
          else
            outside = min(outside, dist);
        }
      }
      if (inside >= outside)
        return;
    }
    result.set(parts - 1);
    return;
  }

  for (int p = 0; p <= parts; p++) {
    part[i] = p;
    rec2(n, i + 1, result, max(parts, p + 1), part, x, y);
  }
}

const bool stress = false;

void solve(Input& in, Output& out) {
  int n = stress ? 7 : in.ni();
  while (n > 0) {
    vec<int> x(n), y(n);
    for (int i = 0; i < n; i++) {
      x[i] = stress ? rand() % 100 : in.ni();
      y[i] = stress ? rand() % 100 : in.ni();
    }

    vec2d<Entry> e(n, vec<Entry>(n + 1));
    vec2d<u64> h(n, vec<u64>(n + 1));
    for (int i = 0; i < n; i++) {
      vec<Entry>& ei = e[i];
      vec<u64>& hi = h[i];
      for (int j = 0; j < n; j++) {
        ei[j].first = (x[i] - x[j]) * (x[i] - x[j]) + (y[i] - y[j]) * (y[i] - y[j]);
        ei[j].second = j;
      }
      ei[n].first = 2000000000;
      ei[n].second = 0;
      sort(ei.begin(), ei.end());
      BitSet b(n);
      for (int j = 0; j < n; j++) {
        b.set(ei[j].second);
        hi[j] = b.hash();
      }
      hi[n] = 0;
    }

    BitSet list(n);
    for (int i = 0; i < n; i++)
      list.set(i);
    BitSet result = rec(n, list, n, e, h);

    if (stress) {
      BitSet result2(n);
      vec<int> part(n);
      rec2(n, 0, result2, 0, part, x, y);
      if (!(result == result2)) {
        out("-----");
        out(n);
        for (int i = 0; i < n; i++) {
          out.print(x[i]);
          out.print(" ");
          out(y[i]);
        }
        out("-----");
        for (int i = 0; i < n; i++)
          out.print(result2.get(i));
        out("");
        out("-----");
        for (int i = 0; i < n; i++)
          out.print(result.get(i));
        out("");
        return;
      }
    }

    for (int i = 0; i < n; i++)
      out.print(result.get(i));
    out("");

    if (!stress)
      n = in.ni();
  }
}

