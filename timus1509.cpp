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

using Point = pair<double, double>;
const double eps = 1e-4;
const double inf = 1e10;

double matches(vec<Point>& v1, vec<Point>& v2) {
  if (v1.size() != v2.size())
    return inf;
  double res = inf;
  for (int m0 = 0; m0 < v2.size(); m0++) {
    for (int m1 = 0; m1 < v2.size(); m1++) {
      if (m0 == m1)
        continue;
      double x1 = v1[1].first - v1[0].first;
      double y1 = v1[1].second - v1[0].second;
      double x2 = v2[m1].first - v2[m0].first;
      double y2 = v2[m1].second - v2[m0].second;
      double c = (x1 * x2 + y1 * y2) / (x1 * x1 + y1 * y1);
      double s = (x1 * y2 - x2 * y1) / (x1 * x1 + y1 * y1);

      double scale = c * c + s * s;
      if (scale > 1 + eps || scale < 0.01 * 0.01 - eps)
        continue;

      double worst = 0;
      vec<int> matched(v1.size(), 0);
      for (int i = 0; i < v1.size(); i++) {
        double xi = v1[i].first - v1[0].first;
        double yi = v1[i].second - v1[0].second;
        double best = inf;
        int bestj = -1;
        for (int j = 0; j < v1.size(); j++) {
          if (matched[j] == 1)
            continue;
          double xj = v2[j].first - v2[m0].first;
          double yj = v2[j].second - v2[m0].second;
          double rx = xi * c - yi * s;
          double ry = xi * s + yi * c;
          double dist = (xj - rx) * (xj - rx) + (yj - ry) * (yj - ry);
          if (dist < best) {
            best = dist;
            bestj = j;
          }
        }
        worst = max(worst, best);
        matched[bestj] = 1;
      }
      res = min(res, worst);
    }
  }
  return res;
}

void solve(Input& in, Output& out) {
  vec<vec<Point>> single(7);

  single[1].push_back(make_pair(50, 50));

  single[2].push_back(make_pair(25, 25));
  single[2].push_back(make_pair(75, 75));

  single[3].push_back(make_pair(25, 25));
  single[3].push_back(make_pair(75, 75));
  single[3].push_back(make_pair(50, 50));

  single[4].push_back(make_pair(25, 25));
  single[4].push_back(make_pair(25, 75));
  single[4].push_back(make_pair(75, 75));
  single[4].push_back(make_pair(75, 25));

  single[5].push_back(make_pair(25, 25));
  single[5].push_back(make_pair(25, 75));
  single[5].push_back(make_pair(75, 75));
  single[5].push_back(make_pair(75, 25));
  single[5].push_back(make_pair(50, 50));

  single[6].push_back(make_pair(25, 25));
  single[6].push_back(make_pair(25, 75));
  single[6].push_back(make_pair(75, 75));
  single[6].push_back(make_pair(75, 25));
  single[6].push_back(make_pair(25, 50));
  single[6].push_back(make_pair(75, 50));

  int n = in.ni();
  vec<Point> ps(n);
  for (int i = 0; i < n; i++)
    ps[i] = make_pair(in.next<double>(), in.next<double>());

  if (n == 1) {
    out("0 1");
    return;
  }

  if (n == 2) {
    double dist = sqrt(
      (ps[1].first - ps[0].first) * (ps[1].first - ps[0].first) +
      (ps[1].second - ps[0].second) * (ps[1].second - ps[0].second)
    );
    if (dist > 0.5 * sqrt(2) - eps && dist < 50 * sqrt(2) + eps)
      out("0 2");
    if (dist > 1 - eps && dist < 100 + eps)
      out("1 1");
    return;
  }

  double best = inf;
  int ra, rb;
  for (int a = 0; a <= 6; a++) {
    for (int b = 0; b <= 6; b++) {
      vec<Point> all;
      for (int i = 0; i < single[a].size(); i++) {
        Point p = single[a][i];
        p.first = 100 - p.first;
        all.push_back(p);
      }
      for (int i = 0; i < single[b].size(); i++) {
        Point p = single[b][i];
        p.first = 100 - p.first;
        p.second += 100;
        all.push_back(p);
      }
      double match = matches(all, ps);
      if (match < best) {
        best = match;
        ra = a;
        rb = b;
      }
    }
  }

  out.print(min(ra, rb));
  out.print(" ");
  out.println(max(ra, rb));
}
