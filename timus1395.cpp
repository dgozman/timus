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

const int maxn = 10002;
u32 num[maxn], ans[maxn];

pair<u32, u32> a[maxn];
u32 b[maxn];

const u32 maxh = 250619;
u32 hash_first[maxh];  // +hash_offset

// (next value, cindex), next+hash_offset
const int maxvalues = 200000;
pair<pair<u32, u32>, u32> hash_values[maxvalues];
// second, delta, len
pair<pair<u32, u32>, u32> c_values[maxn];

int solve1(Input& in, Output& out, u32 n) {
  for (u32 i = 0; i < n; i++)
    a[i] = make_pair(num[i], i);
  sort(a, a + n);
  u32 m = 1;
  for (u32 i = 1; i < n; i++) {
    if (a[i].first != a[m - 1].first)
      a[m++] = a[i];
  }
  n = m;
  b[0] = 0;
  for (u32 i = 0; i < n; i++)
    b[i + 1] = a[i].first;
  b[n + 1] = 2000000001;

  if (n == 1) {
    ans[0] = 1;
    return 1;
  }

  u32 rcount = 2;
  u32 rfirst = 1;
  u32 rsecond = 2;

  u32 iter = 0;
  for (u32 i = 0; i < maxh; i++)
    hash_first[i] = 0;
  for (u32 i = 0; i < maxvalues; i++)
    hash_values[i].second = 0;
  u32 hash_offset = 1;

  for (u32 first = 1; first < n; first++) {
    u32 first_value = b[first];
    u32 previous = first - 1;
    u32 last_cindex = 0;
    u32 hash_size = 0;

    for (u32 second = first + 1; second <= n; second++) {
      u32 second_value = b[second];
      u32 previous_value = 2 * first_value < second_value ? 0 : 2 * first_value - second_value;
      while (b[previous] > previous_value)
        previous--;

      if (previous == 0 || b[previous] != previous_value) {
        u32 delta = second_value - first_value;
        c_values[last_cindex].first.first = second;
        c_values[last_cindex].first.second = delta;
        c_values[last_cindex].second = 2;

        u32 value = second_value + delta;
        u32 h = value % maxh;
        hash_values[hash_size].first.first = value;
        hash_values[hash_size].first.second = last_cindex++;
        hash_values[hash_size].second = hash_first[h];
        hash_first[h] = hash_offset + hash_size;
        hash_size++;
      }

      u32 h = second_value % maxh;
      u32 i = hash_first[h];
      while (i >= hash_offset) {
        iter++;

        i -= hash_offset;
        u32 cindex = hash_values[i].first.second;
        if (hash_values[i].first.first != second_value) {
          i = hash_values[i].second;
          continue;
        }
        i = hash_values[i].second;

        c_values[cindex].second++;
        if (c_values[cindex].second > rcount) {
          rcount = c_values[cindex].second;
          rfirst = first;
          rsecond = c_values[cindex].first.first;
        }

        u32 value = second_value + c_values[cindex].first.second;
        u32 h2 = value % maxh;
        hash_values[hash_size].first.first = value;
        hash_values[hash_size].first.second = cindex;
        hash_values[hash_size].second = hash_first[h2];
        hash_first[h2] = hash_offset + hash_size;
        hash_size++;
      }
    }

    hash_offset += hash_size;
  }
  // out("iter", iter);

  ans[0] = a[rfirst - 1].second + 1;
  ans[1] = a[rsecond - 1].second + 1;
  u32 rindex = rsecond;
  u32 rvalue = b[rsecond];
  for (u32 i = 2; i < rcount; i++) {
    rvalue += b[rsecond] - b[rfirst];
    while (b[rindex] < rvalue)
      rindex++;
    ans[i] = a[rindex - 1].second + 1;
  }
  return rcount;
}

void solve(Input& in, Output& out) {
  int n = in.ni();
  for (int i = 0; i < n; i++)
    num[i] = in.ni();
  // u32 n = 10000;
  // for (u32 i = 0; i < n; i++)
  //   num[i] = i;

  u32 res = solve1(in, out, n);
  out(res);
  for (int i = 0; i < res; i++) {
    if (i > 0)
      out.print(" ");
    out.print(ans[i]);
  }
  out("");
}
