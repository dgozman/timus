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
  template<typename A, typename B> inline void printOne(const pair<A, B>& pair) { print(pair.first, pair.second); }
  template<typename T, typename...Ts> inline void print(const T &f, const Ts&... args) {
    *out << f;
    if (sizeof...(args) != 0) { *out << ' '; print(args...); }
  }
  template<typename...Ts> inline void println(const Ts&... args) { print(args...); *out << '\n'; }
  template<typename...Ts> inline void operator() (const Ts&... args) { println(args...); }
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

void solve(Input& in, Output& out) {
  string s(std::istreambuf_iterator<char>(*in.in), {});
  out.println("//CPP");
  out.println("#include<iostream>");
  out.println("#include<vector>");
  out.print("using s=std::wstring;");

  vec<string> words({"The", "the", "And", "and", "that", "have", "for", "not", "with", "you", "You", "this", "\"", "\n"});
  vec<string> chars({"¡", "¢", "£", "¤", "¥", "¦", "§", "¨", "©", "ª", "°", "±", "$", "^"});

  out.print("s c(L\"");
  for (int i = 0; i < chars.size(); i++)
    out.print(chars[i]);
  out.print("\");");

  out.print("std::vector<std::string> w({");
  for (int i = 0; i < words.size(); i++) {
    if (i > 0)
      out.print(",");
    out.print("\"");
    if (words[i] == "\"")
      out.print("\\\"");
    else if (words[i] == "\n")
      out.print("\\n");
    else
      out.print(words[i]);
    out.print("\"");
  }
  out.print("});");

  out.print("std::vector<s> x({L\"");
  int cur = 0;
  for (int i = 0; i < s.length(); i++) {
    if (cur >= 20000) {
      out.print("\"\n,L\"");
      cur = 0;
    }
    bool matched = false;
    for (int j = 0; j < words.size(); j++) {
      if (s.substr(i, words[j].length()) == words[j]) {
        out.print(chars[j]);
        cur += chars[j].length();
        i += words[j].length() - 1;
        matched = true;
        break;
      }
    }
    if (!matched) {
      out.print(string(1, s[i]));
      cur += 1;
    }
  }
  out.println("\"});");
  out.print("int main(){for(auto& z:x)for(auto y:z){int j=c.find(y);std::cout<<(j==s::npos?std::string(1,y):w[j]);}return 0;}");
}
