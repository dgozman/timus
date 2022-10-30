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
template<typename T> using matrix = vec<vec<T>>;
using i64 = int64_t;
using u64 = uint64_t;

struct Input {
  Input(istream &in) : in(&in) {}
  istream *in;
  template<class T> T next() const { T x; *in >> x; return x; }
  int ni() const { return next<int>(); }
  i64 ni64() const { return next<i64>(); }
  template<class T> vec<T> nvec(int n) const { vec<T> v(n); for (int i = 0; i < n; ++i) v[i] = next<T>(); return v; }
  vec<int> nvi(int n) const { return nvec<int>(n); }
};

struct Output {
  Output(ostream &out) : out(&out) {}
  ostream *out;
  inline void print() {}
  template<typename T, typename...Ts> inline void print(const T &f, const Ts&... args) {
    *out << f;
    if (sizeof...(args) != 0) { *out << ' '; print(args...); }
  }
  template<typename...Ts> inline void println(const Ts&... args) { print(args...); *out << '\n'; }
  template<typename T> inline void printVec(const vec<T>& v) { for (int i = 0; i < v.size(); i++) { if (i) *out << ' '; print(v[i]); } }
  template<typename T> inline void printlnVec(const vec<T>& v) { printVec(v); *out << '\n'; }
  template<typename...Ts> inline void operator() (const Ts&... args) { println(args...); }
  template<typename T> inline void operator() (const vec<T>& v) { printlnVec(v); }
};

template<typename T> matrix<T> newmatrix(int n, int m, const T& init) {
  matrix<T> v(n, vec<T>(m, init));
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

// value, index
using Box = pair<int, int>;

void solve(Input& in, Output& out) {
  int n = in.ni();
  int m = in.ni();
  int K = in.ni();

  vec<Box> boxes;
  int sum = 0;
  for (int i = 0; i < n; i++) {
    boxes.push_back(make_pair(in.ni(), i));
    sum += boxes[i].first;
  }

  sort(boxes.begin(), boxes.end());

  vec<int> where(n);
  set<pair<int, int>> b;
  for (int i = 0; i < m; i++)
    b.insert(make_pair(0, i));
  for (int i = n - 1; i >= 0; i--) {
    auto it = b.begin();
    auto min = *it;
    b.erase(it);
    min.first += boxes[i].first;
    where[i] = min.second;
    b.insert(min);
  }

  vec<int> p(n);
  vec<pair<int, int>> a(m), c(m);
  vec<int> newc(m);
  int result = 0;

  while (true) {
    for (int i = 0; i < n; i++) {
      int j = rand() % (i + 1);
      p[i] = p[j];
      p[j] = i;
    }

    for (int i = 0; i < m; i++) {
      a[i] = make_pair(0, i);
      c[i] = make_pair(0, i);
    }

    for (int i = 0; i < n; i++) {
      int box = p[i];
      int gen = where[box];
      if (gen >= m)
        gen -= m;
      if (a[gen].first < c[gen].first) {
        a[gen].first += boxes[box].first;
        where[box] = gen + m;
      } else {
        c[gen].first += boxes[box].first;
        where[box] = gen;
      }
    }

    int mingen = sum;
    int maxgen = 0;
    for (int i = 0; i < m; i++) {
      int gen = a[i].first + c[i].first;
      mingen = min(mingen, gen);
      maxgen = max(maxgen, gen);
    }
    if (maxgen - mingen <= K) {
      result = maxgen - mingen;
      break;
    }

    sort(a.begin(), a.end());
    sort(c.begin(), c.end());
    for (int i = 0; i < m; i++) {
      int ai = a[i].second;
      int ci = c[m - i - 1].second;
      newc[ci] = ai;
    }

    for (int i = 0; i < n; i++) {
      int gen = where[i];
      if (gen < m)
        gen = newc[gen];
      where[i] = gen;
    }
  }

  vec<vec<int>> r(m);
  for (int i = 0; i < n; i++) {
    int gen = where[i];
    if (gen >= m)
      gen -= m;
    r[gen].push_back(boxes[i].second + 1);
  }

  out(result);
  for (int i = 0; i < m; i++)
    out(r[i]);
}
