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

void panic() {
  int z = 0;
  z = 1 / z;
}

void solve(Input& in, Output& out);
int main() {
//   ifstream fin("input.txt");
//   Input in(fin);
//   ofstream fout("output.txt");
//   Output out(fout);
  Input in(cin);
  Output out(cout);
  solve(in, out);
  return 0;
}

using Match = vec<int>;
using Path = vec<int>;

struct Graph {
  int n;
  matrix<int> w;

  Match match() {
    Match m = vec<int>(n, -1);
    while (augment(m)) ;
    return m;
  }

  bool augment(Match& m) {
    Path p;
    if (!findPath(m, p))
      return false;
    for (int i = 0; i < p.size(); i += 2) {
      m[p[i]] = p[i + 1];
      m[p[i + 1]] = p[i];
    }
    return true;
  }

  bool findPath(Match& m, Path& p) {
    vec<int> mark(n, 0);
    vec<int> blossom;
    const int NONE = 0;
    const int AUGMENT = 1;

    function<int(int u)> dfs = [&] (int u) {
      p.push_back(u);
      mark[u] = 1;
      for (int v = 0; v < n; v++) {
        if (u == v || w[u][v] == 0)
          continue;
        if (mark[v] == 0 && m[v] == -1) {
          p.push_back(v);
          return AUGMENT;
        } else if (mark[v] == 0 && m[v] != -1) {
          mark[v] = 2;
          p.push_back(v);
          int result = dfs(m[v]);
          if (result != NONE)
            return result;
          p.pop_back();
        } else if (mark[v] == 1) {
          if (blossom.size() == 0) {
            blossom.push_back(v);
            while (true) {
              int v = p[p.size() - blossom.size()];
              if (v == blossom[0])
                break;
              blossom.push_back(v);
            }
          }
        } else {
          if (mark[v] != 2)
            panic();
          // mark[v] == 2, do nothing
        }
      }
      p.pop_back();
      return NONE;
    };

    for (int start = 0; start < n; start++) {
      if (mark[start] != 0 || m[start] != -1)
        continue;
      int result = dfs(start);
      if (result == NONE)
        continue;
      if (result == AUGMENT)
        return true;
      panic();
    }

    if (blossom.size() == 0)
      return false;

    // Contract blossom into vertex 0.

    vec<int> taken(n, 0);
    for (int b : blossom)
      taken[b] = 1;

    Graph cg;
    cg.n = 1;
    vec<int> oldToNew(n);
    for (int i = 0; i < n; i++) {
      if (taken[i] == 0) {
        oldToNew[i] = cg.n++;
      } else {
        oldToNew[i] = 0;
      }
    }
    cg.w = newmatrix(cg.n, cg.n, 0);
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        if (w[i][j] == 1)
          cg.w[oldToNew[i]][oldToNew[j]] = 1;
      }
    }
    Match cm(cg.n, -1);
    for (int i = 0; i < n; i++) {
      if (m[i] != -1 && (i == blossom[0] || taken[i] == 0))
        cm[oldToNew[i]] = oldToNew[m[i]];
    }

    Path cp;
    if (!cg.findPath(cm, cp))
      return false;

    p.clear();
    vec<int> newToOld(n);
    for (int i = 0; i < n; i++)
      newToOld[oldToNew[i]] = i;

    for (int i = 0; i < cp.size(); i++) {
      if (cp[i] == 0) {
        bool reverse = !(i == 0 || cp[i - 1] == cm[0]);
        int next = reverse ? newToOld[cp[i - 1]] : newToOld[cp[i + 1]];
        int bi = 0;
        while (w[blossom[bi]][next] == 0) {
          bi++;
          if (bi == blossom.size())
            panic();
        }
        if (bi % 2 == 0) {
          if (reverse) {
            for (int j = bi; j >= 0; j--)
              p.push_back(blossom[j]);
          } else {
            for (int j = 0; j <= bi; j++)
              p.push_back(blossom[j]);
          }
        } else {
          if (reverse) {
            for (int j = bi; j <= blossom.size() - 1; j++)
              p.push_back(blossom[j]);
            p.push_back(blossom[0]);
          } else {
            p.push_back(blossom[0]);
            for (int j = blossom.size() - 1; j >= bi; j--)
              p.push_back(blossom[j]);
          }
        }
      } else {
        p.push_back(newToOld[cp[i]]);
      }
    }

    return true;
  }
};

void solve(Input& in, Output& out) {
  Graph g;
  g.n = in.ni();
  g.w = newmatrix(g.n, g.n, 0);
  while (true) {
    int a;
    if (!(*in.in >> a))
      break;
    int b = in.ni();
    g.w[a - 1][b - 1] = 1;
    g.w[b - 1][a - 1] = 1;
  }

  Match match = g.match();
  vec<int> taken(g.n, 0);
  int count = 0;
  for (int i = 0; i < g.n; i++) {
    if (match[i] != -1)
      count++;
  }
  out(count);
  for (int i = 0; i < g.n; i++) {
    if (match[i] != -1 && taken[i] == 0) {
      taken[i] = 1;
      taken[match[i]] = 1;
      out.print(i + 1);
      out.print(" ");
      out.println(match[i] + 1);
    }
  }
}
