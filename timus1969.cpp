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

int parse_time(const string& s) {
  int hours = stoi(s.substr(0, 2));
  int minutes = stoi(s.substr(3, 2));
  int seconds = stoi(s.substr(6, 2));
  return hours * 3600 + minutes * 60 + seconds;
}

string print_time(int t) {
  int hours = t / 3600;
  t %= 3600;
  int minutes = t / 60;
  t %= 60;
  int seconds = t;
  return (hours < 10 ? "0" : "") + to_string(hours) + ":" +
         (minutes < 10 ? "0" : "") + to_string(minutes) + ":" +
         (seconds < 10 ? "0" : "") + to_string(seconds);
}

void solve(Input& in, Output& out) {
  vec<int> times;
  string s;
  while (s = in.getline(), !s.empty())
    times.push_back(parse_time(s));

  int start = times[0];
  int a = times[1] - times[0];
  vec<int> starts(times.size());
  vec<int> lap(times.size());
  // out(times);
  for (int i = 2; i < times.size(); ++i) {
    if ((times[i] - times[1]) % 2)
      continue;
    int b = (times[i] - times[1]) / 2;
    int first_cycle = start + 2 * a + 2 * b;
    int total = 0;
    int passed_twice = 0;
    int j = 1;
    // out(start, a, b, first_cycle + a);
    while (j < times.size() && times[j] < first_cycle + a) {
      // out(j, times[j], total, passed_twice);
      if (passed_twice < total) {
        int next_back = starts[passed_twice] + a + b + b;
        // out("next_back", next_back, starts[passed_twice]);
        if (next_back == times[j]) {
          ++passed_twice;
          ++j;
          continue;
        }
      }
      starts[total++] = times[j] - a;
      ++j;
    }
    if ((times.size() - 1) % (total * 2))
      continue;
    int laps = (times.size() - 1) / (total * 2);
    for (int j = 0; j < total; ++j)
      lap[j] = 0;
    int next_forward = 0;
    int next_back = 0;
    bool error = false;
    for (int j = 1; j < times.size(); j++) {
      int next_back_time = starts[next_back] + (a + b) * (lap[next_back] * 2 + 1) + b;
      if (next_back_time == times[j]) {
        lap[next_back]++;
        next_back = (next_back + 1) % total;
        continue;
      }
      int next_forward_time = starts[next_forward] + (a + b) * (lap[next_forward] * 2) + a;
      if (next_forward_time == times[j] && lap[next_forward] < laps) {
        next_forward = (next_forward + 1) % total;
        continue;
      }
      error = true;
      break;
    }
    if (error)
      continue;

    for (int j = 0; j < total; ++j) {
      out.print(print_time(starts[j]));
      for (int l = 0; l < laps; ++l)
        out.print("", print_time(starts[j] + (a + b) * (l * 2 + 1)), print_time(starts[j] + (a + b) * (l * 2 + 2)));
      out.println();
    }

    return;
  }

  panic();
}
