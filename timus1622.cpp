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

void gen(Input& in, Output& out) {
  const u8 m = 33;
  const u64 one = 1;
  vec<i8> dr({0, 0, 1, -1});
  vec<i8> dc({1, -1, 0, 0});

  auto is_good = [&] (i8 r, i8 c) {
    if (r < 0 || r >= 7 || c < 0 || c >= 7)
      return false;
    if (r < 2 && c < 2)
      return false;
    if (r < 2 && c > 4)
      return false;
    if (r > 4 && c < 2)
      return false;
    if (r > 4 && c > 4)
      return false;
    return true;
  };

  vec2d<u8> move = newVec2d(m, 4, (u8) 0);
  vec2d<u8> over = newVec2d(m, 4, (u8) 0);
  vec<u8> row;
  vec<u8> col;
  vec2d<u8> id = newVec2d(7, 7, (u8) 0);

  for (i8 r = 0; r < 7; r++) {
    for (i8 c = 0; c < 7; c++) {
      if (!is_good(r, c))
        continue;
      id[r][c] = row.size();
      row.push_back(r);
      col.push_back(c);
    }
  }

  for (i8 r = 0; r < 7; r++) {
    for (i8 c = 0; c < 7; c++) {
      if (!is_good(r, c))
        continue;
      for (i8 dir = 0; dir < 4; dir++) {
        i8 nr = r + dr[dir] * 2;
        i8 nc = c + dc[dir] * 2;
        if (!is_good(nr, nc)) {
          move[id[r][c]][dir] = m;
          continue;
        }
        move[id[r][c]][dir] = id[nr][nc];
        over[id[r][c]][dir] = id[r + dr[dir]][c + dc[dir]];
      }
    }
  }

  auto to_chess = [&] (u8 i) {
    return string(1, 'A' + row[i]) + string(1, '1' + col[i]);
  };

  vec<bool> visited(one << m);
  std::array<u64, m> move_from;
  std::array<u64, m> move_to;
  u8 move_count = 0;
  vec<string> result(m);

  int iter = 0;

  function<void(u64)> go = [&] (u64 state) {
    if (visited[state])
      return;
    visited[state] = true;

    if (move_count >= 16) {
      iter++;
      if (!(iter % 10000))
        out("hi", iter);
    }

    if (move_count == m - 2) {
      for (u8 i = 0; i < m; i++) {
        if ((state >> i) & 1) {
          if (!result[i].length()) {
            for (i8 j = 0; j < move_count; j++)
              result[i] += to_chess(move_from[j]) + "-" + to_chess(move_to[j]) + " ";
            out(to_chess(i), "->", result[i]);
          }
          return;
        }
      }
      panic();
    }

    for (u8 i = 0; i < m; i++) {
      if ((state >> i) & 1) {
        for (u8 dir = 0; dir < 4; dir++) {
          if (move[i][dir] == m)
            continue;
          u8 j = move[i][dir];
          u8 k = over[i][dir];
          if ((state >> j) & 1)
            continue;
          if (!((state >> k) & 1))
            continue;

          if (j >= m)
            panic();
          u64 nstate = (state & (~(one << i)) & (~(one << k))) | (one << j);
          move_from[move_count] = i;
          move_to[move_count] = j;
          move_count++;
          go(nstate);
          move_count--;
        }
      }
    }
  };

  go((one << m) - 1 - (one << 16));

  out("=====");
  for (u8 i = 0; i < m; i++)
    out(to_chess(i), "->", result[i].length() ? result[i] : string("Impossible"));
}

void solve(Input& in, Output& out) {
  // gen(in, out);

  map<string, vec<string>> answer({
    { "A3", { "Impossible" } },
    { "A4", { "B4-D4", "C2-C4", "A3-C3", "A5-A3", "C4-C2", "C1-C3", "C6-C4", "C4-C2", "E1-C1", "C1-C3", "D3-B3", "A3-C3", "E2-C2", "C2-C4", "D4-B4", "E4-E2", "E5-C5", "B5-D5", "D6-D4", "E7-E5", "C7-E7", "G3-E3", "E2-E4", "E4-E6", "E7-E5", "G4-E4", "E4-E6", "G5-E5", "E6-E4", "E4-C4", "C4-A4" } },
    { "A5", { "Impossible" } },
    { "B3", { "Impossible" } },
    { "B4", { "Impossible" } },
    { "B5", { "Impossible" } },
    { "C1", { "Impossible" } },
    { "C2", { "Impossible" } },
    { "C3", { "Impossible" } },
    { "C4", { "Impossible" } },
    { "C5", { "Impossible" } },
    { "C6", { "Impossible" } },
    { "C7", { "Impossible" } },
    { "D1", { "B4-D4", "C2-C4", "A3-C3", "A5-A3", "C4-C2", "C1-C3", "C6-C4", "C4-C2", "E1-C1", "C1-C3", "D3-B3", "A3-C3", "D5-D3", "D7-D5", "E2-C2", "C2-C4", "E5-C5", "C4-C6", "C7-C5", "B5-D5", "E7-E5", "E4-E6", "G5-E5", "D5-F5", "G3-G5", "G5-E5", "E6-E4", "F4-D4", "D4-D2", "F3-D3", "D3-D1" } },
    { "D2", { "Impossible" } },
    { "D3", { "Impossible" } },
    { "D4", { "B4-D4", "C2-C4", "A3-C3", "A5-A3", "C4-C2", "C1-C3", "C5-A5", "C7-C5", "D3-B3", "A3-C3", "D1-D3", "D3-B3", "D5-D3", "D7-D5", "D5-B5", "A5-C5", "E3-C3", "B3-D3", "E1-E3", "E4-E2", "F5-D5", "E7-E5", "G3-E3", "D3-F3", "G5-G3", "G3-E3", "E2-E4", "E4-E6", "C5-E5", "E6-E4", "F4-D4" } },
    { "D5", { "Impossible" } },
    { "D6", { "Impossible" } },
    { "D7", { "B4-D4", "C2-C4", "A3-C3", "A5-A3", "C4-C2", "C1-C3", "C6-C4", "C3-C5", "E1-C1", "E2-C2", "C1-C3", "D3-B3", "A3-C3", "D5-D3", "B5-D5", "E4-E2", "C3-E3", "E2-E4", "E5-E3", "E7-E5", "C7-E7", "G4-E4", "E4-E2", "G3-E3", "E2-E4", "E4-E6", "G5-E5", "D5-F5", "E7-E5", "F5-D5", "D5-D7" } },
    { "E1", { "Impossible" } },
    { "E2", { "Impossible" } },
    { "E3", { "Impossible" } },
    { "E4", { "Impossible" } },
    { "E5", { "Impossible" } },
    { "E6", { "Impossible" } },
    { "E7", { "Impossible" } },
    { "F3", { "Impossible" } },
    { "F4", { "Impossible" } },
    { "F5", { "Impossible" } },
    { "G3", { "Impossible" } },
    { "G4", { "B4-D4", "C2-C4", "A3-C3", "A5-A3", "C4-C2", "C1-C3", "C5-A5", "C7-C5", "D3-B3", "A3-C3", "D1-D3", "D3-B3", "D5-D3", "D7-D5", "D5-B5", "A5-C5", "E3-C3", "B3-D3", "E1-E3", "E4-E2", "F5-D5", "E7-E5", "G3-E3", "D3-F3", "G5-G3", "G3-E3", "E2-E4", "E4-E6", "C5-E5", "E6-E4", "E4-G4" } },
    { "G5", { "Impossible" } }
  });

  string s = in.ns();
  for (string r : answer[s])
    out(r);
}
