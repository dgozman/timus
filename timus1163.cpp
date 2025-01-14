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

using D = long double;

void solve(Input& in, Output& out) {
  vec<D> x(16);
  vec<D> y(16);
  for (int i = 0; i < 16; i++) {
    x[i] = in.next<D>();
    y[i] = in.next<D>();
  }

  const D R2 = 0.8;
  const D eps = 1e-8;

  vec<vec<int>> moves(16);
  for (int from = 0; from < 16; from++) {
    vec<D> angs;
    for (int to = 0; to < 16; to++) {
      if (to == from)
        continue;
      D xi = x[to] - x[from];
      D yi = y[to] - y[from];
      D a = atan2(yi, xi);
      angs.push_back(a);
      D di = sqrt(xi * xi + yi * yi);
      D da = asin(R2 / di);
      angs.push_back(a + da);
      angs.push_back(a - da);
    }
    sort(angs.begin(), angs.end());

    set<int> all_moves;
    auto calc = [&] (D a) {
      int move = 0;
      D dx = cos(a);
      D dy = sin(a);
      for (int j = 0; j < 16; j++) {
        D xj = x[j] - x[from];
        D yj = y[j] - y[from];
        D t = dx * xj + dy * yj;
        D dist = sqrt((xj - dx * t) * (xj - dx  * t) + (yj - dy * t) * (yj - dy * t));
        if (dist < R2 + eps && t > -eps)
          move |= 1 << j;
      }
      if (((move >> from) & 1) == 0)
        panic();
      all_moves.insert(move);
    };

    for (int i = 0; i < angs.size(); i++) {
      calc(angs[i]);
      int j = (i + 1) % angs.size();
      calc((angs[i] + angs[j]) / 2);
    }

    for (int move : all_moves)
      moves[from].push_back(move);
  }

  vec<int> win(1 << 17, -1);
  function<int(int, int)> dfs = [&] (int state, int player) {
    int cache = state + (player << 16);
    if (win[cache] != -1)
      return win[cache];
    int start = player * 8;
    bool can = false;
    for (int i = start; i < start + 8; i++) {
      if (((state >> i) & 1) == 0)
        continue;
      for (int move : moves[i]) {
        if (dfs(state & ~move, 1 - player) == player) {
          can = true;
          break;
        }
      }
      if (can)
        break;
    }
    win[cache] = can ? player : 1 - player;
    return win[cache];
  };

  if (dfs((1 << 16) - 1, 0) == 0)
    out("RED");
  else
    out("WHITE");
}

/*
0.5 0.5 1.5 0.5 2.5 0.5 3.5 0.5 4.5 0.5 5.5 0.5 6.5 0.5 7.5 0.5
-1 0.5 10 0.5 0.5 7.5 1.5 7.5 2.5 7.5 3.5 7.5 4.5 7.5 5.5 7.5
WHITE

*/
