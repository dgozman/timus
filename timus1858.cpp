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
  int kBottom = 0;
  int kTop = 1;
  int kRight = 2;
  int kLeft = 3;
  int kFront = 4;
  int kBack = 5;
  // rotations[x][i] => which original side will be at side [i]
  vector<vector<int>> rotations = {
    { kBottom, kTop, kRight, kLeft, kFront, kBack },
    { kBottom, kTop, kBack, kFront, kRight, kLeft },
    { kBottom, kTop, kLeft, kRight, kBack, kFront },
    { kBottom, kTop, kFront, kBack, kLeft, kRight },
    { kTop, kBottom, kRight, kLeft, kBack, kFront },
    { kTop, kBottom, kFront, kBack, kRight, kLeft },
    { kTop, kBottom, kLeft, kRight, kFront, kBack },
    { kTop, kBottom, kBack, kFront, kLeft, kRight },
    { kLeft, kRight, kBottom, kTop, kFront, kBack },
    { kLeft, kRight, kBack, kFront, kBottom, kTop },
    { kLeft, kRight, kTop, kBottom, kBack, kFront },
    { kLeft, kRight, kFront, kBack, kTop, kBottom },
    { kRight, kLeft, kBack, kFront, kTop, kBottom },
    { kRight, kLeft, kBottom, kTop, kBack, kFront },
    { kRight, kLeft, kFront, kBack, kBottom, kTop },
    { kRight, kLeft, kTop, kBottom, kFront, kBack },
    { kFront, kBack, kTop, kBottom, kLeft, kRight },
    { kFront, kBack, kRight, kLeft, kTop, kBottom },
    { kFront, kBack, kBottom, kTop, kRight, kLeft },
    { kFront, kBack, kLeft, kRight, kBottom, kTop },
    { kBack, kFront, kLeft, kRight, kTop, kBottom },
    { kBack, kFront, kBottom, kTop, kLeft, kRight },
    { kBack, kFront, kRight, kLeft, kBottom, kTop },
    { kBack, kFront, kTop, kBottom, kRight, kLeft },
  };

  double L = in.ni();
  int n = in.ni();
  auto m = newVec2d(n * n * n, 6, 0);
  vec<int> place_x(n * n * n, -1), place_y(n * n * n, -1), place_z(n * n * n, -1);
  map<int, pair<int, int>> value_owner;
  auto cube = newVec3d(n, n, n, make_pair(-1, 0));

  int start = -1;
  for (int i = 0; i < n * n * n; i++) {
    int zeroes = 0;
    for (int j = 0; j < 6; j++) {
      int value = in.ni();
      m[i][j] = value;
      if (value != 0) {
        if (value_owner.count(value) == 0)
          value_owner[value].first = i;
        else
          value_owner[value].second = i;
      } else {
        zeroes++;
      }
    }
    if (zeroes == 3)
      start = i;
  }

  auto find_rotation = [&] (int i, int bottom, int front, int left) {
    for (int j = 0; j < rotations.size(); j++) {
      auto& r = rotations[j];
      if (m[i][r[kBottom]] == bottom && m[i][r[kFront]] == front && m[i][r[kLeft]] == left)
        return j;
    }
    panic();
    return -1;
  };

  auto place = [&] (int i, int r, int x, int y, int z) {
    cube[x][y][z] = make_pair(i, r);
    place_x[i] = x;
    place_y[i] = y;
    place_z[i] = z;
  };

  auto get_value = [&] (int x, int y, int z, int side) {
    int c = cube[x][y][z].first;
    auto& r = rotations[cube[x][y][z].second];
    return m[c][r[side]];
  };

  auto find_by_value = [&] (int value) {
    auto p = value_owner[value];
    return place_x[p.first] == -1 ? p.first : p.second;
  };

  for (int x = 0; x < n; x++) {
    for (int y = 0; y < n; y++) {
      for (int z = 0; z < n; z++) {
        int bottom = x == 0 ? 0 : get_value(x - 1, y, z, kTop);
        int front = y == 0 ? 0 : get_value(x, y - 1, z, kBack);
        int left = z == 0 ? 0 : get_value(x, y, z - 1, kRight);
        int index = start;
        if (bottom != 0)
          index = find_by_value(bottom);
        else if (front != 0)
          index = find_by_value(front);
        else if (left != 0)
          index = find_by_value(left);
        place(index, find_rotation(index, bottom, front, left), x, y, z);
      }
    }
  }

  out.setPrecision(10);
  for (int query = in.ni(); query > 0; query--) {
    int a = in.ni() - 1;
    int b = in.ni() - 1;
    double d2 = (place_x[a] - place_x[b]) * (place_x[a] - place_x[b]) +
      (place_y[a] - place_y[b]) * (place_y[a] - place_y[b]) +
      (place_z[a] - place_z[b]) * (place_z[a] - place_z[b]);
    out(sqrt(d2) * L);
  }
}


