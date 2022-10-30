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

using D = float;

D sqr(D x) {
  return x * x;
}

struct Pos {
  int cur;
  D x1, y1, x2, y2, x3, y3;
  Pos() {}
  Pos(int cur, D x1, D y1, D x2, D y2, D x3, D y3):
    cur(cur), x1(x1), y1(y1), x2(x2), y2(y2), x3(x3), y3(y3) {}
  bool operator<(const Pos& h) const {
    return true;
  }
};

void solve(Input& in, Output& out) {
  D a1 = in.next<D>();
  D a2 = in.next<D>();
  D maxa = max(a1, a2);
  D max_move = (a1 + a2) / 2;

  D h = sqrt(sqr(a2) - sqr(a1 / 2));
  D ha1 = h / a1;
  D ha1half = 0.5 + h / a1;

  D s1 = 2 * a1 / (a1 + a2);
  D s2 = 2 * a2 / (a1 + a2);

  D target_x = in.next<D>();
  D target_y = in.next<D>();
  int maxn = in.ni();

  auto get_moves = [&] (const Pos& pos, std::array<pair<D, Pos>, 4>& moves) {
    int move_count = 0;
    if (pos.cur == 0) {
      D x4 = pos.x1 + pos.x3 - pos.x2;
      D y4 = pos.y1 + pos.y3 - pos.y2;
      move_count = 4;
      moves[0].second = Pos(1, pos.x1, pos.y1, pos.x2, pos.y2, pos.x1 * 0.5 + pos.x2 * ha1half - pos.x3 * ha1, pos.y1 * 0.5 + pos.y2 * ha1half - pos.y3 * ha1);
      moves[1].second = Pos(2, pos.x2, pos.y2, pos.x3, pos.y3, pos.x2 * 0.5 + pos.x3 * ha1half - x4 * ha1, pos.y2 * 0.5 + pos.y3 * ha1half - y4 * ha1);
      moves[2].second = Pos(3, pos.x3, pos.y3, x4, y4, pos.x3 * 0.5 + x4 * ha1half - pos.x1 * ha1, pos.y3 * 0.5 + y4 * ha1half - pos.y1 * ha1);
      moves[3].second = Pos(4, x4, y4, pos.x1, pos.y1, x4 * 0.5 + pos.x1 * ha1half - pos.x2 * ha1, y4 * 0.5 + pos.y1 * ha1half - pos.y2 * ha1);
    } else {
      move_count = 3;

      int next = pos.cur + 1;
      if (next == 5)
        next = 1;
      moves[0].second = Pos(next, pos.x2, pos.y2, pos.x2 * s2 + pos.x3 * s1 - pos.x1, pos.y2 * s2 + pos.y3 * s1 - pos.y1, pos.x3, pos.y3);

      int prev = pos.cur - 1;
      if (prev == 0)
        prev = 4;
      moves[1].second = Pos(prev, pos.x1 * s2 + pos.x3 * s1 - pos.x2, pos.y1 * s2 + pos.y3 * s1 - pos.y2, pos.x1, pos.y1, pos.x3, pos.y3);

      if (pos.cur == 1)
        moves[2].second = Pos(0, pos.x1, pos.y1, pos.x2, pos.y2, pos.x2 + pos.y2 - pos.y1, pos.y2 + pos.x1 - pos.x2);
      if (pos.cur == 2)
        moves[2].second = Pos(0, pos.x1 + pos.y2 - pos.y1, pos.y1 + pos.x1 - pos.x2, pos.x1, pos.y1, pos.x2, pos.y2);
      if (pos.cur == 3)
        moves[2].second = Pos(0, pos.x2 + pos.y2 - pos.y1, pos.y2 + pos.x1 - pos.x2, pos.x1 + pos.y2 - pos.y1, pos.y1 + pos.x1 - pos.x2, pos.x1, pos.y1);
      if (pos.cur == 4)
        moves[2].second = Pos(0, pos.x2, pos.y2, pos.x2 + pos.y2 - pos.y1, pos.y2 + pos.x1 - pos.x2, pos.x1 + pos.y2 - pos.y1, pos.y1 + pos.x1 - pos.x2);
    }

    for (int i = 0; i < move_count; i++) {
      D x = (moves[i].second.x1 + moves[i].second.x3) / 2;
      D y = (moves[i].second.y1 + moves[i].second.y3) / 2;
      moves[i].first = sqr(x - target_x) + sqr(y - target_y);
    }
    sort(moves.begin(), moves.begin() + move_count);
    return move_count;
  };

  const int maxm = 16;
  vec<D> dist(maxn + 1, (D) 0.0);
  function<void(int, int, const Pos&)> max_dist = [&] (int prev, int len, const Pos& pos) {
    if (pos.cur == 0) {
      D cur_dist = sqrt(sqr((pos.x1 + pos.x3) / 2) + sqr((pos.y1 + pos.y3) / 2));
      dist[len] = max(dist[len], cur_dist);
    }
    if (len == maxm)
      return;
    std::array<pair<D, Pos>, 4> moves;
    int move_count = get_moves(pos, moves);
    for (int i = 0; i < move_count; i++) {
      if (moves[i].second.cur == prev)
        continue;
      max_dist(pos.cur, len + 1, moves[i].second);
    }
  };
  max_dist(-1, 0, Pos(0, -a1 / 2, a1 / 2, a1 / 2, a1 / 2, a1 / 2, -a1 / 2));

  for (int i = maxm + 1; i <= maxn; i++) {
    dist[i] = dist[i - 1];
    for (int j = 2; j <= i - 2; j++)
      dist[i] = max(dist[i], dist[j] + dist[i - j]);
  }
  out(dist);

  D result = 1e100;
  int result_len = 0;
  std::array<Pos, 33> result_sequence;
  std::array<Pos, 33> sequence;
  const D eps = 1e-4;
  int counter = 0;

  function<void(int)> go = [&] (int len) {
    ++counter;
    if (!(counter % 1000000))
      out(counter);

    Pos& pos = sequence[len];
    if (pos.cur == 0) {
      D cx = (pos.x1 + pos.x3) / 2;
      D cy = (pos.y1 + pos.y3) / 2;
      D d = sqrt(sqr(cx - target_x) + sqr(cy - target_y));
      if (d > result + dist[maxn - len] - eps)
        return;

      if (d < result - eps || (d < result + eps && len < result_len)) {
        out(d);
        result_len = len;
        result = d;
        result_sequence = sequence;
      }
    }
    if (len == maxn)
      return;

    int last0 = len;
    while (sequence[last0].cur != 0)
      last0--;
    int prev0 = last0;
    while (prev0 > 0 && sequence[prev0].cur != 0)
      prev0--;

    std::array<pair<D, Pos>, 4> moves;
    int move_count = get_moves(pos, moves);
    for (int i = 0; i < move_count; i++) {
      if (len - 1 >= 0 && moves[i].second.cur == sequence[len - 1].cur)
        continue;
      // if (moves[i].second.cur == 0 && prev0 < last0 && (len + 1 - last0) < (last0 - prev0))
      //   continue;
      sequence[len + 1] = moves[i].second;
      go(len + 1);
    }
  };

  sequence[0] = Pos(0, -a1 / 2, a1 / 2, a1 / 2, a1 / 2, a1 / 2, -a1 / 2);
  go(0);

  out.setPrecision(3);
  out(result);
  out(result_len);
  for (int i = 0; i <= result_len; i++) {
    if (i > 0)
      out.print(" ");
    out.print(result_sequence[i].cur);
  }
  out("");
}
