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
  int n = in.ni();
  vec<i64> px(n * 2), py(n * 2);
  for (int i = 0; i < n; i++) {
    px[i] = in.ni();
    py[i] = in.ni();
  }

  i64 xs = in.ni();
  i64 ys = in.ni();
  i64 xf = in.ni();
  i64 yf = in.ni();

  auto area = [&] () {
    i64 a = 0;
    for (int i = 0; i < n; i++)
      a += px[i] * py[i + 1] - px[i + 1] * py[i];
    return a;
  };

  auto cross4 = [&](i64 x1, i64 y1, i64 x2, i64 y2) {
    return x1 * y2 - x2 * y1;
  };

  bool reversed = area() < 0;
  if (reversed) {
    // out ("reversed");
    for (int i = 0; i < n - 1 - i; i++) {
      i64 tmp = px[i]; px[i] = px[n - 1 - i]; px[n - 1 - i] = tmp;
      tmp = py[i]; py[i] = py[n - 1 - i]; py[n - 1 - i] = tmp;
    }
  }

  for (int i = 0; i < n; i++) {
    px[n + i] = px[i];
    py[n + i] = py[i];
  }

  int pow2 = 64;
  int m = (n + pow2 - 1) / pow2;
  u64 one = 1;
  // both ends of the segment are counter-clockwise when rotating j against i
  vec2d<u64> mask_ccw = newVec2d(n, m, (u64)0);
  // both ends of the segment are clockwise when rotating j against i
  vec2d<u64> mask_cw = newVec2d(n, m, (u64)0);

  auto maybe_mark = [&](int i, int j) {
    // out ("====== maybe_mark", i, j);
    i64 dot1 = (px[j] - px[i]) * (ys - py[i]) - (xs - px[i]) * (py[j] - py[i]);
    i64 dot2 = (px[j] - px[i]) * (yf - py[i]) - (xf - px[i]) * (py[j] - py[i]);
    if (dot1 > 0 && dot2 > 0) {
      // out ("mark", i, j);
      mask_ccw[i][j / pow2] |= one << (j % pow2);
      mask_cw[j][i / pow2] |= one << (i % pow2);
    }
  };

  vec<int> p(n);
  for (int i = 0; i < n; i++)
    p[i] = i;
  vec<i64> x(2 * n), y(2 * n);

  for (int c = 0; c < n; c++) {
    for (int i = 0; i < 2 * n; i++) {
      x[i] = px[i] - px[c];
      y[i] = py[i] - py[c];
    }

    // sort by polar angle relative to the ray c->Oxs
    sort(p.begin(), p.end(), [&](int a, int b) -> bool {
      bool a_top = y[a] > 0 || y[a] == 0 && x[a] >= 0;
      bool b_top = y[b] > 0 || y[b] == 0 && x[b] >= 0;
      if (a_top && !b_top)
        return true;
      if (!a_top && b_top)
        return false;
      i64 v = x[a] * y[b] - y[a] * x[b];
      if (v == 0) {
        i64 da = x[a] * x[a] + y[a] * y[a];
        i64 db = x[b] * x[b] + y[b] * y[b];
        return da < db;
      }
      return v > 0;
    });

    if (p[0] != c)
      panic();

    // out ("c", c, "sorted", p);

    auto cross = [&](int i, int j) {
      return x[i] * y[j] - x[j] * y[i];
    };

    auto dot = [&](int i, int j) {
      return x[i] * x[j] + y[i] * y[j];
    };

    auto norm2 = [&](int i) {
      return x[i] * x[i] + y[i] * y[i];
    };

    auto same_ang = [&](int i, int j) {
      // two points are on the same ray starting at "c"
      return cross(i, j) == 0 && dot(i, j) >= 0;
    };

    int cur = p[1];
    auto sides_comp = [&](int i, int j) {
      // compare two sides by their intersection point with the ray towards "cur"
      if (i == j)
        return false;

      i64 a1 = y[i] - y[i + 1];
      i64 b1 = x[i + 1] - x[i];
      i64 c1 = -(a1 * x[i] + b1 * y[i]);
      i64 d1 = a1 * x[cur] + b1 * y[cur];
      if (d1 < 0) {
        a1 = -a1;
        b1 = -b1;
        c1 = -c1;
        d1 = -d1;
      }

      i64 a2 = y[j] - y[j + 1];
      i64 b2 = x[j + 1] - x[j];
      i64 c2 = -(a2 * x[j] + b2 * y[j]);
      i64 d2 = a2 * x[cur] + b2 * y[cur];
      if (d2 < 0) {
        a2 = -a2;
        b2 = -b2;
        c2 = -c2;
        d2 = -d2;
      }

      // ray is: k * x[cur], k * y[cur], k >= 0
      // a1 * k * x[cur] + b1 * k * y[cur] + c1 = 0
      // k1 = -c1 / (a1 * x[cur] + b1 * y[cur])
      // k2 = -c2 / (a2 * x[cur] + b2 * y[cur])
      // k1 < k2    <->    -c1/d1 < -c2/d2    <->    c1*d2 > c2*d1

      // one intersection is closer to the origin
      if (c1 * d2 != c2 * d1)
        return c1 * d2 > c2 * d1;

      // pick the closest end on the 0->cur ray for each side
      int i_closest = i;
      if (d1 == 0)
        i_closest = norm2(i) < norm2(i + 1) ? i : i + 1;
      else
        i_closest = same_ang(i, cur) ? i : i + 1;
      int j_closest = j;
      if (d2 == 0)
        j_closest = norm2(j) < norm2(j + 1) ? j : j + 1;
      else
        j_closest = same_ang(j, cur) ? j : j + 1;

      // perhaps one of them is closer to the origin
      i64 norm_dist = norm2(i_closest) - norm2(j_closest);
      if (norm_dist != 0)
        return norm_dist < 0;

      // when one belongs to the ray "0->cur", the order does not matter as long as it is consistent
      if (d1 == 0 || d2 == 0)
        return i < j;

      int i_furthest = i + i + 1 - i_closest;
      int j_furthest = j + j + 1 - j_closest;
      // intersecting at the same point, but to the opposite sides of the ray "0->cur"
      // the order does not matter as long as it is consistent
      i64 v1 = cross(cur, i_furthest);
      i64 v2 = cross(cur, j_furthest);
      if (v1 == 0 || v2 == 0)
        panic();
      if ((v1 < 0 && v2 > 0) || (v1 > 0 && v2 < 0))
        return i < j;

      // intersecting at the same point, to the smae side of the ray "0->cur"
      // choose the side that goes closer to origin
      i64 v = cross4(x[j_furthest] - x[j_closest], y[j_furthest] - y[j_closest], x[i_furthest] - x[i_closest], y[i_furthest] - y[i_closest]);
      if (v1 > 0 && v2 > 0)
        return v > 0;
      if (v1 < 0 && v2 < 0)
        return v < 0;
      panic();
      return false;
    };
    // keep sides (i -> i+1) in the set, so that start<=cur, end>=cur
    set<int, decltype(sides_comp)> sides(sides_comp);

    auto is_banned = [&](int i) {
      return i == c || i == c - 1 || i == c + n - 1;
    };

    auto intersects_cur = [&](int i) {
      // first check that cur is between i and i+1
      bool between1 = cross(i, cur) >= 0 && cross(cur, i + 1) >= 0;
      bool between2 = cross(i + 1, cur) >= 0 && cross(cur, i) >= 0;
      if (!between1 && !between2)
        return false;

      // ray is: k * x[cur], k * y[cur], k >= 0
      // a * k * x[cur] + b * k * y[cur] + c = 0
      // k = -c / (a * x[cur] + b * y[cur])
      i64 a = y[i] - y[i + 1];
      i64 b = x[i + 1] - x[i];
      i64 c = -(a * x[i] + b * y[i]);
      i64 d = a * x[cur] + b * y[cur];
      if (d != 0)
        return (-c * d) >= 0;
      if (c != 0)
        return false;
      return same_ang(i, cur) || same_ang(i + 1, cur);
    };

    for (int i = 0; i < n; i++) {
      // start with all sides that intersect the ray (c -> Ox)
      if (intersects_cur(i) && !is_banned(i)) {
        // out ("inserting initial", i);
        sides.insert(i);
      }
    }

    auto check_for_addition = [&](int same_angle, int adjacent) {
      // out ("check for add", same_angle, adjacent, cross(same_angle, adjacent));
      // add all sides that start at "cur" angle and go counter-clockwise
      if (cross(same_angle, adjacent) >= 0) {
        int start = min(same_angle, adjacent);
        if (!is_banned(start)) {
          // out ("inserting", start);
          sides.insert(start);
        }
      }
    };

    auto check_for_removal = [&](int same_angle, int adjacent) {
      // remove all sides that go counter-clockwise and end at "cur" angle
      if (cross(adjacent, same_angle) >= 0) {
        // out ("removing", min(same_angle, adjacent));
        sides.erase(min(same_angle, adjacent));
        // out ("after removing", min(same_angle, adjacent), "size=", sides.size());
      }
    };

    auto lies_inside = [&](int i) {
      // if c->i lies inside the angle between (c-1 -> c) and (c -> c+1), it goes inside the polygon
      int v = cross(c + 1, c + n - 1);
      // out ("lies_inside", "c", c, "i", i, "v", v);
      if (v >= 0)
        return cross(c + 1, i) >= 0 && cross(i, c + n - 1) >= 0;
      return !(cross(c + 1, i) < 0 && cross(i, c + n - 1) < 0);
    };

    for (int index = 1; index < n;) {
      cur = p[index];
      // out ("switching cur to", cur);

      int last = index;
      while (last < n && same_ang(cur, p[last])) {
        int prev = (p[last] - 1 + n) % n;
        check_for_addition(prev + 1, prev);
        check_for_addition(p[last], p[last] + 1);
        last++;
      }

      // out ("sides.size", sides.size(), "cur", cur);
      if (sides.empty())
        panic();
      // c->cur has no intersections when "cur" is the end of the closest side in the set
      int first = *sides.begin();
      bool is_closest = (first % n) == (cur % n) || ((first + 1) % n) == (cur % n);
      // out ("cur", cur, "first", first, "is_closest", is_closest);
      if (is_closest && lies_inside(cur))
        maybe_mark(c, cur);

      last = index;
      while (last < n && same_ang(cur, p[last])) {
        int prev = (p[last] - 1 + n) % n;
        check_for_removal(prev + 1, prev);
        check_for_removal(p[last], p[last] + 1);
        last++;
      }

      // out ("sides.size after removal", sides.size(), "cur", cur);

      index = last;
    }
  }

  auto area3 = [&](i64 x1, i64 y1, i64 x2, i64 y2, i64 x3, i64 y3) {
    return abs(x1 * y2 + x2 * y3 + x3 * y1 - x1 * y3 - x2 * y1 - x3 * y2);
  };

  auto iniside_triangle = [&](int i, int j, int k, i64 xs, i64 ys) {
    i64 total = area3(px[i], py[i], px[j], py[j], px[k], py[k]);
    i64 parts = area3(xs, ys, px[j], py[j], px[k], py[k])
      + area3(px[i], py[i], xs, ys, px[k], py[k])
      + area3(px[i], py[i], px[j], py[j], xs, ys);
    return total == parts;
  };

  auto found = [&](int i, int j, int k) {
    if (!iniside_triangle(i, j, k, xs, ys))
      panic();
    if (!iniside_triangle(i, j, k, xf, yf))
      panic();

    if (reversed) {
      i = n - 1 - i;
      j = n - 1 - j;
      k = n - 1 - k;
    }
    vec<int> result = { i + 1, j + 1, k + 1 };
    sort(result.begin(), result.end());
    out(result);
  };

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (!((mask_ccw[i][j / pow2] >> (j % pow2)) & one))
        continue;
      for (int chunk = 0; chunk < m; chunk++) {
        u64 both = mask_ccw[j][chunk] & mask_cw[i][chunk];
        if (!both)
          continue;

        for (int k_rem = 0; k_rem < pow2; k_rem++) {
          if ((both >> k_rem) & one) {
            int k = chunk * pow2 + k_rem;
            found(i, j, k);
            return;
          }
        }
        panic();
      }
    }
  }

  out("Impossible");
}

/*


16
8 0
8 1
1 1
1 7
7 7
7 3
3 3
3 4
6 4
6 6
2 6
2 2
8 2
8 8
0 8
0 0
3 5 4 5
====
8 10 11

16
0 0
0 8
8 8
8 2
2 2
2 6
6 6
6 4
3 4
3 3
7 3
7 7
1 7
1 1
8 1
8 0
3 5 5 5
====
Impossible


6
0 0
500 -10000
1000 0
10000 1000
500 500
-10000 1000
600 400 9000 900
===
Impossible

6
0 0
500 -10000
1000 0
10000 1000
500 500
-10000 1000
600 401 9000 900
====
3 4 5

6
500 -10000
1000 0
10000 1000
500 500
-10000 1000
0 0
2 1 998 1
====
2 4 6


4
-10000 -10000
-10000 10000
10000 10000
10000 -10000
-9999 -9999 9998 -9999
=====
1 2 4


6
0 0
10 9
20 0
20 20
10 20
0 10
1 2 18 19
===
Impossible

6
0 0
10 9
20 0
20 20
10 20
0 11
1 2 18 19
====
1 4 6

6
0 0
10 9
20 0
20 20
10 20
0 19
1 2 18 19
====
1 4 6

6
0 0
10 9
20 0
20 20
17 20
0 3
1 2 18 19
====
Impossible



6
0 0
10 9
20 0
20 20
18 20
0 2
1 2 18 19
====
Impossible

5
0 0
10 9
20 0
20 20
0 20
1 2 18 19
====
1 4 5


4        3
      f
    1
 s
0        2


5
0 0
10 10
20 0
20 20
0 20
1 2 18 19
====
Impossible



3
0 0
1000 0
2000 2
1002 1 1001 1
====
1 2 3


10
0 0
100 0
200 1
300 3
400 6
500 10
600 15
700 21
800 28
0 100
750 28 760 28
====
1 9 10


5
0 0
100 0
700 21
800 28
0 100
750 28 760 28
====
1 4 5





10
0 0
100 -1
200 1
300 3
400 6
500 10
600 15
700 21
800 28
0 100
100 0 1 99
=====
2 3 10

10
0 0
100 0
200 1
300 3
400 6
500 10
600 15
700 21
800 28
0 100
1 1 1 99
====
1 3 10


4
0 0
100 0
10 10
0 100
3 5 3 7
===
1 3 4

4
0 0
100 0
10 10
0 100
2 1 3 1
===
1 2 3

4
0 0
100 0
10 10
0 100
2 1 1 2
===
Impossible

4
0 0
0 100
100 100
100 0
50 50 50 49
===
Impossible


4
0 0
0 100
100 100
100 0
5 1 6 1
===
1 3 4

3
0 0
4 3
0 3
1 2 2 2
===
1 2 3



*/