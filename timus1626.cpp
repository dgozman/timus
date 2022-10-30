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

int dot(int x1, int y1, int x2, int y2) {
  return x1 * y2 - x2 * y1;
}

int cross(int x1, int y1, int x2, int y2) {
  return x1 * x2 + y1 * y2;
}

int dist(int x1, int y1, int x2, int y2) {
  return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
}

bool inside(int x1, int y1, int x2, int y2, int x, int y) {
  int v = dot(x1, y1, x2, y2);
  if (v >= 0) {
    return dot(x1, y1, x, y) >= 0 && dot(x2, y2, x, y) >= 0;
  } else {
    return !(dot(x1, y1, x, y) < 0 && dot(x2, y2, x, y) < 0);
  }
}

void solve(Input& in, Output& out) {
  int n = in.ni();
  vec<int> x(n * 2), y(n * 2);
  for (int i = 0; i < n; i++) {
    x[i] = in.ni();
    y[i] = in.ni();
  }
  int xs = in.ni();
  int ys = in.ni();
  int xt = in.ni();
  int yt = in.ni();

  auto area = [&] () {
    int a = 0;
    for (int i = 0; i < n; i++)
      a += x[i] * y[i + 1] - x[i + 1] * y[i];
    return a;
  };

  bool reversed = area() < 0;
  if (reversed) {
    for (int i = 0; i < n - 1 - i; i++) {
      int tmp = x[i]; x[i] = x[n - 1 - i]; x[n - 1 - i] = tmp;
      tmp = y[i]; y[i] = y[n - 1 - i]; y[n - 1 - i] = tmp;
    }
  }

  for (int i = 0; i < n; i++) {
    x[n + i] = x[i];
    y[n + i] = y[i];
  }

  int pow2 = 64;

  int m = (n + pow2 - 1) / pow2;
  u64 one = 1;
  vec2d<u64> to_right = newVec2d(n, m, (u64) 0);
  vec2d<u64> in_right = newVec2d(n, m, (u64) 0);

  vec<int> index(n);
  for (int i = 0; i < n; i++)
    index[i] = i;

  for (int i = 0; i < n; i++) {
    int pi = (i + n - 1) % n;
    auto is_up = [&] (int j) {
      return (y[j] - y[i] > 0) || (y[j] - y[i] == 0 && x[j] - x[i] >= 0);
    };

    auto start = [&] (int j) {
      int v = dot(x[j] - x[i], y[j] - y[i], x[j + 1] - x[i], y[j + 1] - y[i]);
      if (v)
        return v > 0 ? j : (j + 1) % n;
      return dist(x[i], y[i], x[j], y[j]) < dist(x[i], y[i], x[j + 1], y[j + 1]) ? j : (j + 1) % n;
    };

    sort(index.begin(), index.end(), [&] (int j, int k) -> bool {
      bool up_j = is_up(j);
      bool up_k = is_up(k);
      if (up_j != up_k)
        return up_j;
      int v = dot(x[j] - x[i], y[j] - y[i], x[k] - x[i], y[k] - y[i]);
      if (v)
        return v > 0;
      return dist(x[i], y[i], x[j], y[j]) < dist(x[i], y[i], x[k], y[k]);
    });

    if (index[0] != i)
      panic();

    int cur = index[1];
    auto side_dist = [&] (int j) {
      int a1 = y[i] - y[cur];
      int b1 = x[cur] - x[i];
      int c1 = -(a1 * x[i] + b1 * y[i]);
      int a2 = y[j] - y[j + 1];
      int b2 = x[j + 1] - x[j];
      int c2 = -(a2 * x[j] + b2 * y[j]);
      int d = a1 * b2 - a2 * b1;
      if (d == 0) {
        return dist(x[i], y[i], x[j], y[j]) < dist(x[i], y[i], x[j + 1], y[j + 1]) ?
          make_pair(x[j] * 1.0, y[j] * 1.0) : make_pair(x[j + 1] * 1.0, y[j + 1] * 1.0);
      }
      double xx = (-c1 * b2 + b1 * c2) * 1.0 / d;
      double yy = (-a1 * c2 + c1 * a2) * 1.0 / d;
      return make_pair(xx, yy);
    };
    auto sides_comp = [&] (int j, int k) {
      if (j == k)
        return false;
      const double eps = 1e-8;
      auto pj = side_dist(j);
      auto pk = side_dist(k);
      double dj = (pj.first - x[i]) * (pj.first - x[i]) + (pj.second - y[i]) * (pj.second - y[i]);
      double dk = (pk.first - x[i]) * (pk.first - x[i]) + (pk.second - y[i]) * (pk.second - y[i]);
      if (dj < dk - eps)
        return true;
      if (dk < dj - eps)
        return false;
      auto pj2 = make_pair(x[j] + x[j + 1] - pj.first, y[j] + y[j + 1] - pj.second);
      auto pk2 = make_pair(x[k] + x[k + 1] - pk.first, y[k] + y[k + 1] - pk.second);
      double v_rel = (pj2.first - pj.first) * (pk2.second - pk.second) - (pk2.first - pk.first) * (pj2.second - pj.second);
      if (abs(v_rel) < eps)
        return j < k;
      double v_or1 = (pj.first - x[i]) * (pj2.second - pj.second) - (pj2.first - pj.first) * (pj.second - y[i]);
      double v_or2 = (pk.first - x[i]) * (pk2.second - pk.second) - (pk2.first - pk.first) * (pk.second - y[i]);
      if (v_or1 * v_or2 < -eps)
        return j < k;
      if (v_or1 >= 0)
        return v_rel < 0;
      return v_rel > 0;
    };
    set<int, decltype(sides_comp)> sides(sides_comp);

    for (int j = 0; j < n; j++) {
      if (j == i || j == pi)
        continue;
      int a = y[j] - y[j + 1];
      int b = x[j + 1] - x[j];
      int c = -(a * x[j] + b * y[j]);
      if (a == 0) {
        if (y[j] == y[i] && x[j] >= x[i])
          sides.insert(j);
      } else {
        if (-c * a >= x[i] * a * a && (y[j] - y[i]) * (y[j + 1] - y[i]) <= 0)
          sides.insert(j);
      }
    }

    int cur_index = 1;
    while (cur_index < n) {
      cur = index[cur_index];

      int next_index = cur_index + 1;
      while (next_index < n) {
        int next = index[next_index];
        if (dot(x[next] - x[i], y[next] - y[i], x[cur] - x[i], y[cur] - y[i]) == 0 &&
            cross(x[next] - x[i], y[next] - y[i], x[cur] - x[i], y[cur] - y[i]) > 0) {
          next_index++;
        } else {
          break;
        }
      }

      for (int ind = cur_index; ind < next_index; ind++) {
        int j = index[ind];
        if (start(j) == j && (j != i && j != pi))
          sides.insert(j);
        int pj = (j + n - 1) % n;
        if (start(pj) == j && (pj != i && pj != pi))
          sides.insert(pj);
      }

      int first = sides.begin() == sides.end() ? -1 : *(sides.begin());
      for (int ind = cur_index; ind < next_index; ind++) {
        int j = index[ind];
        if (first == -1 || first == j || (first + 1) % n == j) {
          if (inside(x[i] - x[pi], y[i] - y[pi], x[i + 1] - x[i], y[i + 1] - y[i], x[j] - x[i], y[j] - y[i])) {
//            out(i, j);
            if (dot(x[j] - x[i], y[j] - y[i], xs - x[i], ys - y[i]) > 0 &&
                dot(x[j] - x[i], y[j] - y[i], xt - x[i], yt - y[i]) > 0) {
              to_right[i][j / pow2] |= one << (j % pow2);
              in_right[j][i / pow2] |= one << (i % pow2);
            }
          }
        }
      }

      for (int ind = cur_index; ind < next_index; ind++) {
        int j = index[ind];
        if (start(j) != j && (j != i && j != pi))
          sides.erase(j);
        int pj = (j + n - 1) % n;
        if (start(pj) != j && (pj != i && pj != pi))
          sides.erase(pj);
      }

      cur_index = next_index;
    }
  }

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      u64 bit = (to_right[i][j / pow2] >> (j % pow2)) & 1;
      if (!bit)
        continue;
      for (int chunk = 0; chunk < m; chunk++) {
        u64 both = to_right[j][chunk] & in_right[i][chunk];
        if (!both)
          continue;

        for (int k_rem = 0; k_rem < pow2; k_rem++) {
          if ((both >> k_rem) & 1) {
            int k = chunk * pow2 + k_rem;

            if (reversed) {
              i = n - 1 - i;
              j = n - 1 - j;
              k = n - 1 - k;
            }
            out(i + 1, j + 1, k + 1);
//            out(i, j, k);
            return;
          }
        }
        panic();
      }
    }
  }

  out("Impossible");
}
