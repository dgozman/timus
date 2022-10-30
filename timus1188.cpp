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

template<typename T> bool rmn(T& value, T candidate) {
  if (candidate < value) {
    value = candidate;
    return true;
  }
  return false;
}

void solve(Input& in, Output& out);
int main() {
  //  ifstream fin("input.txt");
  //  Input in(fin);
  // ofstream fout("output.txt");
  // Output out(fout);
  Input in(cin);
  Output out(cout);
  solve(in, out);
  return 0;
}

// pegs, cuts
using Ans = pair<int, int>;

void solve(Input& in, Output& out) {
  int xn = in.ni();
  int yn = in.ni();
  int xt = in.ni();
  int yt = in.ni();
  int n = in.ni();
  vec<int> x(n), y(n), x1(n), x2(n), l(n);
  for (int i = 0; i < n; i++) {
    y[i] = in.ni();
    x[i] = in.ni();
    l[i] = in.ni();
    x1[i] = in.ni();
    x2[i] = in.ni();
  }

  Ans ans(2 * n + 1, 0);
  for (int base = 0; base < n; base++) {
    if (y[base] + yt > yn || l[base] < xt)
      continue;
    for (int left = 0; left <= xn - xt; left++) {
      int leftMostMovingPeg = x[base] + x1[base] - l[base];
      int leftMostNotMovingPeg = max(x[base] + x2[base] - l[base], x[base] + x1[base] - (l[base] >> 1));
      int rightMostMovingPeg = x[base] + x2[base] + l[base];
      int rightMostNotMovingPeg = min(x[base] + x1[base] + l[base], x[base] + x2[base] + (l[base] >> 1));

      if (left < leftMostMovingPeg || left + xt > rightMostMovingPeg) {
        // Have to move two pegs - this shelf does not work.
        continue;
      }

      Ans baseAns(0, 0);
      if (left < leftMostNotMovingPeg) {
        // Must move the right peg and move the shelf to the left.
        baseAns = make_pair(1, 0);
      } else if (left + xt > rightMostNotMovingPeg) {
        // Must move the left peg and move the shelf to the right.
        baseAns = make_pair(1, 0);
      } else {
        // Nothing to do.
      }

      for (int i = 0; i < n; i++) {
        if (y[i] <= y[base] || y[i] >= y[base] + yt)
          continue;

        // Remove the shelf.
        Ans iAns(2, l[i]);

        // Cut and move the shelf to the right.
        if (x[i] + x1[i] >= left + xt) {
          int len = min(l[i], min(xn - left - xt, (x[i] + x2[i] - left - xt) * 2));
          int cut = l[i] - len;
          rmn(iAns, make_pair(0, cut));
        }

        // Cut and move the shelf to the left.
        if (x[i] + x2[i] <= left) {
          int len = min(l[i], min(left, (left - x[i] - x1[i]) * 2));
          int cut = l[i] - len;
          rmn(iAns, make_pair(0, cut));
        }

        // Move the left peg, cut and move the shelf to the right.
        if (x[i] + x2[i] >= left + xt) {
          int len = min(l[i], xn - left - xt);
          int cut = l[i] - len;
          if (len > 0)
            rmn(iAns, make_pair(1, cut));
        }

        // Move the right peg, cut and move the shelf to the left.
        if (x[i] + x1[i] <= left) {
          int len = min(l[i], left);
          int cut = l[i] - len;
          if (len > 0)
            rmn(iAns, make_pair(1, cut));
        }

        baseAns.first += iAns.first;
        baseAns.second += iAns.second;
      }

      rmn(ans, baseAns);
    }
  }

  if (ans.first > 2 * n - 2)
    panic();
  out.print(ans.first);
  out.print(" ");
  out.print(ans.second);
}
