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
using uint128_t = __uint128_t;

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
  int cols = in.ni();
  int rows = in.ni() + 1;
  int target_col = in.ni() - 1;
  int target_row = rows - in.ni();
  int D = in.ni();

  vec<string> w(rows);
  w[0] = string(cols, '.');
  for (int r = 1; r < rows; r++)
    w[r] = in.ns();

  // out(w);

  vec<pair<int, int>> q;
  for (int c = 0; c < cols; c++)
    q.push_back(make_pair(0, c));
  for (int tail = 0; tail < q.size(); tail++) {
    int r = q[tail].first;
    int c = q[tail].second;
    if (r + 1 < rows && w[r + 1][c] == '.') {
      w[r + 1][c] = 'w';
      q.push_back(make_pair(r + 1, c));
    }
    if (c + 1 < cols && w[r][c + 1] == '.' && r > 0) {
      w[r][c + 1] = 'w';
      q.push_back(make_pair(r, c + 1));
    }
    if (c - 1 >= 0 && w[r][c - 1] == '.' && r > 0) {
      w[r][c - 1] = 'w';
      q.push_back(make_pair(r, c - 1));
    }
  }

  // out(w);

  q.clear();
  vec2d<int> dist = newVec2d(rows, cols, rows * cols + 1);
  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < cols; c++) {
      if (w[r][c] == '.') {
        dist[r][c] = 0;
        q.push_back(make_pair(r, c));
      }
    }
  }
  if (w[target_row][target_col] != '.') {
    dist[target_row][target_col] = 0;
    q.push_back(make_pair(target_row, target_col));
  }
  for (int tail = 0; tail < q.size(); tail++) {
    int r = q[tail].first;
    int c = q[tail].second;
    int d = dist[r][c] + 1;
    if (r + 1 < rows && dist[r + 1][c] > d && w[r + 1][c] != '#') {
      dist[r + 1][c] = d;
      q.push_back(make_pair(r + 1, c));
    }
    if (r - 1 >= 0 && dist[r - 1][c] > d && w[r - 1][c] != '#') {
      dist[r - 1][c] = d;
      q.push_back(make_pair(r - 1, c));
    }
    if (c + 1 < cols && dist[r][c + 1] > d && w[r][c + 1] != '#') {
      dist[r][c + 1] = d;
      q.push_back(make_pair(r, c + 1));
    }
    if (c - 1 >= 0 && dist[r][c - 1] > d && w[r][c - 1] != '#') {
      dist[r][c - 1] = d;
      q.push_back(make_pair(r, c - 1));
    }
  }
  // out(dist);

  q.clear();
  w[target_row][target_col] = 'R';
  q.push_back(make_pair(target_row, target_col));
  for (int tail = 0; tail < q.size(); tail++) {
    int r = q[tail].first;
    int c = q[tail].second;
    int d = D - dist[r][c] - 1;
    if (r + 1 < rows && dist[r + 1][c] <= d && w[r + 1][c] != 'R') {
      w[r + 1][c] = 'R';
      q.push_back(make_pair(r + 1, c));
    }
    if (r - 1 >= 0 && dist[r - 1][c] <= d && w[r - 1][c] != 'R') {
      w[r - 1][c] = 'R';
      q.push_back(make_pair(r - 1, c));
    }
    if (c + 1 < cols && dist[r][c + 1] <= d && w[r][c + 1] != 'R') {
      w[r][c + 1] = 'R';
      q.push_back(make_pair(r, c + 1));
    }
    if (c - 1 >= 0 && dist[r][c - 1] <= d && w[r][c - 1] != 'R') {
      w[r][c - 1] = 'R';
      q.push_back(make_pair(r, c - 1));
    }
  }

  bool reaches = false;
  for (int c = 0; c < cols; c++) {
    if (w[0][c] == 'R')
      reaches = true;
  }
  out(reaches ? "Can be rescued by himself" : "Rescue operation required");
}
