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
  int UNKNOWN = 1000;
  int n = in.ni();
  array<array<int, 4>, 4> w = {{
    {UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN},
    {UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN},
    {UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN},
    {UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN},
  }};
  array<int, 4> score = {0, 0, 0, 0};
  array<int, 4> total = {0, 0, 0, 0};
  for (int i = 0; i < n; i++) {
    int a = in.ni() - 1;
    int b = in.ni() - 1;
    int delta = in.ni() - in.ni();
    w[a][b] = delta;
    total[a] += delta;
    total[b] += -delta;
    score[a] += (delta > 0) ? 3 : (delta < 0) ? 0 : 1;
    score[b] += (delta > 0) ? 0 : (delta < 0) ? 3 : 1;
  }

  array<array<int, 4>, 24> perms = {{
    {0, 1, 2, 3},
    {0, 1, 3, 2},
    {0, 2, 1, 3},
    {0, 2, 3, 1},
    {0, 3, 1, 2},
    {0, 3, 2, 1},
    {1, 0, 2, 3},
    {1, 0, 3, 2},
    {1, 2, 0, 3},
    {1, 2, 3, 0},
    {1, 3, 0, 2},
    {1, 3, 2, 0},
    {2, 0, 1, 3},
    {2, 0, 3, 1},
    {2, 1, 0, 3},
    {2, 1, 3, 0},
    {2, 3, 0, 1},
    {2, 3, 1, 0},
    {3, 0, 1, 2},
    {3, 0, 2, 1},
    {3, 1, 0, 2},
    {3, 1, 2, 0},
    {3, 2, 0, 1},
    {3, 2, 1, 0},
  }};
  array<bool, 256> can;
  for (int i = 0; i < 256; i++)
    can[i] = false;

  auto encode = [&](const array<int, 4>& p) {
    return p[0] * 64 + p[1] * 16 + p[2] * 4 + p[3];
  };

  array<int, 4> p = {0, 1, 2, 3};
  auto eq = [&](int a, int b) {
    return score[a] == score[b] && total[a] == total[b];
  };
  auto comp = [&](int a, int b) {
    return score[a] > score[b] || (score[a] == score[b] && total[a] > total[b]);
  };

  if (n <= 1) {
    for (int p = 0; p < 24; p++)
      can[encode(perms[p])] = true;
  } else {
    int MAX = n == 2 ? 38 : 100;
    function<void(int)> go = [&](int moves) {
      if (moves == 0) {
        sort(p.begin(), p.end(), comp);
        if (eq(p[0], p[1]) && eq(p[1], p[2]) && eq(p[2], p[3])) {
          for (int p = 0; p < 24; p++)
            can[encode(perms[p])] = true;
        } else if (eq(p[0], p[1]) && eq(p[1], p[2])) {
          int tmp;
          can[encode(p)] = true;  // 1 2 3
          tmp = p[1]; p[1] = p[2]; p[2] = tmp;
          can[encode(p)] = true;  // 1 3 2
          tmp = p[0]; p[0] = p[1]; p[1] = tmp;
          can[encode(p)] = true;  // 2 3 1
          tmp = p[1]; p[1] = p[2]; p[2] = tmp;
          can[encode(p)] = true;  // 2 1 3
          tmp = p[0]; p[0] = p[1]; p[1] = tmp;
          can[encode(p)] = true;  // 3 1 2
          tmp = p[1]; p[1] = p[2]; p[2] = tmp;
          can[encode(p)] = true;  // 3 2 1
        } else if (eq(p[1], p[2]) && eq(p[2], p[3])) {
          int tmp;
          can[encode(p)] = true;  // 1 2 3
          tmp = p[2]; p[2] = p[3]; p[3] = tmp;
          can[encode(p)] = true;  // 1 3 2
          tmp = p[1]; p[1] = p[2]; p[2] = tmp;
          can[encode(p)] = true;  // 2 3 1
          tmp = p[2]; p[2] = p[3]; p[3] = tmp;
          can[encode(p)] = true;  // 2 1 3
          tmp = p[1]; p[1] = p[2]; p[2] = tmp;
          can[encode(p)] = true;  // 3 1 2
          tmp = p[2]; p[2] = p[3]; p[3] = tmp;
          can[encode(p)] = true;  // 3 2 1
        } else if (eq(p[0], p[1]) && eq(p[2], p[3])) {
          int tmp;
          can[encode(p)] = true;  // 1 2 3 4
          tmp = p[0]; p[0] = p[1]; p[1] = tmp;
          can[encode(p)] = true;  // 2 1 3 4
          tmp = p[2]; p[2] = p[3]; p[3] = tmp;
          can[encode(p)] = true;  // 2 1 4 3
          tmp = p[0]; p[0] = p[1]; p[1] = tmp;
          can[encode(p)] = true;  // 1 2 4 3
        } else if (eq(p[0], p[1])) {
          int tmp;
          can[encode(p)] = true;  // 1 2
          tmp = p[0]; p[0] = p[1]; p[1] = tmp;
          can[encode(p)] = true;  // 2 1
        } else if (eq(p[1], p[2])) {
          int tmp;
          can[encode(p)] = true;  // 1 2
          tmp = p[1]; p[1] = p[2]; p[2] = tmp;
          can[encode(p)] = true;  // 2 1
        } else if (eq(p[2], p[3])) {
          int tmp;
          can[encode(p)] = true;  // 1 2
          tmp = p[2]; p[2] = p[3]; p[3] = tmp;
          can[encode(p)] = true;  // 2 1
        } else {
          can[encode(p)] = true;
        }
        return;
      }

      for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j < 4; j++) {
          if (w[i][j] == UNKNOWN) {
            for (int delta = -MAX; delta <= MAX; delta++) {
              w[i][j] = delta;
              total[i] += delta;
              total[j] += -delta;
              score[i] += (delta > 0) ? 3 : (delta < 0) ? 0 : 1;
              score[j] += (delta > 0) ? 0 : (delta < 0) ? 3 : 1;
              // if (total[i] >= -50 && total[i] <= 50 && total[j] >= -50 && total[j] <= 50)
              go(moves - 1);
              w[i][j] = UNKNOWN;
              total[i] -= delta;
              total[j] -= -delta;
              score[i] -= (delta > 0) ? 3 : (delta < 0) ? 0 : 1;
              score[j] -= (delta > 0) ? 0 : (delta < 0) ? 3 : 1;
            }
            return;
          }
        }
      }
    };
    go(6 - n);
  }

  int result = 0;
  for (int p = 0; p < 24; p++) {
    if (can[encode(perms[p])])
      result++;
  }
  out.println(result);
  for (int p = 0; p < 24; p++) {
    if (can[encode(perms[p])]) {
      for (int i = 0; i < 4; i++) {
        if (i)
          out.print(' ');
        out.print(perms[p][i] + 1);
      }
      out.println();
    }
  }
}
