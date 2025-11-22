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
using i16 = int16_t;
using u16 = uint16_t;
// using uint128_t = __uint128_t;

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
  throw 42;
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
  vec2d<char> grid = newVec2d(5, 5, '*');
  for (int i = 0; i < 5; ++i) {
    string s = in.ns();
    for (int j = 0; j < 5; ++j)
      grid[i][j] = s[j];
  }

  vec2d<bool> visited = newVec2d(11, 11, false);
  array<char, 4> dirs = {'D', 'R', 'L', 'U'};
  array<char, 4> dx = {-1, 0, 0, 1};
  array<char, 4> dy = {0, -1, 1, 0};

  function<int(int, int, int, int)> dfs = [&](int x, int y, int tx, int ty) -> int {
    // out("dfs", x, y, tx, ty);
    bool is_start_way = (tx == x || tx == -1) && (ty == y || ty == -1);
    if ((x == 0 || x == 10 || y == 0 || y == 10) && !is_start_way)
      return 0;
    visited[x][y] = true;
    if (!(x % 2)) {
      int r = x / 2;
      int c = y / 2;
      if (r < 5) {
        if (grid[r][c] == '\\' && !visited[x + 1][y + 1])
          return 1 + dfs(x + 1, y + 1, tx, ty);
        if (grid[r][c] == '/' && !visited[x + 1][y - 1])
          return 1 + dfs(x + 1, y - 1, tx, ty);
      }
      if (r > 0) {
        if (grid[r - 1][c] == '\\' && !visited[x - 1][y - 1])
          return 1 + dfs(x - 1, y - 1, tx, ty);
        if (grid[r - 1][c] == '/' && !visited[x - 1][y + 1])
          return 1 + dfs(x - 1, y + 1, tx, ty);
      }
    } else {
      int r = x / 2;
      int c = y / 2;
      if (c < 5) {
        // out("dfs", x, y, tx, ty, r, c, grid[r][c], visited[x + 1][y + 1]);
        if (grid[r][c] == '\\' && !visited[x + 1][y + 1])
          return 1 + dfs(x + 1, y + 1, tx, ty);
        if (grid[r][c] == '/' && !visited[x - 1][y + 1])
          return 1 + dfs(x - 1, y + 1, tx, ty);
      }
      if (c > 0) {
        if (grid[r][c - 1] == '\\' && !visited[x - 1][y - 1])
          return 1 + dfs(x - 1, y - 1, tx, ty);
        if (grid[r][c - 1] == '/' && !visited[x + 1][y - 1])
          return 1 + dfs(x + 1, y - 1, tx, ty);
      }
    }
    return -10000;
  };

  auto is_win = [&]() {
    for (int i = 0; i < 11; ++i) {
      for (int j = 0; j < 11; ++j)
        visited[i][j] = false;
    }
    for (int i = 1; i <= 9; i += 2) {
      int c;
      // out("====");
      c = dfs(0, i, 0, -1);
      if (c >= 3) {
        // out("res", c, 0, i, 10, -1);
        return true;
      }
      // out("====");
      c = dfs(10, i, 10, -1);
      if (c >= 3) {
        // out("res", c, 10, i, 0, -1);
        return true;
      }
      // out("====");
      c = dfs(i, 0, -1, 0);
      if (c >= 3) {
        // out("res", c, i, 0, -1, 10);
        return true;
      }
      // out("====");
      c = dfs(i, 10, -1, 10);
      if (c >= 3) {
        // out("res", c, i, 10, -1, 0);
        return true;
      }
    }
    return false;
  };

  auto find_star = [&](int& sx, int& sy) {
    for (sx = 0; sx < 5; ++sx) {
      for (sy = 0; sy < 5; ++sy) {
        if (grid[sx][sy] == '*')
          return;
      }
    }
  };

  auto move = [&](int dir, int count) {
    int sx, sy;
    find_star(sx, sy);
    if (sx + dx[dir] * count < 0 || sx + dx[dir] * count >= 5 || sy + dy[dir] * count < 0 || sy + dy[dir] * count >= 5)
      return false;
    for (int i = 1; i <= count; i++) {
      int nx = sx + dx[dir];
      int ny = sy + dy[dir];
      swap(grid[sx][sy], grid[nx][ny]);
      sx = nx;
      sy = ny;
    }
    return true;
  };

  if (is_win()) {
    out("WIN");
    return;
  }

  for (int dir = 0; dir < 4; dir++) {
    for (int count = 1; count <= 4; count++) {
      if (move(dir, count)) {
        if (is_win()) {
          out.print(dirs[dir]);
          out.print(count);
          out.println();
          return;
        }
        move(3 - dir, count);
      }
    }
  }

  out("FAIL");
}
