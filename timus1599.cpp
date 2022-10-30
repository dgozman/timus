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

void solve(Input& in, Output& out) {
  int n = in.ni();
  vec<int> x(n + 1);
  vec<int> y(n + 1);
  for (int i = 0; i < n; i++) {
    x[i] = in.ni();
    y[i] = in.ni();
  }
  x[n] = x[0];
  y[n] = y[0];
  int m = in.ni();
  for (int j = 0; j < m; j++) {
    int cx = in.ni();
    int cy = in.ni();
    int counter = 0;
    bool edge = false;
    for (int i = 0; i < n; i++) {
      int x1 = x[i] - cx;
      int y1 = y[i] - cy;
      int x2 = x[i + 1] - cx;
      int y2 = y[i + 1] - cy;

      if (y1 == 0 && y2 == 0) {
        if ((x1 <= 0 && x2 >= 0) || (x1 >= 0 && x2 <= 0)) {
          edge = true;
        } else {
          // horizontal - no changes
        }
      } else if (y1 != y2) {
        if ((y1 > 0 && y2 > 0) || (y1 < 0 && y2 < 0)) {
          // does not touch Ox+ - no changes
        } else {
          i64 v = (i64) x1 * (y1 - y2) + (i64) y1 * (x2 - x1);
          if (v == 0) {
            edge = true;
          } else {
            bool left = (y1 > y2 && v < 0) || (y1 < y2 && v > 0);
            if (left) {
              // does not touch Ox+ - no changes
            } else {
              if (y1 > 0 && y2 <= 0)
                counter--;
              else if (y1 <= 0 && y2 > 0)
                counter++;
            }
          }
        }
      } else {
        // horizontal - no changes
      }
    }
    if (edge)
      out("EDGE");
    else
      out(counter);
  }
}
