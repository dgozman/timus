#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <cstring>
#include <cmath>
#include <fstream>
#include <functional>
#include <numeric>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <memory>
#include <queue>
#include <random>

using namespace std;
bool __hack1 = std::ios::sync_with_stdio(false);
auto __hack2 = cin.tie(nullptr);
template<typename T> using vec = vector<T>;
template<typename T> using matrix = vec<vec<T>>;
using i64 = int64_t;

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

void solve(Input& in, Output& out);
int main() {
  // ifstream fin("input.txt");
  // Input in(fin);
  // ofstream fout("output.txt");
  // Output out(fout);
  Input in(cin);
  Output out(cout);
  solve(in, out);
}

const int n = 8;

double sqrt2(double x) {
  return x < 0 ? 0 : sqrt(x);
}

double dist2(vec<double> p1, vec<double> p2) {
  double d = 0;
  for (int i = 0; i < n; i++)
    d += (p1[i] - p2[i]) * (p1[i] - p2[i]);
  return d;
}

double dist(vec<double> p1, vec<double> p2) {
  return sqrt2(dist2(p1, p2));
}

double acos2(double x) {
  if (x < -1)
    return acos(-1);
  if (x > 1)
    return acos(1);
  return acos(x);
}

void solve(Input& in, Output& out) {
  auto a = in.nvec<double>(n);
  auto b = in.nvec<double>(n);
  auto c = in.nvec<double>(n);
  auto r = in.next<double>();

  double da = dist(c, a);
  double db = dist(c, b);
  double d = dist(a, b);

  double p = (da + db + d) / 2;
  double s = sqrt2(p * (p - da) * (p - db) * (p - d));
  double h = 2 * s / d;

  *out.out << std::setprecision(2) << std::fixed;
  if (h > r || sqrt2(dist2(a, c) - h * h) > d || sqrt2(dist2(b, c) - h * h) > d || d < 1e-10) {
    out(d);
  } else {
    double ha = sqrt2(da * da - r * r);
    double hb = sqrt2(db * db - r * r);
    double ang = acos2((dist2(a, c) + dist2(b, c) - dist2(a, b)) / (2 * da * db));
    ang -= acos2(r / da);
    ang -= acos2(r / db);
    double result = ha + hb + r * ang;
    out(result);
  }
}
