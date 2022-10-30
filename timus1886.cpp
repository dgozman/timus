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

long double inf = 1e100;

struct Flight {
  int from, to;
  long double p;
  i64 time, len, late;
  long double finish;

  bool operator<(const Flight& f) const {
    return time < f.time;
  }
};

void solve(Input& in, Output& out) {
  int n = in.ni();
  int m = in.ni();
  int start_u = 0;
  int finish_u = n - 1;

  vec<Flight> all(m);
  for (int i = 0; i < m; i++) {
    Flight f;
    f.from = in.ni() - 1;
    f.to = in.ni() - 1;
    f.time = in.ni();
    f.len = in.ni();
    f.p = 0.01 * in.ni();
    f.late = in.ni();
    f.finish = inf;
    all[i] = f;
  }
  sort(all.begin(), all.end());

  vec2d<Flight> outgoing(n);
  for (int i = 0; i < m; i++)
    outgoing[all[i].from].push_back(all[i]);
  vec<int> counter(n);
  vec2d<long double> min_from(n);
  for (int i = 0; i < n; i++) {
    counter[i] = outgoing[i].size();
    min_from[i] = vec<long double>(counter[i] + 1, inf);
  }

  Flight temp;
  for (int i = m - 1; i >= 0; i--) {
    int u = all[i].from;
    if (counter[u] == 0)
      panic();
    Flight& f = outgoing[u][counter[u] - 1];

    if (f.to == finish_u) {
      f.finish = f.time + f.len + f.p * f.late;
    } else {
      auto& candidates = outgoing[f.to];
      temp.time = f.time + f.len + f.late;
      auto it_late = std::lower_bound(candidates.begin() + counter[f.to], candidates.end(), temp);
      // long double late_time = it_late != candidates.end() ? min_from[f.to][it_late - candidates.begin()] : inf;
      long double late_time = min_from[f.to][it_late - candidates.begin()];
      if (late_time < inf / 2) {
        f.finish = f.p * late_time;

        temp.time = f.time + f.len;
        auto it_on_time = std::lower_bound(candidates.begin() + counter[f.to], candidates.end(), temp);
        if (it_on_time == candidates.end())
          panic();
        long double on_time = min_from[f.to][it_on_time - candidates.begin()];
        if (on_time > inf / 2)
          panic();
        f.finish += (1 - f.p) * on_time;
      }
    }

    min_from[u][counter[u] - 1] = min(f.finish, min_from[u][counter[u]]);
    counter[u]--;
  }

  out.setPrecision(10);
  if (min_from[start_u][0] > inf / 2)
    out("Fail");
  else
    out(min_from[start_u][0]);
}
