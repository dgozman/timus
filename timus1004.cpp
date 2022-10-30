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

struct Input {
  Input(istream &in) : in(&in) {}
  istream *in;
  template<class T> T next() const { T x; *in >> x; return x; }
  int ni() const { return next<int>(); }
  template<class T> vector<T> nv(int n) const { vector<T> v(n); for (int i = 0; i < n; ++i) v[i] = next<T>(); return v; }
  vector<int> nvi(int n) const { return nv<int>(n); }
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
  template<typename T> inline void printVector(const vector<T>& v) { for (int i = 0; i < v.size(); i++) { if (i) *out << ' '; print(v[i]); } }
  template<typename T> inline void printlnVector(const vector<T>& v) { printVector(v); *out << '\n'; }
  template<typename...Ts> inline void operator() (const Ts&... args) { println(args...); }
  template<typename T> inline void operator() (const vector<T>& v) { printlnVector(v); }
};

template <typename T> vector<vector<T>> matrix(int n, int m, const T& init) {
  vector<vector<T>> v(n, vector<T>(m, init));
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

const int INF = 1000000000;

void solve(Input& in, Output& out) {
  while (true) {
    auto n = in.ni();
    if (n == -1)
      break;
    auto m = in.ni();
    auto w = matrix<int>(n, n, INF);
    for (int i = 0; i < m; i++) {
      auto a = in.ni() - 1;
      auto b = in.ni() - 1;
      auto weight = in.ni();
      if (weight < w[a][b]) {
        w[a][b] = weight;
        w[b][a] = weight;
      }
    }

    auto d = matrix<int>(n, n, INF);
    auto first = matrix<int>(n, n, -1);
    auto mid = matrix<int>(n, n, -1);
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        d[i][j] = w[i][j];
        first[i][j] = j;
      }
      w[i][i] = 0;
    }

    for (int k = 0; k < n; k++) {
      for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
          if (d[i][k] + d[k][j] < d[i][j]) {
            d[i][j] = d[i][k] + d[k][j];
            first[i][j] = first[i][k];
            mid[i][j] = k;
          }
        }
      }
    }

    int result = INF;
    int ri, rj, ro;
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        if (i == j || d[i][j] == INF)
          continue;
        for (int o = 0; o < n; o++) {
          if (o == i || o == j || o == first[i][j] || w[i][o] == INF)
            continue;
          int r = d[i][j] + d[o][j] + w[o][i];
          if (r < result && first[o][j] != i) {
            result = r;
            ri = i;
            rj = j;
            ro = o;
          }
        }
      }
    }

    if (result == INF) {
      out("No solution.");
    } else {
      vector<int> r;
      function<void(int, int)> path = [&] (int from, int to) {
        if (w[from][to] != d[from][to]) {
          path(from, mid[from][to]);
          path(mid[from][to], to);
        } else {
          r.push_back(to + 1);
        }
      };

      r.push_back(ri + 1);
      path(ri, rj);
      path(rj, ro);
      out(r);
    }
  }
}
