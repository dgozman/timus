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
  int k = in.ni();

  auto expect = newVec2d<double>(k + 1, 2, 0.0);
  auto probability = newVec2d<double>(k + 1, 2, 0.0);
  probability[0][0] = 1.0;
  probability[0][1] = 0.0;

  double p_same = 1.0;
  double p_diff = 1.0;

  for (int i = 1; i <= k; i++) {
    int count = 0;
    int first = i == k ? 1 : 0;
    for (int x = first; x < 10; x++) {
      for (int y = first; y < 10; y++) {
        count++;
        for (int c = 0; c < 2; c++) {
          int s = x + y + c;
          int nc = s >= 10 ? 1 : 0;
          probability[i][nc] += probability[i - 1][c];
          if (x <= 1 || y <= 1) {
            expect[i][nc] += (expect[i - 1][c] + 1 + c + nc + 1) * probability[i - 1][c];
          } else {
            double p = (x == y) ? p_same : p_diff;
            expect[i][nc] += (expect[i - 1][c] + 1 + p + c + nc + 1) * probability[i - 1][c];
          }
        }
      }
    }
    out("=====", i);
    out(probability[i]);
    out(count);
    probability[i][0] /= count;
    probability[i][1] /= count;
    expect[i][0] /= count;
    expect[i][1] /= count;
    p_same *= 0.99;
    p_diff *= 0.98;
  }

  out(probability[0]);
  out(expect[0]);

  out(probability[1]);
  out(expect[1]);

  out.setPrecision(10);
  out(expect[k][0] * probability[k][0] + (expect[k][1] + 1) * probability[k][1]);
}
