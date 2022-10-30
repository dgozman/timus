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

vec<pair<int, int>> solve6(int n, int m) {
  vec<pair<int, int>> w(n * m);
  int index = 0;

  auto l2 = [&] () {
    w[index++] = {n - 1, 1};
    w[index++] = {n - 1, 0};
    w[index++] = {n - 2, 0};
    w[index++] = {n - 2, 1};
    for (int j = n - 3; j >= 0; j--) {
      w[index++] = {j, 0};
      w[index++] = {j, 1};
    }
    w[index++] = {1, 2};
    w[index++] = {0, 2};
  };

  auto top = [&] (int l, int r) {
    w[index++] = {1, l + 1};
    w[index++] = {0, l + 1};
    if (m - r - 1 < l + 2)
      panic();
    for (int j = l + 2; j < m - r - 1; j++) {
      w[index++] = {0, j};
      w[index++] = {1, j};
    }
  };

  auto r2 = [&] () {
    w[index++] = {0, m - 3};
    w[index++] = {1, m - 3};
    w[index++] = {0, m - 2};
    w[index++] = {0, m - 1};
    for (int j = 1; j < n - 2; j++) {
      w[index++] = {j, m - 1};
      w[index++] = {j, m - 2};
    }
    w[index++] = {n - 2, m - 1};
    w[index++] = {n - 1, m - 1};
    w[index++] = {n - 1, m - 2};
    w[index++] = {n - 2, m - 2};
  };

  auto l3 = [&] () {
    for (int j = n - 1; j >= 3; j -= 2) {
      w[index++] = {j, 2};
      w[index++] = {j - 1, 2};
      w[index++] = {j, 1};
      w[index++] = {j, 0};
      w[index++] = {j - 1, 0};
      w[index++] = {j - 1, 1};
    }
    if (n % 2) {
      w[index++] = {2, 1};
      w[index++] = {2, 0};
      w[index++] = {1, 1};
      w[index++] = {1, 0};
      w[index++] = {0, 0};
      w[index++] = {0, 1};
      w[index++] = {1, 2};
      w[index++] = {2, 2};
      w[index++] = {1, 3};
      w[index++] = {0, 2};
      w[index++] = {0, 3};
    } else {
      w[index++] = {1, 1};
      w[index++] = {1, 0};
      w[index++] = {0, 0};
      w[index++] = {0, 1};
      w[index++] = {1, 2};
      w[index++] = {0, 2};
      w[index++] = {1, 3};
      w[index++] = {0, 3};
    }
  };

  auto r3 = [&] () {
    if (n % 2) {
      w[index++] = {0, m - 4};
      w[index++] = {0, m - 3};
      w[index++] = {1, m - 4};
      w[index++] = {2, m - 3};
      w[index++] = {1, m - 3};
      w[index++] = {0, m - 2};
      w[index++] = {0, m - 1};
      w[index++] = {1, m - 1};
      w[index++] = {1, m - 2};
      w[index++] = {2, m - 1};
      w[index++] = {2, m - 2};
    } else {
      w[index++] = {0, m - 4};
      w[index++] = {1, m - 4};
      w[index++] = {0, m - 3};
      w[index++] = {1, m - 3};
      w[index++] = {0, m - 2};
      w[index++] = {0, m - 1};
      w[index++] = {1, m - 1};
      w[index++] = {1, m - 2};
    }

    for (int j = 3 + (n % 2); j <= n - 3; j += 2) {
      w[index++] = {j - 1, m - 3};
      w[index++] = {j, m - 3};
      w[index++] = {j - 1, m - 2};
      w[index++] = {j - 1, m - 1};
      w[index++] = {j, m - 1};
      w[index++] = {j, m - 2};
    }
    w[index++] = {n - 2, m - 1};
    w[index++] = {n - 1, m - 1};
    w[index++] = {n - 1, m - 2};
    w[index++] = {n - 2, m - 2};
    w[index++] = {n - 1, m - 3};
    w[index++] = {n - 2, m - 3};
  };

  auto l4 = [&] () {
    w[index++] = {n - 1, 3};
    w[index++] = {n - 2, 3};
    w[index++] = {n - 1, 2};
    w[index++] = {n - 2, 2};
    w[index++] = {n - 1, 1};
    w[index++] = {n - 1, 0};
    w[index++] = {n - 2, 0};
    w[index++] = {n - 2, 1};
    for (int j = n - 3; j >= 3; j -= 2) {
      w[index++] = {j, 1};
      w[index++] = {j, 0};
      w[index++] = {j - 1, 0};
      w[index++] = {j - 1, 1};
      w[index++] = {j, 2};
      w[index++] = {j, 3};
      w[index++] = {j - 1, 3};
      w[index++] = {j - 1, 2};
    }
    if (n % 2) {
      w[index++] = {2, 1};
      w[index++] = {2, 0};
      w[index++] = {1, 0};
      w[index++] = {1, 1};
      w[index++] = {0, 0};
      w[index++] = {0, 1};
      w[index++] = {1, 2};
      w[index++] = {2, 2};
      w[index++] = {2, 3};
      w[index++] = {1, 3};
      w[index++] = {0, 2};
      w[index++] = {0, 3};
      w[index++] = {1, 4};
      w[index++] = {0, 4};
    } else {
      w[index++] = {1, 1};
      w[index++] = {1, 0};
      w[index++] = {0, 0};
      w[index++] = {0, 1};
      w[index++] = {1, 2};
      w[index++] = {0, 2};
      w[index++] = {1, 3};
      w[index++] = {0, 3};
      w[index++] = {1, 4};
      w[index++] = {0, 4};
    }
  };

  auto mid4 = [&] (int right) {
    w[index++] = {n - 1, right};
    w[index++] = {n - 1, right - 1};
    for (int j = n - 2; j >= 3; j--) {
      w[index++] = {j, right - 1};
      w[index++] = {j, right};
    }
    w[index++] = {2, right};
    w[index++] = {2, right - 1};
    w[index++] = {3, right - 2};
    w[index++] = {2, right - 2};
    w[index++] = {2, right - 3};
    w[index++] = {3, right - 3};
    for (int j = 4; j <= n - 3; j++) {
      w[index++] = {j, right - 2};
      w[index++] = {j, right - 3};
    }
    w[index++] = {n - 2, right - 2};
    w[index++] = {n - 1, right - 2};
    w[index++] = {n - 1, right - 3};
    w[index++] = {n - 2, right - 3};
  };

  auto mids = [&] (int r) {
    for (int right = m - r - 1; right >= 5; right -= 4)
      mid4(right);
  };

  if (m % 4 == 0) {
    l2();
    top(2, 2);
    r2();
    mids(2);
  } else if (m % 4 == 1) {
    l3();
    top(3, 2);
    r2();
    mids(2);
  } else if (m % 4 == 2) {
    l3();
    top(3, 3);
    r3();
    mids(3);
  } else if (m % 4 == 3) {
    l4();
    top(4, 3);
    r3();
    mids(3);
  }

  if (index != n * m)
    panic();
  return w;
}

vec<pair<int, int>> solve4(int n, int m) {
  vec<pair<int, int>> w(n * m);
  int index = 0;
  w[index++] = {0, 0};
  w[index++] = {0, 1};
  w[index++] = {1, 2};
  w[index++] = {0, 2};
  w[index++] = {0, 3};
  w[index++] = {1, 3};
  for (int j = 2; j < n - 1; j++) {
    w[index++] = {j, 2};
    w[index++] = {j, 3};
  }
  w[index++] = {n - 1, 3};
  w[index++] = {n - 1, 2};
  w[index++] = {n - 2, 1};
  w[index++] = {n - 1, 1};
  w[index++] = {n - 1, 0};
  w[index++] = {n - 2, 0};
  for (int j = n - 3; j >= 1; j--) {
    w[index++] = {j, 1};
    w[index++] = {j, 0};
  }
  return w;
}

vec<pair<int, int>> solve5(int n, int m) {
  // n == 5
  vec<pair<int, int>> w(n * m);
  int index = 0;
  w[index++] = {3, 1};
  w[index++] = {4, 1};
  w[index++] = {4, 0};
  w[index++] = {3, 0};
  w[index++] = {2, 1};
  w[index++] = {2, 0};
  w[index++] = {1, 0};
  w[index++] = {1, 1};
  w[index++] = {0, 0};
  w[index++] = {0, 1};
  if (m % 2) {
    for (int j = 2; j <= m - 6; j++) {
      w[index++] = {1, j};
      w[index++] = {0, j};
    }
    w[index++] = {0, m - 5};
    w[index++] = {1, m - 5};
    w[index++] = {0, m - 4};
    w[index++] = {0, m - 3};
    w[index++] = {1, m - 4};
    w[index++] = {2, m - 3};
    w[index++] = {1, m - 3};
    w[index++] = {0, m - 2};
    w[index++] = {0, m - 1};
    w[index++] = {1, m - 1};
    w[index++] = {1, m - 2};
    w[index++] = {2, m - 1};
    w[index++] = {2, m - 2};
    w[index++] = {3, m - 1};
    w[index++] = {4, m - 1};
    w[index++] = {4, m - 2};
    w[index++] = {3, m - 2};
    w[index++] = {4, m - 3};
    w[index++] = {3, m - 3};
  } else {
    for (int j = 2; j <= m - 2; j++) {
      w[index++] = {1, j};
      w[index++] = {0, j};
    }
    w[index++] = {0, m - 1};
    w[index++] = {1, m - 1};
    w[index++] = {2, m - 2};
    w[index++] = {2, m - 1};
    w[index++] = {3, m - 1};
    w[index++] = {3, m - 2};
    w[index++] = {4, m - 1};
    w[index++] = {4, m - 2};
  }
  for (int j = m - 3 - (m % 2); j >= 3; j -= 2) {
    w[index++] = {3, j};
    w[index++] = {2, j};
    w[index++] = {2, j - 1};
    w[index++] = {3, j - 1};
    w[index++] = {4, j};
    w[index++] = {4, j - 1};
  }
  return w;
}

vec<pair<int, int>> solve57() {
  vec<pair<int, int>> w({
    {0, 0},
    {0, 1},
    {1, 2},
    {2, 2},
    {3, 3},
    {2, 4},
    {2, 3},
    {1, 3},
    {0, 2},
    {0, 3},
    {1, 4},
    {0, 4},
    {0, 5},
    {1, 5},
    {0, 6},
    {1, 6},
    {2, 5},
    {2, 6},
    {3, 6},
    {3, 5},
    {4, 6},
    {4, 5},
    {3, 4},
    {4, 4},
    {4, 3},
    {3, 2},
    {4, 2},
    {4, 1},
    {3, 1},
    {4, 0},
    {3, 0},
    {2, 1},
    {2, 0},
    {1, 1},
    {1, 0},
  });
  return w;
}

vec<pair<int, int>> solve66() {
  vec<pair<int, int>> w({
    {0, 0},
    {0, 1},
    {1, 2},
    {0, 2},
    {0, 3},
    {1, 3},
    {0, 4},
    {0, 5},
    {1, 5},
    {1, 4},
    {2, 3},
    {3, 3},
    {2, 4},
    {2, 5},
    {3, 5},
    {3, 4},
    {4, 5},
    {5, 5},
    {5, 4},
    {4, 4},
    {5, 3},
    {4, 3},
    {5, 2},
    {4, 2},
    {5, 1},
    {5, 0},
    {4, 0},
    {4, 1},
    {3, 2},
    {2, 2},
    {3, 1},
    {3, 0},
    {2, 0},
    {2, 1},
    {1, 1},
    {1, 0},
  });
  return w;
}

vec<pair<int, int>> solve67() {
  vec<pair<int, int>> w({
    {0, 0},
    {0, 1},
    {1, 2},
    {0, 2},
    {1, 3},
    {0, 3},
    {0, 4},
    {1, 4},
    {0, 5},
    {0, 6},
    {1, 6},
    {1, 5},
    {2, 4},
    {3, 4},
    {2, 5},
    {2, 6},
    {3, 6},
    {3, 5},
    {4, 6},
    {5, 6},
    {5, 5},
    {4, 5},
    {5, 4},
    {4, 4},
    {5, 3},
    {4, 3},
    {5, 2},
    {4, 2},
    {5, 1},
    {5, 0},
    {4, 0},
    {4, 1},
    {3, 1},
    {3, 0},
    {2, 0},
    {2, 1},
    {3, 2},
    {3, 3},
    {2, 3},
    {2, 2},
    {1, 1},
    {1, 0},
  });
  return w;
}

vec<pair<int, int>> solve77() {
  vec<pair<int, int>> w({
    {0, 0},
    {0, 1},
    {1, 1},
    {0, 2},
    {1, 2},
    {0, 3},
    {1, 3},
    {0, 4},
    {1, 4},
    {0, 5},
    {0, 6},
    {1, 5},
    {1, 6},
    {2, 6},
    {2, 5},
    {3, 6},
    {4, 6},
    {4, 5},
    {5, 6},
    {6, 6},
    {5, 5},
    {6, 5},
    {6, 4},
    {5, 4},
    {6, 3},
    {5, 3},
    {6, 2},
    {5, 2},
    {6, 1},
    {6, 0},
    {5, 1},
    {5, 0},
    {4, 0},
    {4, 1},
    {3, 0},
    {2, 0},
    {3, 1},
    {3, 2},
    {4, 2},
    {4, 3},
    {3, 4},
    {4, 4},
    {3, 5},
    {2, 4},
    {2, 3},
    {3, 3},
    {2, 2},
    {2, 1},
    {1, 0},
  });
  return w;
}

void solve(Input& in, Output& out) {
  int n = in.ni();
  int m = in.ni();

  vec<pair<int, int>> w;
  auto transpose = [&] () {
    for (int i = 0; i < w.size(); i++)
      w[i] = make_pair(w[i].second, w[i].first);
  };

  if (n == 2 && m == 2) {
    w.push_back({0, 0});
    w.push_back({0, 1});
    w.push_back({1, 1});
    w.push_back({1, 0});
  } else if (m == 4 && n >= 4) {
    w = solve4(n, m);
  } else if (n == 4 && m >= 4) {
    w = solve4(m, n);
    transpose();
  } else if (n == 5 && m != 5 && m != 7) {
    w = solve5(n, m);
  } else if (m == 5 && n != 5 && n != 7) {
    w = solve5(m, n);
    transpose();
  } else if (n == 5 && m == 7) {
    w = solve57();
  } else if (n == 7 && m == 5) {
    w = solve57();
    transpose();
  } else if (n == 6 && m == 6) {
    w = solve66();
  } else if (n == 6 && m == 7) {
    w = solve67();
  } else if (n == 7 && m == 6) {
    w = solve67();
    transpose();
  } else if (n == 7 && m == 7) {
    w = solve77();
  } else if (m >= 8 && n >= 6) {
    w = solve6(n, m);
  } else if (n >= 8 && m >= 6) {
    w = solve6(m, n);
    transpose();
  } else {
    out("No");
    return;
  }

  if (w.size() != n * m)
    panic();
  vec2d<char> result = newVec2d(2 * n - 1, 2 * m - 1, ' ');

  auto print = [&] () {
    out("Yes");
    for (int i = 0; i < result.size(); i++) {
      for (int j = 0; j < result[i].size(); j++)
        out.print(string(1, result[i][j]));
      out("");
    }
  };

  int pd = -1;
  vec<int> dr({0, 0, 1, -1, 1, 1, -1, -1});
  vec<int> dc({1, -1, 0, 0, 1, -1, 1, -1});
  vec<char> ds({'-', '-', '|', '|', '\\', '/', '/', '\\'});
  for (int i = 1; i <= n * m; i++) {
    pair<int, int> from = w[i - 1];
    pair<int, int> to = w[i % (n * m)];
    if (to.first < 0 || to.first >= n || to.second < 0 || to.second >= m)
      panic();
    int r = 2 * to.first;
    int c = 2 * to.second;
    if (result[r][c] != ' ') {
      print();
      out(to.first, to.second);
      panic();
    }
    result[r][c] = 'o';

    int fd = -1;
    for (int d = 0; d < 8; d++) {
      if (to.first + dr[d] == from.first && to.second + dc[d] == from.second)
        fd = d;
    }
    if (fd == -1 || fd == pd) {
      print();
      out(to.first, to.second);
      panic();
    }
    pd = fd;
    if (result[r + dr[fd]][c + dc[fd]] != ' ')
      panic();
    result[r + dr[fd]][c + dc[fd]] = ds[fd];
  }

  print();
}
