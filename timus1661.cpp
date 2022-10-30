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

vec<int> concat(vec<int>& a, vec<int>& b) {
  vec<int> r(30);
  for (int i = 0; i < 30; i++)
    r[i] = a[b[i]];
  return r;
}

bool equals(vec<int>& x, vec<int>& y) {
  for (int i = 0; i < 30; i++) {
    if (x[i] != y[i])
      return false;
  }
  return true;
}

bool contains(vec2d<int>& p, vec<int> x) {
  for (auto& t : p) {
    if (equals(t, x))
      return true;
  }
  return false;
}

struct BigInt {
  vec<int> digits;

  BigInt() {}
  BigInt(int x) {
    while (x > 0) {
      digits.push_back(x % 10);
      x /= 10;
    }
  }

  BigInt operator+(const BigInt& other) const {
    BigInt r;
    int carry = 0;
    for (int i = 0; i < max(digits.size(), other.digits.size()) || carry; i++) {
      carry = carry + (i < digits.size() ? digits[i] : 0) + (i < other.digits.size() ? other.digits[i] : 0);
      r.digits.push_back(carry % 10);
      carry /= 10;
    }
    return r;
  }

  BigInt operator*(int o) const {
    BigInt r;
    int carry = 0;
    for (int i = 0; i < digits.size() || carry; i++) {
      carry = carry + (i < digits.size() ? digits[i] : 0) * o;
      r.digits.push_back(carry % 10);
      carry /= 10;
    }
    return r;
  }

  BigInt operator/(int o) const {
    BigInt r;
    r.digits.resize(digits.size());
    int carry = 0;
    for (int i = digits.size() - 1; i >= 0; i--) {
      carry = 10 * carry + digits[i];
      r.digits[i] = carry / o;
      carry = carry % o;
    }
    if (carry > 0)
      panic();
    while (r.digits.size() && r.digits[r.digits.size() - 1] == 0)
      r.digits.pop_back();
    return r;
  }

  BigInt operator*(const BigInt& other) const {
    BigInt r;
    if (!digits.size() || !other.digits.size())
      return r;
    r.digits.resize(digits.size() + other.digits.size() - 1);
    for (int i = 0; i < digits.size(); i++) {
      for (int j = 0; j < other.digits.size(); j++)
        r.digits[i + j] += digits[i] * other.digits[j];
    }
    for (int i = 0; i < r.digits.size() - 1; i++) {
      r.digits[i + 1] += r.digits[i] / 10;
      r.digits[i] = r.digits[i] % 10;
    }
    while (r.digits.size() && r.digits[r.digits.size() - 1] >= 10) {
      int x = r.digits[r.digits.size() - 1];
      r.digits.push_back(x / 10);
      r.digits[r.digits.size() - 2] = x % 10;
    }
    return r;
  }
};

void solve(Input& in, Output& out) {
  vec<int> p1({ 1, 2, 3, 4, 0, 6, 7, 8, 9, 5, 11, 12, 13, 14, 10, 16, 17, 18, 19, 15, 21, 22, 23, 24, 20, 26, 27, 28, 29, 25 });
  // 20 25 15 5 19
  // 21 6 9 29 10
  // 3 12 27 23 13
  // 4 2 22 17 28
  // 26 7 18 14 1
  // 11 16 8 24 0
  vec<int> p2({ 11, 26, 22, 12, 2, 19, 9, 18, 24, 29, 21, 16, 27, 3, 1, 5, 8, 28, 14, 20, 25, 6, 17, 13, 0, 15, 7, 23, 4, 10 });

  // All permutations are a composition of two rotations.
  // https://en.wikipedia.org/wiki/Icosahedral_symmetry
  vec2d<int> p;
  p.push_back(vec<int>({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29 }));
  while (true) {
    int before = p.size();
    auto pcopy = p;
    for (auto& perm : pcopy) {
      auto np = concat(perm, p1);
      if (!contains(p, np))
        p.push_back(np);
      np = concat(perm, p2);
      if (!contains(p, np))
        p.push_back(np);
    }
    if (p.size() == before)
      break;
  }
  if (p.size() != 60)
    panic();

  int c1 = 0;
  int c3 = 0;
  int c5 = 0;
  int c12 = 0;
  for (auto& perm : p) {
    vec<bool> visited(30);
    vec<int> cycles;
    for (int i = 0; i < 30; i++) {
      if (!visited[i]) {
        int len = 0;
        int j = i;
        while (!visited[j]) {
          visited[j] = true;
          len++;
          j = perm[j];
        }
        cycles.push_back(len);
      }
    }
    sort(cycles.begin(), cycles.end());
    // out(cycles);
    if (cycles.size() == 30)
      c1++;
    else if (cycles.size() == 6)
      c5++;
    else if (cycles.size() == 10)
      c3++;
    else if (cycles.size() == 16 && cycles[0] == 1 && cycles[1] == 1)
      c12++;
    else
      panic();
  }

  // out(c1, c5, c3, c12);
  vec<int> colors = in.nvi(30);
  sort(colors.begin(), colors.end());
  vec<int> groups;
  int last = 0;
  for (int i = 0; i < 30; i++) {
    if (i > 0 && colors[i] != colors[i - 1]) {
      groups.push_back(last);
      last = 0;
    }
    last++;
  }
  groups.push_back(last);
  sort(groups.begin(), groups.end());

  vec<BigInt> fact;
  fact.push_back(BigInt(1));
  for (int i = 1; i <= 30; i++)
    fact.push_back(fact[i - 1] * i);

  vec2d<BigInt> C = newVec2d(31, 31, BigInt());
  C[0][0] = BigInt(1);
  for (int i = 1; i <= 30; i++) {
    C[i][0] = BigInt(1);
    C[i][i] = BigInt(1);
    for (int j = 1; j < i; j++)
      C[i][j] = C[i - 1][j] + C[i - 1][j - 1];
  }

  BigInt result;

  // (1, 1, ... 1)
  BigInt r1 = BigInt(1);
  int s = 30;
  for (int g : groups) {
    r1 = r1 * C[s][g];
    s -= g;
  }
  result = result + r1 * c1;

  // (5, 5, ... 5)
  BigInt r5 = BigInt(1);
  s = 30 / 5;
  for (int g : groups) {
    if (g % 5) {
      r5 = r5 * 0;
    } else {
      r5 = r5 * C[s][g / 5];
      s -= g / 5;
    }
  }
  result = result + r5 * c5;

  // (3, 3, ... 3)
  BigInt r3 = BigInt(1);
  s = 30 / 3;
  for (int g : groups) {
    if (g % 3) {
      r3 = r3 * 0;
    } else {
      r3 = r3 * C[s][g / 3];
      s -= g / 3;
    }
  }
  result = result + r3 * c3;

  // (1, 1, 2, 2, 2, 2, ... 2)
  BigInt r12 = BigInt(0);
  for (int i = 0; i < groups.size(); i++) {
    for (int j = 0; j < groups.size(); j++) {
      groups[i]--;
      groups[j]--;

      BigInt rt = BigInt(1);
      s = 28 / 2;
      for (int g : groups) {
        if (g % 2) {
          rt = rt * 0;
        } else {
          rt = rt * C[s][g / 2];
          s -= g / 2;
        }
      }
      r12 = r12 + rt;

      groups[i]++;
      groups[j]++;
    }
  }
  result = result + r12 * c12;

  // https://en.wikipedia.org/wiki/Burnside%27s_lemma
  result = result / 60;
  // out(r1.digits);
  // out(r5.digits);
  // out(r3.digits);
  // out(r12.digits);
  if (!result.digits.size())
    panic();
  for (int i = result.digits.size() - 1; i >= 0; i--)
    out.print(result.digits[i]);
  out("");
}
