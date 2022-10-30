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

using pnt = pair<double, double>;

double dist(pnt p1, pnt p2) {
  return sqrt((p1.first - p2.first) * (p1.first - p2.first) + (p1.second - p2.second) * (p1.second - p2.second));
}

double area(pnt p1, pnt p2, pnt p3) {
  return abs(p1.first * p2.second + p2.first * p3.second + p3.first * p1.second
    - p1.second * p2.first - p2.second * p3.first - p3.second * p1.first) / 2;
}

double solveOne(pnt p1, pnt p2, pnt q1, pnt q2) {
  auto calc = [&] (double r) {
    pnt p = make_pair(
      p1.first + (p2.first - p1.first) * r,
      p1.second + (p2.second - p1.second) * r
    );
    double s1 = area(p1, p, q1);
    double s2 = area(p, q1, q2);
    double r2 = s1 / s2;
    pnt q = make_pair(
      q1.first + (q2.first - q1.first) * r2,
      q1.second + (q2.second - q1.second) * r2
    );
    return dist(p, q);
  };

  double left = 0;
  double right = 1;
  while (right - left > 1e-9) {
    double left_third = (2 * left + right) / 3;
    double right_third = (left + 2 * right) / 3;
    if (calc(left_third) < calc(right_third))
      right = right_third;
    else
      left = left_third;
  }
  return calc((left + right ) / 2);
}

void solve(Input& in, Output& out) {
  int n = in.ni();
  vec<pnt> p(n * 2);
  for (int i = 0; i < n; i++) {
    auto pnt = make_pair(in.next<double>(), in.next<double>());
    p[i] = p[i + n] = pnt;
  }

  double A = 0;
  for (int i = 1; i + 1 < n; i++)
    A += area(p[0], p[i], p[i + 1]);
  A /= 2;

  int i = 0;
  pnt pi = p[0];
  int j = 1;
  pnt pj = p[1];
  double s = 0;
  while (true) {
    double s3 = area(p[i], p[j], p[j + 1]);
    if (s + s3 >= A) {
      double ratio = (A - s) / s3;
      pj = make_pair(
        p[j].first + ratio * (p[j + 1].first - p[j].first),
        p[j].second + ratio * (p[j + 1].second - p[j].second)
      );
      break;
    }
    s += s3;
    j++;
  }

  double result = 1e100;
  while (i < n) {
    double s1 = area(pi, p[i + 1], pj);
    double s2 = area(p[i + 1], pj, p[j + 1]);
    if (s2 >= s1) {
      double ratio = s1 / s2;
      pnt pjnext = make_pair(
        pj.first + ratio * (p[j + 1].first - pj.first),
        pj.second + ratio * (p[j + 1].second - pj.second)
      );
      result = min(result, solveOne(pi, p[i + 1], pj, pjnext));
      pj = pjnext;
      pi = p[i + 1];
      i++;
    } else {
      double ratio = area(pi, pj, p[j + 1]) / area(pi, p[i + 1], p[j + 1]);
      pnt pinext = make_pair(
        pi.first + ratio * (p[i + 1].first - pi.first),
        pi.second + ratio * (p[i + 1].second - pi.second)
      );
      result = min(result, solveOne(pi, pinext, pj, p[j + 1]));
      pi = pinext;
      pj = p[j + 1];
      j++;
    }
  }

  *out.out << std::setprecision(4) << std::fixed;
  out(result);
}
