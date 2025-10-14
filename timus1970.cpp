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
template<typename T> using vec4d = vec<vec3d<T>>;
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

template<typename T> vec4d<T> newVec4d(int n, int m, int k, int l, const T& init) {
  vec4d<T> v(n, newVec3d(m, k, l, init));
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
  auto w = newVec4d<int>(9, 4, 4, 4, 0);
  for (int d = 0; d < 9; d++) {
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++)
        w[d][0][i][j] = in.ni();
    }
    for (int r = 1; r < 4; r++) {
      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++)
          w[d][r][i][j] = w[d][r - 1][j][3 - i];
      }
    }
  }

  // row,col => top-left is 0, 0

  int tmp = 0;
  auto perm = newVec2d<int>(120, 5, -1);
  for (int a = 0; a < 5; a++) {
    for (int b = 0; b < 5; b++) {
      for (int c = 0; c < 5; c++) {
        for (int d = 0; d < 5; d++) {
          for (int e = 0; e < 5; e++) {
            if (a != b && a != c && a != d && a != e &&
                b != c && b != d && b != e &&
                c != d && c != e &&
                d != e) {
              perm[tmp][0] = a;
              perm[tmp][1] = b;
              perm[tmp][2] = c;
              perm[tmp][3] = d;
              perm[tmp][4] = e;
              tmp++;
            }
          }
        }
      }
    }
  }

  auto result = newVec2d<int>(10, 10, -1);
  auto place = [&](vec2d<int>& face, int row, int col) {
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        if (result[row + i][col + j] != -1 && result[row + i][col + j] != face[i][j])
          panic();
        result[row + i][col + j] = face[i][j];
      }
    }
  };

  auto same_row = [&](vec2d<int>& a, int a_row, vec2d<int>& b, int b_row) {
    for (int j = 0; j < 4; j++) {
      if (a[a_row][j] != b[b_row][j])
        return false;
    }
    return true;
  };

  auto same_col = [&](vec2d<int>& a, int a_col, vec2d<int>& b, int b_col) {
    for (int i = 0; i < 4; i++) {
      if (a[i][a_col] != b[i][b_col])
        return false;
    }
    return true;
  };

  vec<int> five(5);
  auto can_place = newVec2d<int>(5, 5, -1);

  for (int left_index = 0; left_index < 9; left_index++) {
    for (int left_dir = 0; left_dir < 4; left_dir++) {
      auto& left = w[left_index][left_dir];
      for (int up_index = 0; up_index < 9; up_index++) {
        if (up_index == left_index)
          continue;
        for (int up_dir = 0; up_dir < 4; up_dir++) {
          auto& up = w[up_index][up_dir];
          if (left[0][3] != up[3][0])
            continue;
          for (int right_index = 0; right_index < 9; right_index++) {
            if (right_index == left_index || right_index == up_index)
              continue;
            for (int right_dir = 0; right_dir < 4; right_dir++) {
              auto& right = w[right_index][right_dir];
              if (right[0][0] != up[3][3])
                continue;
              for (int down_index = 0; down_index < 9; down_index++) {
                if (down_index == left_index || down_index == up_index || down_index == right_index)
                  continue;
                for (int down_dir = 0; down_dir < 4; down_dir++) {
                  auto& down = w[down_index][down_dir];
                  if (down[0][0] != left[3][3] || down[0][3] != right[3][0])
                    continue;

                  tmp = 0;
                  for (int i = 0; i < 9; i++) {
                    if (i == left_index || i == up_index || i == right_index || i == down_index)
                      continue;
                    five[tmp++] = i;
                  }

                  // out("trying", five);
                  // out("up", up);

                  for (int i = 0; i < 5; i++)
                    for (int j = 0; j < 5; j++)
                      can_place[i][j] = -1;

                  for (int i = 0; i < 5; i++) {
                    for (int d = 0; d < 4; d++) {
                      auto& face = w[five[i]][d];
                      // out("face", i, d, face);
                      if (can_place[i][0] == -1 && same_row(left, 0, face, 3) && same_col(up, 0, face, 3))
                        can_place[i][0] = d;
                      if (can_place[i][1] == -1 && same_row(right, 0, face, 3) && same_col(up, 3, face, 0))
                        can_place[i][1] = d;
                      if (can_place[i][2] == -1 && same_col(left, 3, face, 0) && same_row(up, 3, face, 0) && same_col(right, 0, face, 3) && same_row(down, 0, face, 3))
                        can_place[i][2] = d;
                      if (can_place[i][3] == -1 && same_row(left, 3, face, 0) && same_col(down, 0, face, 3))
                        can_place[i][3] = d;
                      if (can_place[i][4] == -1 && same_row(right, 3, face, 0) && same_col(down, 3, face, 0))
                        can_place[i][4] = d;
                    }
                  }
                  // out("can_place", can_place);

                  for (auto& p : perm) {
                    bool ok = true;
                    for (int i = 0; i < 5; i++) {
                      if (can_place[p[i]][i] == -1) {
                        ok = false;
                        break;
                      }
                    }
                    if (ok) {
                      // out("found p", p);
                      // out("left", left);
                      // out("up", up);
                      // out("right", right);
                      // out("down", down);
                      place(left, 3, 0);
                      place(up, 0, 3);
                      place(right, 3, 6);
                      place(down, 6, 3);
                      // out("good here");
                      auto real = newVec3d<int>(5, 4, 4, 0);
                      for (int i = 0; i < 5; i++)
                        real[i] = w[five[p[i]]][can_place[p[i]][i]];
                      // out("before 0"); out(result, real[0]);
                      place(real[0], 0, 0);
                      // out("before 1"); out(result, real[1]);
                      place(real[1], 0, 6);
                      // out("before 2"); out(result, real[2]);
                      place(real[2], 3, 3);
                      // out("before 3"); out(result, real[3]);
                      place(real[3], 6, 0);
                      // out("before 4"); out(result, real[4]);
                      place(real[4], 6, 6);
                      out(result);
                      return;
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }

  panic();
}
