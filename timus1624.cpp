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
  // N, S, E, W
  const int NORTH = 0;
  const int SOUTH = 1;
  const int EAST = 2;
  const int WEST = 3;
  vec<int> dr({-1, 1, 0, 0});
  vec<int> dc({0, 0, 1, -1});
  vec<int> dreverse({SOUTH, NORTH, WEST, EAST});
  int cols = in.ni();
  int rows = in.ni();
  int K = in.ni();
  vec<int> dir(K);
  vec<bool> success(K);
  for (int i = 0; i < K; i++) {
    string s = in.ns();
    if (s[0] == 'N')
      dir[i] = 0;
    else if (s[0] == 'S')
      dir[i] = 1;
    else if (s[0] == 'E')
      dir[i] = 2;
    else if (s[0] == 'W')
      dir[i] = 3;
    else
      panic();
    success[i] = s[1] == '+';
  }

  auto fill = [&] (int P, int start, vec2d<u8>& closed, vec2d<u8>& open, int& min_row, int& max_row, int& min_col, int& max_col) {
    int row = rows - 1;
    int col = cols - 1;
    int max_allowed_row = row + rows - 1;
    int max_allowed_col = col + cols - 1;
    closed = newVec2d(max_allowed_row + 1, max_allowed_col + 1, (u8) 0);
    open = newVec2d(max_allowed_row + 1, max_allowed_col + 1, (u8) 0);
    min_row = max_row = row;
    min_col = max_col = col;
    for (int i = start; i < P; i += 2) {
      int r = row + dr[dir[i]];
      int c = col + dc[dir[i]];
      if (!success[i]) {
        if (open[row][col] & (1 << dir[i]))
          return false;
        closed[row][col] |= (1 << dir[i]);
        if (r >= 0 && c >= 0 && r <= max_allowed_row && c <= max_allowed_col)
          closed[r][c] |= (1 << dreverse[dir[i]]);
      } else {
        if (closed[row][col] & (1 << dir[i]))
          return false;
        if (r < 0 || c < 0 || r > max_allowed_row || c > max_allowed_col)
          return false;
        open[row][col] |= (1 << dir[i]);
        open[r][c] |= (1 << dreverse[dir[i]]);
        row = r;
        col = c;
        min_row = min(min_row, r);
        max_row = max(max_row, r);
        min_col = min(min_col, c);
        max_col = max(max_col, c);
      }
    }
    return true;
  };

  auto count_positions = [&] (int P) {
    vec2d<u8> closed1, open1, closed2, open2;
    int min_row1, max_row1, min_col1, max_col1, min_row2, max_row2, min_col2, max_col2;
    if (!fill(P, 0, closed1, open1, min_row1, max_row1, min_col1, max_col1))
      return 0;
    if (!fill(P, 1, closed2, open2, min_row2, max_row2, min_col2, max_col2))
      return 0;

    int count = 0;
    for (int sr = -rows; sr <= rows; sr++) {
      int min_row = min(min_row1, sr + min_row2);
      int max_row = max(max_row1, sr + max_row2);
      int row_span = max_row - min_row;
      if (row_span >= rows)
        continue;

      for (int sc = -cols; sc <= cols; sc++) {
        int min_col = min(min_col1, sc + min_col2);
        int max_col = max(max_col1, sc + max_col2);
        int col_span = max_col - min_col;
        if (col_span >= cols)
          continue;

        bool good = true;
        for (int r = min_row1; good && r <= max_row1; r++) {
          for (int c = min_col1; good && c <= max_col1; c++) {
            if (open1[r][c] & closed2[r - sr][c - sc])
              good = false;
            if (closed1[r][c] & open2[r - sr][c - sc])
              good = false;
          }
        }
        if (good)
          count += (rows - row_span) * (cols - col_span);
      }
    }
    return count;
  };

  int total = count_positions(K);
  if (total > 1) {
    out("There is not enough data");
    return;
  }

  int l = 1;
  int r = K;
  while (l < r) {
    int m = (l + r) / 2;
    int partial = count_positions(m);
    if (partial > total)
      l = m + 1;
    else
      r = m;
  }
  out(total == 0 ? "A mistake has been made at step number" : "The scouts are safe at step number", l);
}
