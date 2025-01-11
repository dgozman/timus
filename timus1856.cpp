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

pair<string, int> solveSlow1(Output& out, int n, vec<int>& a, vec<int>& b, bool together) {
  int NONE = 0;
  int SINGLE = 1;
  int MULTIPLE = 2;
  int COUNTING = 3;
  map<int, int> who;
  int max_diff = 0;

  auto apply = [&](const vec<int>& has, const vec<int>& a, vec<int>& has2) {
    for (int i = 0; i < n; i++)
      has2[i] = 0;
    for (int i = 0; i < n; i++) {
      if (has[i])
        has2[a[i]] = 1;
    }
  };

  function<int(const vec<int>&, int, int)> go = [&](const vec<int>& has, int turn, int depth) {
    int key = 0;
    for (int i = 0; i < n; i++)
      key = key * 2 + has[i];
    key = key * 2 + turn - 1;

    // out("go", has, turn, key, MAX);

    int count = 0;
    for (int i = 0; i < n; i++)
      count += has[i];
    if (count == 1)
      return SINGLE;

    if (who.count(key)) {
      if (who[key] >= COUNTING) {
        max_diff = max(max_diff, depth - who[key]);
        return MULTIPLE;
      }
      return who[key];
    }

    who[key] = COUNTING + depth;
    int other = 3 - turn;
    vec<int> has2(n);

    int target = together ? SINGLE : turn;
    apply(has, a, has2);
    if (go(has2, other, depth + 1) == target) {
      who[key] = target;
      return target;
    }

    apply(has, b, has2);
    if (go(has2, other, depth + 1) == target) {
      who[key] = target;
      return target;
    }

    who[key] = together ? MULTIPLE : other;
    return other;
  };

  vec<int> has(n);
  for (int i = 0; i < n; i++)
    has[i] = 1;

  return go(has, 1, 0) == SINGLE ? make_pair("War", max_diff) : make_pair("Peace", max_diff);
}

pair<string, int> solveSlow2(Output& out, int n, vec<int>& a, vec<int>& b) {
  int COUNTING = 0;
  int MULTIPLE = -1;
  map<int, int> who;

  auto apply = [&](const vec<int>& has, const vec<int>& a, vec<int>& has2) {
    for (int i = 0; i < n; i++)
      has2[i] = 0;
    for (int i = 0; i < n; i++) {
      if (has[i])
        has2[a[i]] = 1;
    }
  };

  auto single = [&](const vec<int>& has) {
    int count = 0;
    for (int i = 0; i < n; i++)
      count += has[i];
    return count == 1;
  };

  auto best = [&](int a, int b) {
    if (a == MULTIPLE || b == MULTIPLE)
      return a + b - MULTIPLE;
    return min(a, b);
  };

  auto worst = [&](int a, int b) {
    if (a == MULTIPLE || b == MULTIPLE)
      return MULTIPLE;
    return max(a, b);
  };

  function<int(const vec<int>&)> go = [&](const vec<int>& has) {
    int key = 0;
    for (int i = 0; i < n; i++)
      key = key * 2 + has[i];

    out("go", has, "k=", key);

    if (who.count(key)) {
      if (who[key] == COUNTING) {
        out("go.1", has, "k=", key);
        return MULTIPLE;
      }
      out("go.2", has, "k=", key, "who", who[key]);
      return who[key];
    }

    if (single(has)) {
      out("go.3", has, "k=", key);
      return who[key] = 1;
    }

    who[key] = COUNTING;

    int res = MULTIPLE;
    vec<int> temp(n), has_next(n);

    apply(has, a, temp);
    out("go.4", has, "k=", key, temp);
    if (single(temp)) {
      out("go.5", has, "k=", key, temp);
      res = 1;
    } else {
      int other = 0;
      out("go.6", has, "k=", key, temp);
      apply(temp, a, has_next);
      out("go.7", has, "k=", key, has_next);
      other = worst(other, go(has_next));
      out("go.8", has, "k=", key, has_next, "->", other);
      apply(temp, b, has_next);
      out("go.9", has, "k=", key, has_next);
      other = worst(other, go(has_next));
      out("go.10", has, "k=", key, has_next, "->", other);
      res = best(res, other);
    }

    apply(has, b, temp);
    out("go.11", has, "k=", key, temp);
    if (single(temp)) {
      out("go.12", has, "k=", key, temp);
      res = 1;
    } else {
      int other = 0;
      out("go.13", has, "k=", key, temp);
      apply(temp, a, has_next);
      out("go.14", has, "k=", key, has_next);
      other = worst(other, go(has_next));
      out("go.15", has, "k=", key, has_next, "->", other);
      apply(temp, b, has_next);
      out("go.16", has, "k=", key, has_next);
      other = worst(other, go(has_next));
      out("go.17", has, "k=", key, has_next, "->", other);
      res = best(res, other);
    }

    out("go.18", has, "k=", key, "res", res);
    return who[key] = res;
  };

  vec<int> has(n);
  for (int i = 0; i < n; i++)
    has[i] = 1;

  return go(has) == MULTIPLE ? make_pair("Peace", who.size()) : make_pair("War", who.size());
}

void stress(Output& out) {
  std::srand(17);
  int n = 3;
  vec<int> a(n), b(n);
  set<int> seen;
  int count = 0;
  int max_who = 0;
  while (true) {
    int key = 0;
    for (int i = 0; i < n; i++) {
      a[i] = std::rand() % n;
      key = key * n + a[i];
      b[i] = std::rand() % n;
      key = key * n + b[i];
    }
    if (seen.count(key))
      continue;
    seen.insert(key);
    auto p1 = solveSlow1(out, n, a, b, false);
    auto p2 = solveSlow2(out, n, a, b);
    if (p1.first != p2.first) {
      out("========", ++count, p1.first, p2.first);
      out(n);
      out(a);
      out(b);
    }
  }
}

void solve(Input& in, Output& out) {
  // stress(out);

  int n = in.ni();
  vec<int> a(n);
  vec<int> b(n);
  for (int i = 0; i < n; i++) {
    a[i] = in.ni() - 1;
    b[i] = in.ni() - 1;
  }
  out(solveSlow2(out, n, a, b).first);
}


// 405 and 324
