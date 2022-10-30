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
  // http://gaebler.us/share/Knight_tour.html
  vec<int> dr({0, 2, 2, -2, -2, 1, 1, -1, -1});
  vec<int> dc({0, 1, -1, 1, -1, 2, -2, 2, -2});
  vec<int> dback({0, 4, 3, 2, 1, 8, 7, 6, 5});

//  for (int xn = 6; xn <= 20; xn++) {
//  for (int xm = 6; xm <= 20; xm++) {
//  int n = xn;
//  int m = xm;

  int n = in.ni();
  int m = in.ni();
  bool swap = false;
  if (n % 2 == 1) {
    swap = true;
    int t = n;
    n = m;
    m = t;
  }
  if (n % 2 == 1) {
    out("No solution");
    return;
  }

  auto visit = [&] (const vec2d<int>& w, int rows, int cols) {
    set<pair<int, int>> all;
    vec<pair<int, int>> list;
    int r = 0;
    int c = 0;
    all.insert(make_pair(r, c));
    list.push_back(make_pair(r, c));
    while (true) {
      int d = w[r][c];
      if (d < 1 || d > 8)
        panic();
      int nr = r + dr[d];
      int nc = c + dc[d];
      if (nr < 0 || nr >= rows || nc < 0 || nc >= cols)
        panic();
      if (nr == 0 && nc == 0)
        break;
      if (all.find(make_pair(nr, nc)) != all.end())
        panic();
      all.insert(make_pair(nr, nc));
      list.push_back(make_pair(nr, nc));
      r = nr;
      c = nc;
    }
    if (all.size() != rows * cols)
      panic();
    return list;
  };

  vec2d<int> w = newVec2d(n, m, 0);

  auto copy = [&] (const vec2d<int>& b, int br, int bc, int row, int col) {
    for (int r = 0; r < br; r++) {
      for (int c = 0; c < bc; c++)
        w[row + r][col + c] = b[r][c];
    }
  };

  vec2d<int> base4x3({
    {5, 1, 2},
    {7, 1, 2},
    {3, 6, 6},
    {8, 4, 8}
  });
  vec2d<int> base4x6({
    {5, 1, 5, 5, 1, 2},
    {7, 7, 6, 7, 1, 2},
    {3, 4, 6, 3, 6, 6},
    {3, 4, 0, 8, 4, 8}
  });
  vec2d<int> base4x7({
    {5, 1, 1, 5, 5, 1, 2},
    {7, 7, 6, 2, 7, 1, 2},
    {3, 4, 6, 3, 3, 6, 6},
    {3, 4, 0, 8, 4, 4, 8}
  });
  vec2d<int> base4x8({
    {5, 1, 1, 1, 5, 5, 1, 2},
    {7, 7, 6, 2, 2, 7, 1, 2},
    {3, 4, 6, 3, 3, 3, 6, 6},
    {3, 4, 0, 8, 4, 4, 4, 8}
  });

  auto gen4xm = [&] (int m, int row, int col) {
    int base = (m % 3) + 6;
    if (base == 6)
      copy(base4x6, 4, 6, row, col);
    else if (base == 7)
      copy(base4x7, 4, 7, row, col);
    else
      copy(base4x8, 4, 8, row, col);
    while (base < m) {
      if (w[row][col + base - 1] != 2)
        panic();
      w[row][col + base - 1] = 5;
      if (w[row + 1][col + base - 2] != 1)
        panic();
      w[row + 1][col + base - 2] = 7;
      copy(base4x3, 4, 3, row, col + base);
      base += 3;
    }
  };

  auto reverse = [&] (int rstart, int cstart, int rend, int cend) {
    if (w[rend][cend] != 0)
      panic();
    int r = rstart;
    int c = cstart;
    int prev = dback[w[r][c]];
    int nr = r + dr[w[r][c]];
    int nc = c + dc[w[r][c]];
    w[r][c] = 0;
    r = nr;
    c = nc;
    while (r != rend || c != cend) {
      nr = r + dr[w[r][c]];
      nc = c + dc[w[r][c]];
      int next = dback[w[r][c]];
      w[r][c] = prev;
      prev = next;
      r = nr;
      c = nc;
    }
    w[r][c] = prev;
  };

  auto append4 = [&] (int rend, int cend) {
    // rend == row + 5
    // cend == col + base - 1
    if (w[rend - 2][cend - 1] == 1) {
      if (w[rend][cend] != 8)
        panic();
      w[rend - 2][cend - 1] = 5;
      if (w[rend - 2][cend + 1] != 0)
        panic();
      w[rend - 2][cend + 1] = 2;
    } else {
      if (w[rend][cend] != 4)
        panic();
      if (w[rend - 1][cend - 2] != 5)
        panic();
      w[rend][cend] = 3;
      reverse(rend - 1, cend + 1, rend - 2, cend + 1);
      if (w[rend - 1][cend + 1] != 0)
        panic();
      w[rend - 1][cend + 1] = 8;
    }
  };

  vec2d<int> base6x4({
    {1, 2, 6, 6},
    {5, 2, 8, 8},
    {1, 7, 3, 4},
    {0, 2, 2, 4},
    {5, 5, 3, 8},
    {7, 7, 3, 4}
  });
  vec2d<int> base6x5({
    {1, 2, 5, 1, 6},
    {7, 5, 8, 8, 6},
    {1, 2, 3, 3, 2},
    {3, 4, 1, 1, 4},
    {5, 7, 6, 6, 8},
    {3, 4, 7, 3, 8}
  });
  vec2d<int> base6x6({
    {5, 1, 5, 5, 2, 6},
    {7, 7, 7, 8, 1, 2},
    {1, 4, 6, 8, 3, 2},
    {5, 4, 8, 7, 1, 2},
    {3, 5, 6, 6, 6, 4},
    {3, 3, 8, 7, 4, 8}
  });
  vec2d<int> base6x7({
    {5, 5, 1, 5, 5, 2, 2},
    {7, 7, 7, 6, 7, 2, 2},
    {3, 4, 7, 8, 5, 1, 4},
    {5, 4, 1, 6, 8, 1, 2},
    {7, 4, 6, 6, 6, 3, 6},
    {3, 4, 8, 7, 4, 8, 8}
  });
  vec2d<int> base6x8({
    {5, 2, 6, 1, 2, 6, 1, 2},
    {1, 7, 7, 2, 7, 7, 1, 8},
    {1, 4, 4, 4, 7, 7, 1, 8},
    {3, 2, 2, 3, 5, 1, 1, 8},
    {3, 7, 8, 4, 6, 6, 6, 6},
    {7, 7, 8, 3, 3, 3, 8, 8}
  });
  auto gen6xm = [&] (int m, int row, int col) {
    int base = (m % 4) + 4;
    if (base == 4)
      base = 8;
    if (base == 5)
      copy(base6x5, 6, 5, row, col);
    else if (base == 6)
      copy(base6x6, 6, 6, row, col);
    else if (base == 7)
      copy(base6x7, 6, 7, row, col);
    else
      copy(base6x8, 6, 8, row, col);
    while (base < m) {
      copy(base6x4, 6, 4, row, col + base);
      append4(row + 5, col + base - 1);
      base += 4;
    }
  };

  vec2d<int> base8x4({
    {1, 2, 6, 6},
    {5, 2, 8, 8},
    {5, 7, 3, 4},
    {5, 7, 6, 4},
    {1, 7, 6, 8},
    {0, 2, 2, 8},
    {5, 5, 3, 8},
    {7, 7, 3, 4}
  });
  vec2d<int> base8x5({
    {1, 2, 5, 6, 6},
    {7, 2, 8, 8, 2},
    {5, 2, 8, 3, 4},
    {7, 2, 7, 6, 4},
    {1, 2, 7, 1, 4},
    {1, 7, 4, 1, 8},
    {5, 5, 6, 3, 8},
    {3, 7, 3, 3, 8}
  });
  vec2d<int> base8x6({
    {1, 1, 6, 6, 6, 6},
    {1, 1, 8, 5, 2, 6},
    {3, 3, 3, 5, 3, 4},
    {1, 5, 6, 5, 3, 4},
    {1, 4, 8, 1, 1, 4},
    {7, 2, 7, 8, 1, 4},
    {7, 5, 5, 6, 6, 8},
    {7, 4, 8, 3, 3, 8}
  });
  vec2d<int> base8x7({
    {1, 2, 5, 5, 2, 1, 2},
    {7, 7, 8, 7, 7, 2, 8},
    {5, 7, 4, 8, 5, 6, 2},
    {5, 4, 1, 4, 5, 3, 6},
    {5, 1, 2, 4, 2, 4, 2},
    {3, 4, 2, 2, 5, 1, 4},
    {3, 4, 6, 5, 4, 6, 6},
    {3, 4, 8, 3, 3, 3, 8}
  });
  vec2d<int> base8x8({
    {5, 5, 2, 6, 1, 5, 1, 2},
    {7, 2, 6, 2, 2, 7, 8, 2},
    {3, 4, 8, 6, 4, 3, 4, 8},
    {7, 5, 6, 1, 6, 4, 8, 4},
    {1, 5, 1, 7, 4, 7, 8, 6},
    {5, 1, 2, 1, 1, 5, 1, 2},
    {3, 5, 6, 3, 3, 7, 3, 4},
    {3, 4, 8, 4, 7, 3, 8, 8}
  });
  auto gen8xm = [&] (int m, int row, int col) {
    int base = (m % 4) + 4;
    if (base == 4)
      base = 8;
    if (base == 5)
      copy(base8x5, 8, 5, row, col);
    else if (base == 6)
      copy(base8x6, 8, 6, row, col);
    else if (base == 7)
      copy(base8x7, 8, 7, row, col);
    else
      copy(base8x8, 8, 8, row, col);
    while (base < m) {
      copy(base8x4, 8, 4, row, col + base);
      append4(row + 7, col + base - 1);
      base += 4;
    }
  };

  int base = 0;
  if (n % 4 == 2) {
    gen6xm(m, n - 6, 0);
    base = n - 6;
  } else {
    gen8xm(m, n - 8, 0);
    base = n - 8;
  }
  while (base > 0) {
    base -= 4;
    gen4xm(m, base, 0);
    if (w[base + 4][0] == 5) {
      if (w[base + 6][1] != 4)
        panic();
      w[base + 4][0] = 7;
      reverse(base + 3, 1, base + 3, 2);
      if (w[base + 3][1] != 0)
        panic();
      w[base + 3][1] = 1;
    } else {
      if (w[base + 4][0] != 1)
        panic();
      if (w[base + 5][2] != 8)
        panic();
      w[base + 5][2] = 4;
      if (w[base + 3][2] != 0)
        panic();
      w[base + 3][2] = 6;
    }
  }

  if (swap) {
    auto rotate = [&] (pair<int, int> pos) {
      return make_pair(pos.second, pos.first);
    };
    vec<pair<int, int>> list = visit(w, n, m);
    vec2d<int> s = newVec2d(m, n, 0);
    for (int i = 0; i < list.size(); i++) {
      auto cur = rotate(list[i]);
      auto prev = rotate(i == 0 ? list[list.size() - 1] : list[i - 1]);
      for (int d = 1; d <= 8; d++) {
        if (dr[d] == prev.first - cur.first && dc[d] == prev.second - cur.second)
          s[cur.first][cur.second] = d;
      }
      if (s[cur.first][cur.second] == 0)
        panic();
    }
    w = s;
    int t = m;
    m = n;
    n = t;
  }

  visit(w, n, m);

//  out.print(n);
//  out.print(" ");
//  out(m);

  for (int i = 0; i < n; i++)
    out(w[i]);

//  }
//  }
}
