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
template<typename T> using matrix = vec<vec<T>>;
using i64 = int64_t;
using u64 = uint64_t;

struct Input {
  Input(istream &in) : in(&in) {}
  istream *in;
  template<class T> T next() const { T x; *in >> x; return x; }
  int ni() const { return next<int>(); }
  i64 ni64() const { return next<i64>(); }
  template<class T> vec<T> nvec(int n) const { vec<T> v(n); for (int i = 0; i < n; ++i) v[i] = next<T>(); return v; }
  vec<int> nvi(int n) const { return nvec<int>(n); }
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

template<typename T> matrix<T> newmatrix(int n, int m, const T& init) {
  matrix<T> v(n, vec<T>(m, init));
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

const double eps = 1e-10;

double sqrt2(double x) {
  return x < 0 ? 0 : sqrt(x);
}

double calc(double a1, double a2, double a3, double b1, double b2, double b3) {
  double cosa = (a1 * a1 + a2 * a2 - a3 * a3) / (2 * a1 * a2);
  double sina = sqrt2(1 - cosa * cosa);
  double cosb = (b1 * b1 + b2 * b2 - b3 * b3) / (2 * b1 * b2);
  double sinb = sqrt2(1 - cosb * cosb);
  if (cosa < cosb)
    return 0;
  return b2 * (sinb * cosa / sina - cosb);
}

void solve(Input& in, Output& out) {
  vec<double> a = in.nvec<double>(3);
  vec<double> b = in.nvec<double>(3);
  bool can = false;
  for (int i = 0; i < 3; i++) {
    double a1 = a[i];
    double a2 = a[(i + 1) % 3];
    double a3 = a[(i + 2) % 3];
    for (int j = 0; j < 3; j++) {
      for (int k = 1; k <= 2; k++) {
        double b1 = b[j];
        double b2 = b[(j + k) % 3];
        double b3 = b[(j + 2 * k) % 3];

        double left = calc(a1, a2, a3, b1, b2, b3);
        double right = calc(a1, a3, a2, b1, b3, b2);
        if (left + right + b1 < a1 + eps)
          can = true;
      }
    }
  }
  out(can ? "YES" : "NO");
}
