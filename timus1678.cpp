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

struct Pos {
  int size, len;
  std::array<int, 15> c;

  Pos(): size(0), len(0), c({}) {}

  void sort() {
    std::sort(c.begin(), c.begin() + len, std::greater<>());
  }

  bool covers(const Pos& other) const {
    if (other.len > len)
      return false;
    for (int i = 0; i < other.len; i++) {
      if (other.c[i] > c[i])
        return false;
    }
    return true;
  }

  bool operator<(const Pos& other) const {
    if (len != other.len)
      return len < other.len;
    for (int i = 0; i < len; i++) {
      if (c[i] != other.c[i])
        return c[i] < other.c[i];
    }
    return false;
  }
};

const int cards = 13;

struct Counter {
  Counter(): cnt({}) {}

  std::array<int, cards> cnt;

  Pos to_pos() {
    Pos result;
    for (int i = 0; i < cards; i++) {
      if (cnt[i] > 0) {
        result.c[result.len++] = cnt[i];
        result.size += cnt[i];
      }
    }
    result.sort();
    return result;
  }
};

void solve(Input& in, Output& out) {
  stringstream ss;
  ss << in.getline();
  Input first_line(ss);
  int players = first_line.ni();
  int self = first_line.ni();
  int shared = first_line.ni();
  int total = self + shared;

  auto read_counter = [&] () {
    Counter result;
    string s = in.getline();
    string all = "23456789TJQKA";
    for (int i = 0; i < s.length(); i++) {
      int c = all.find(s[i]);
      result.cnt[c]++;
    }
    return result;
  };
  Counter initial_self = read_counter();
  Counter initial_shared = read_counter();

  int combination_count = in.ni();
  vec<Pos> combinations;
  for (int i = 0; i < combination_count; i++) {
    Pos comb;
    comb.len = in.ni();
    for (int j = 0; j < comb.len; j++) {
      comb.c[j] = in.ni();
      comb.size += comb.c[j];
    }
    comb.sort();
    combinations.push_back(comb);
  }

  auto gen_positions = [&] (int size) {
    vec<Pos> result;
    function<void(Pos&)> gen = [&] (Pos& pos) {
      if (pos.size == size) {
        Pos p = pos;
        p.sort();
        result.push_back(p);
        return;
      }
      if (pos.len == cards)
        return;
      int from = pos.len ? pos.c[pos.len - 1] : 1;
      for (int next = from; pos.size + next <= size; next++) {
        pos.c[pos.len++] = next;
        pos.size += next;
        gen(pos);
        pos.size -= next;
        pos.len--;
      }
    };
    Pos temp;
    gen(temp);
    sort(result.begin(), result.end());
    return result;
  };

  vec<Pos> total_all = gen_positions(total);
  vec<int> total_gain(total_all.size(), 0);
  for (int i = 0; i < total_all.size(); i++) {
    const Pos& pos = total_all[i];
    for (int j = 0; j < combination_count; j++) {
      if (pos.covers(combinations[j]))
        total_gain[i] = j + 1;
    }
  }

  vec<Pos> shared_all = gen_positions(shared);
  vec2d<u64> gain_counts = newVec2d(shared_all.size(), combination_count + 1, (u64) 0);
  u64 gain_counts_divider = 0;

  auto fill_gain_counts = [&] () {
    Counter shared_c = initial_shared;
    Counter total_c = initial_self;
    int shared_missing = shared;
    for (int i = 0; i < cards; i++) {
      total_c.cnt[i] += initial_shared.cnt[i];
      shared_missing -= initial_shared.cnt[i];
    }

    function<void(int)> gen = [&] (int left) {
      if (!left) {
        gain_counts_divider++;
        Pos shared_p = shared_c.to_pos();
        Pos total_p = total_c.to_pos();
        int shared_i = std::lower_bound(shared_all.begin(), shared_all.end(), shared_p) - shared_all.begin();
        int total_i = std::lower_bound(total_all.begin(), total_all.end(), total_p) - total_all.begin();
        gain_counts[shared_i][total_gain[total_i]]++;
        return;
      }
      for (int i = 0; i < cards; i++) {
        shared_c.cnt[i]++;
        total_c.cnt[i]++;
        gen(left - 1);
        shared_c.cnt[i]--;
        total_c.cnt[i]--;
      }
    };
    gen(shared_missing);
  };
  fill_gain_counts();

  vec2d<u64> C = newVec2d(16, 16, (u64) 0);
  C[0][0] = 1;
  for (int i = 1; i < 16; i++) {
    C[i][0] = C[i][i] = 1;
    for (int j = 1; j < i; j++)
      C[i][j] = C[i - 1][j] + C[i - 1][j - 1];
  }
  vec<u64> F(16, 0);
  F[0] = 1;
  for (int i = 1; i < 16; i++)
    F[i] = F[i - 1] * i;

  auto upgrade = [&] (const Pos& from, const Pos& to) {
    std::array<int, 15> where;
    for (int i = 0; i < from.len; i++)
      where[i] = -1;
    std::array<int, 15> whom;
    for (int i = 0; i < to.len; i++)
      whom[i] = -1;
    u64 result = 0;
    if (!to.covers(from))
      return result;
    function<void(int, int, int, u64)> go = [&] (int index, int taken, int rem, u64 c) {
      if (to.len - index + taken < from.len)
        return;
      if (index == to.len) {
        u64 d = 1;
        for (int i = 0; i < index; i++) {
          int cnt = 0;
          bool skip = false;
          for (int j = 0; j < index; j++) {
            if (to.c[i] == to.c[j] && whom[i] == whom[j]) {
              if (j < i)
                skip = true;
              else
                cnt++;
            }
          }
          if (!skip)
            d *= F[cnt];
        }
        if (c % d)
          panic();
        result += c / d;
        return;
      }
      for (int i = 0; i < from.len; i++) {
        if (where[i] != -1 || to.c[index] < from.c[i] || rem < (to.c[index] - from.c[i]))
          continue;
        bool bad = false;
        for (int j = 0; j < index; j++) {
          if (to.c[j] == to.c[index] && (whom[j] == -1 || whom[j] > i))
            bad = true;
        }
        if (bad)
          continue;
        where[i] = index;
        whom[index] = i;
        go(index + 1, taken + 1, rem - (to.c[index] - from.c[i]), c * C[rem][to.c[index] - from.c[i]]);
        where[i] = -1;
        whom[index] = -1;
      }
      if (rem >= to.c[index])
        go(index + 1, taken, rem - to.c[index], c * (cards - from.len - (index - taken)) * C[rem][to.c[index]]);
    };
    go(0, 0, to.size - from.size, 1);
    return result;
  };

  long double result = 0;
  u64 self_max = 1;
  for (int i = 0; i < self; i++)
    self_max *= cards;
  for (int shared_i = 0; shared_i < shared_all.size(); shared_i++) {
    const Pos& shared_p = shared_all[shared_i];
    vec<u64> other_gain_counts(combination_count + 1, (u64) 0);
    for (int total_i = 0; total_i < total_all.size(); total_i++) {
      u64 count = upgrade(shared_p, total_all[total_i]);
      other_gain_counts[total_gain[total_i]] += count;
    }

    u64 less_than = 0;
    for (int gain = 0; gain <= combination_count; gain++) {
      long double r = gain_counts[shared_i][gain];
      for (int i = 0; i < players - 1; i++)
        r = r * less_than / self_max;
      result += r;
      less_than += other_gain_counts[gain];
    }
  }
  result = result / gain_counts_divider;
  out.setPrecision(10);
  out(result);
}

/*

2 5 2
23456
78
2
7 1 1 1 1 1 1 1
1 4
// 0.8407915043

2 5 2
23456

1
1 2
// 0.0883526857

2 2 1
23

1
1 2
// 0.1201638598

2 2 1
23
4
1
1 2
// 0

10 10 5
2222222222

2
1 2
1 10
// 0.9999982258

2 10 5
2222222222

1
1 1
// 0


*/

