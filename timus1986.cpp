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
  bool eof() { return in->eof(); }
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

const long double eps = 1e-15;
const long double lax_eps = 1e-10;

// Returns the number of non-free variables. These are first in the w and perm vectors.
int gauss(vec2d<long double>& w, vec<int>& perm) {
  int m = w[0].size() - 1;
  perm.resize(m);
  for (int i = 0; i < m; i++)
    perm[i] = i;

  int i = 0;
  while (i < m && i < w.size()) {
    int maxr = i;
    int maxc = i;
    for (int r = i; r < w.size(); r++) {
      for (int c = i; c < m; c++) {
        if (fabs(w[r][c]) > fabs(w[maxr][maxc])) {
          maxr = r;
          maxc = c;
        }
      }
    }
    if (fabs(w[maxr][maxc]) < eps)
      break;
    for (int j = 0; j <= m; j++) {
      long double tmp = w[i][j];
      w[i][j] = w[maxr][j];
      w[maxr][j] = tmp;
    }
    int tperm = perm[i];
    perm[i] = perm[maxc];
    perm[maxc] = tperm;
    for (int j = 0; j < w.size(); j++) {
      long double tmp = w[j][i];
      w[j][i] = w[j][maxc];
      w[j][maxc] = tmp;
    }

    for (int j = 0; j < w.size(); j++) {
      if (j == i)
        continue;
      long double t = w[j][i] / w[i][i];
      for (int k = 0; k <= m; k++)
        w[j][k] = w[j][k] - w[i][k] * t;
    }

    long double s = 0;
    for (int j = 0; j < w.size(); j++) {
      for (int k = 0; k < m; k++)
        s += w[j][k];
    }
    if (fabs(s) > eps) {
      for (int j = 0; j < w.size(); j++) {
        for (int k = 0; k < m + 1; k++)
          w[j][k] /= s;
      }
    }
    i++;
  }

  bool solution = true;
  for (int r = i; r < w.size(); r++) {
    if (fabs(w[r][m]) > lax_eps) {
      // cout << "bad at" << r << " " << w[r][m] << endl;
      solution = false;
    }
  }
  if (!solution)
    return -1;
  return i;
}

void restore(vec2d<long double>& w, vec<long double>& x, int dep) {
  int m = w[0].size() - 1;
  for (int i = 0; i < dep; i++) {
    x[i] = w[i][m];
    for (int j = dep; j < m; j++)
      x[i] -= w[i][j] * x[j];
    x[i] /= w[i][i];
  }
}

vec<long double> permute(vec<long double>& x, vec<int>& perm) {
  vec<long double> result(x.size());
  for (int i = 0; i < x.size(); i++)
    result[perm[i]] = x[i];
  return result;
}

void solve(Input& in, Output& out) {
  set<string> all_names;
  map<string, vec<pair<string, int>>> receipts;

  while (!in.eof()) {
    string s = in.getline();
    if (!s.size())
      break;

    istringstream stream(s);
    Input line(stream);

    string name = line.ns();
    all_names.insert(name);
    if (line.ns() != ":")
      panic();

    vec<pair<string, int>> receipt;
    int total = 0;
    while (!line.eof()) {
      string ingridient = line.ns();
      if (!ingridient.size())
        break;
      all_names.insert(ingridient);
      int percent = line.ni();
      total += percent;
      receipt.emplace_back(ingridient, percent);
    }
    if (total != 100)
      panic();
    receipts[name] = receipt;
  }

  set<string> basic_names;
  map<string, int> basic_name_to_index;
  set<string> receipt_names;
  map<string, int> receipt_name_to_index;
  for (auto& name : all_names) {
    if (receipts.count(name))
      receipt_names.insert(name);
    else
      basic_names.insert(name);
  }

  vec<string> sorted_basic_names(basic_names.begin(), basic_names.end());
  vec<string> sorted_receipt_names(receipt_names.begin(), receipt_names.end());

  int r = receipt_names.size();
  for (int i = 0; i < r; i++)
    receipt_name_to_index[sorted_receipt_names[i]] = i;

  int b = basic_names.size();
  for (int i = 0; i < b; i++)
    basic_name_to_index[sorted_basic_names[i]] = i;

  int n = r * b;
  // b1 + c1 + t1 = 100
  // b1 - b2 * 50 / 100 = 0
  // c1 - c2 * 50 / 100 = 30
  // t1 - t2 * 50 / 100 = 20
  auto a = newVec2d<long double>(r * (b + 1), n + 1, (long double)0);
  for (int i = 0; i < r; i++) {
    int base_row = i * (b + 1);
    for (int j = 0; j < b; j++)
      a[base_row][i * b + j] = 1;
    a[base_row][n] = 100;
    for (int j = 0; j < b; j++)
      a[base_row + j + 1][i * b + j] = 1;
    for (auto& [ingridient, percent] : receipts[sorted_receipt_names[i]]) {
      if (receipt_name_to_index.count(ingridient)) {
        int index = receipt_name_to_index[ingridient];
        for (int j = 0; j < b; j++)
          a[base_row + j + 1][index * b + j] += -(long double)percent / 100.0;
      } else {
        int j = basic_name_to_index[ingridient];
        // out("here", ingridient, j);
        a[base_row + j + 1][n] = percent;
      }
    }
  }

  // out(n, "====");
  // out(a);
  // out("====");

  vec<int> perm;
  int gauss_res = gauss(a, perm);
  // out("perm", perm, "gauss_res", gauss_res);
  // out(a);
  if (gauss_res != n)
    panic();

  vec<long double> values(n, (long double)0);
  restore(a, values, n);
  vec<long double> res = permute(values, perm);

  out.setPrecision(6);
  for (int i = 0; i < r; i++) {
    out.print(sorted_receipt_names[i], ":");
    for (int j = 0; j < b; j++) {
      out.print(" ");
      long double value = res[i * b + j];
      if (value < -lax_eps)
        panic();
      if (value < lax_eps)
        out.print(sorted_basic_names[j], 0);
      else
        out.print(sorted_basic_names[j], value);
    }
    out.println();
  }
}
