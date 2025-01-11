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

i64 my_div(i64 a, i64 b) {
  return 0;
}

constexpr i64 BASE = 10;
using Digit = u64;
struct BigInt {
  vec<Digit> digits;
  int sign;
  BigInt(): sign(0) {}
  BigInt(i64 x): sign(0) {
    if (x < 0) {
      sign = -1;
      x = -x;
    } else if (x > 0) {
      sign = 1;
    }
    while (x) {
      digits.push_back(x % BASE);
      x /= BASE;
    }
  }
  BigInt(const vec<Digit>& d, int s): digits(d), sign(s) {}

  BigInt operator-() const {
    return BigInt(digits, -sign);
  }
  BigInt operator+(const BigInt& o) const {
    if (!sign)
      return o;
    if (!o.sign)
      return *this;
    if (sign == o.sign)
      return BigInt(add(digits, o.digits), sign);
    int cmp = compare(digits, o.digits);
    if (!cmp)
      return BigInt();
    BigInt result;
    result.digits = cmp > 0 ? sub(digits, o.digits) : sub(o.digits, digits);
    result.sign = cmp > 0 ? sign : -sign;
  }
  BigInt operator-(const BigInt& o) const {
    if (!sign)
      return -o;
    if (!o.sign)
      return *this;
    if (sign != o.sign)
      return BigInt(add(digits, o.digits), sign);
    int cmp = compare(digits, o.digits);
    if (!cmp)
      return BigInt();
    BigInt result;
    result.digits = cmp > 0 ? sub(digits, o.digits) : sub(o.digits, digits);
    result.sign = cmp > 0 ? sign : -sign;
  }
  BigInt operator*(const BigInt& o) const {
    if (!sign || !o.sign)
      return BigInt();
    return BigInt(mul(digits, o.digits), sign * o.sign);
  }

  static vec<Digit> add(const vec<Digit>& a, const vec<Digit>& b) {
    int n = a.size();
    int m = b.size();
    int carry = 0;
    vec<Digit> result;
    for (int i = 0; i < n || i < m || carry; i++) {
      Digit s = carry + (i < n ? a[i] : 0) + (i < m ? b[i] : 0);
      if (s >= BASE) {
        result.push_back(s - BASE);
        carry = 1;
      } else {
        result.push_back(s);
        carry = 0;
      }
    }
    return result;
  }

  static vec<Digit> sub(const vec<Digit>& a, const vec<Digit>& b) {
    vec<Digit> result;
    int take = 0;
    for (int i = 0; i < a.size(); i++) {
      Digit x = a[i];
      Digit y = i < b.size() ? b[i] : 0;
      y += take;
      if (x < y) {
        result.push_back(x + BASE - y);
        take = 1;
      } else {
        result.push_back(x - y);
        take = 0;
      }
    }
    while (result.size() && !result.back())
      result.pop_back();
    return result;
  }

  static vec<Digit> mul(const vec<Digit>& a, const vec<Digit>& b) {
    int n = a.size();
    int m = b.size();
    vec<Digit> result(n + m, 0);
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < m; j++) {
        int k = i + j;
        result[k] += a[i] * b[j];
        if (result[k] >= BASE) {
          result[k + 1] += result[k] / BASE;
          result[k] %= BASE;
        }
      }
    }
    for (int i = 0; i < result.size() - 1; i++) {
      if (result[i] >= BASE) {
        result[i + 1] += result[i] / BASE;
        result[i] %= BASE;
      }
    }
    while (result.size() && !result.back())
      result.pop_back();
    return result;
  }

  static int compare(const vec<Digit>& a, const vec<Digit>& b) {
    int n = a.size();
    int m = b.size();
    if (n != m)
      return n < m ? -1 : 1;
    for (int i = n - 1; i >= 0; i--) {
      if (a[i] != b[i])
        return a[i] < b[i] ? -1 : 1;
    }
    return 0;
  }
};

struct Gauss {
  i64 p, q;
  Gauss(): p(0), q(0) {}
  Gauss(i64 x): p(x), q(0) {}
  Gauss(i64 np, i64 nq): p(np), q(nq) {}

  i64 norm() const {
    return p * p + q * q;
  }
  Gauss conj() const {
    return Gauss(p, -q);
  }
  Gauss inv(const Gauss& mod) const {
    // 1 / p+qi
    // p-qi / (p^2 + q^2)
    //

    Gauss nom = this->conj() * mod.conj();
    i64 d = mod.norm() * this->norm();
    Gauss quotient(nom.p / d, nom.q / d);
    Gauss remainder = *this - quotient * mod;
  }

  Gauss operator-() const {
    return Gauss(-p, -q);
  }
  Gauss operator+(const Gauss& other) const {
    return Gauss(p + other.p, q + other.q);
  }
  Gauss operator-(const Gauss& other) const {
    return Gauss(p - other.p, q - other.q);
  }
  Gauss operator*(i64 d) const {
    return Gauss(p * d, q * d);
  }
  Gauss operator*(const Gauss& other) const {
    return Gauss(p * other.p - q * other.q, p * other.q + q * other.p);
  }
  Gauss operator/(const Gauss& other) const {
    Gauss nom = *this * other.conj();
    i64 d = other.norm();
    return Gauss(nom.p / d, nom.q / d);
  }
  Gauss operator%(const Gauss& other) const {
    return *this - (*this / other) * other;
  }
  Gauss& operator*=(const Gauss& other) {
    i64 temp = p * other.p - q * other.q;
    q = p * other.q + q * other.p;
    p = temp;
    return *this;
  }
  bool operator==(const Gauss& other) const {
    return p == other.p && q == other.q;
  }
  bool operator!=(const Gauss& other) const {
    return p != other.p || q != other.q;
  }
  operator bool() const {
    return !(!p && !q);
  }
};

ostream& operator<<(ostream& os, const Gauss& g)
{
  if (g.q < 0)
    os << g.p << '-' << -g.q << 'i';
  else
    os << g.p << '+' << g.q << 'i';
  return os;
}

Gauss det(Output& out, vec2d<Gauss>& w, Gauss mod) {
  int m = w.size();
  for (int i = 0; i < m; i++) {
    // out("before", i, w);
    int maxr = i;
    int maxc = i;
    for (int r = i; r < m; r++) {
      for (int c = i; c < m; c++) {
        if (w[r][c]) {
          maxr = r;
          maxc = c;
        }
      }
    }
    if (!w[maxr][maxc])
      break;
    // out("middle", maxr, maxc, w[maxr][maxc]);
    for (int j = 0; j < m; j++) {
      Gauss tmp = w[i][j];
      w[i][j] = w[maxr][j];
      w[maxr][j] = tmp;
    }
    for (int j = 0; j < m; j++) {
      Gauss tmp = w[j][i];
      w[j][i] = w[j][maxc];
      w[j][maxc] = tmp;
    }
    // out("swapped", w);

    for (int j = i + 1; j < m; j++) {
      Gauss mul = w[j][i].inv(mod);
      for (int k = 0; k < m; k++)
        w[j][k] = (w[j][k] - w[i][k] * mul) % mod;
    }
    // out("after", i, w);
  }

  Gauss result(1, 0);
  for (int j = 0; j < m; j++)
    result = (result * w[j][j]) % mod;
  return result;
}


void solve(Input& in, Output& out) {
  int n = in.ni();
  auto w = newVec2d(n, n, Gauss(0, 0));
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++)
      w[i][j] = Gauss(in.ni64(), in.ni64());
  }
  Gauss p(in.ni64(), in.ni64());
  Gauss result = det(out, w, p);
  out(result.p, result.q);
}

// https://en.wikipedia.org/wiki/Bareiss_algorithm
// https://math.libretexts.org/Bookshelves/Combinatorics_and_Discrete_Mathematics/Elementary_Number_Theory_(Barrus_and_Clark)/01%3A_Chapters/1.13%3A_The_Gaussian_Integers
