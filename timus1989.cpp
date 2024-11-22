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

struct Hash {
  struct Node {
    int left, right, from, to, value;
  };

  int mod, base, size, root;
  vec<Node> nodes;
  vec<int> base_powers;
  // vec<int> values;

  Hash(int n, int base, int mod) : base(base), mod(mod) {
    size = 1;
    while (size < n)
      size = size * 2;
    nodes.resize(size * 2);

    base_powers.resize(size);
    base_powers[0] = 1;
    for (int i = 1; i < size; i++)
      base_powers[i] = (base_powers[i - 1] * base) % mod;

    // values.resize(size);

    int nc = 0;
    function<int(int, int)> build = [&] (int from, int to) {
      if (from > to)
        return -1;
      int index = nc++;
      int mid = (from + to) / 2;
      Node& node = nodes[index];
      node.value = 0;
      node.from = from;
      node.to = to;
      if (from == to) {
        node.left = node.right = -1;
      } else {
        node.left = build(from, mid);
        node.right = build(mid + 1, to);
      }
      return index;
    };
    root = build(0, n - 1);
  }

  void update(int pos, int value) {
    // values[pos] = value;
    update(root, pos, value);
  }

  int update(int u, int pos, int value) {
    Node& node = nodes[u];
    if (pos > node.to || node.from > pos)
      return node.value;

    if (node.from == pos && node.to == pos) {
      node.value = value;
      return node.value;
    }

    int left = 0;
    int left_len = 0;
    if (node.left != -1) {
      left = update(node.left, pos, value);
      left_len = nodes[node.left].to - nodes[node.left].from + 1;
    }
    int right = 0;
    if (node.right != -1)
      right = update(node.right, pos, value);

    node.value = (left + right * base_powers[left_len]) % mod;
    return node.value;
  }

  int get(int from, int to) {
    return get(root, from, to).first;
  }

  int get_slow(int from, int to) {
    int value = 0;
    // for (int i = to; i >= from; i--)
    //   value = (value * base + values[i]) % mod;
    return value;
  }

  pair<int, int> get(int u, int from, int to) {
    Node& node = nodes[u];
    if (from > node.to || node.from > to)
      return make_pair(0, 0);
    if (from <= node.from && to >= node.to)
      return make_pair(node.value, node.to - node.from + 1);

    auto left = make_pair(0, 0);
    if (node.left != -1)
      left = get(node.left, from, to);
    auto right = make_pair(0, 0);
    if (node.right != -1)
      right = get(node.right, from, to);

    return make_pair(
      (left.first + right.first * base_powers[left.second]) % mod,
      left.second + right.second
    );
  }
};

void solve(Input& in, Output& out) {
  auto s = in.ns();
  int n = s.size();

  vec<Hash> hashes_left;
  hashes_left.push_back(Hash(n, 31, 41617));
  hashes_left.push_back(Hash(n, 37, 43997));
  // hashes_left.push_back(Hash(n, 47, 44753));
  // hashes_left.push_back(Hash(n, 53, 43003));

  vec<Hash> hashes_right;
  hashes_right.push_back(Hash(n, 31, 41617));
  hashes_right.push_back(Hash(n, 37, 43997));
  // hashes_right.push_back(Hash(n, 47, 44753));
  // hashes_right.push_back(Hash(n, 53, 43003));

  auto update = [&] (int pos, int c) {
    // out("update", pos, n - 1 - pos, c);
    for (auto& hash : hashes_right)
      hash.update(pos, c);
    for (auto& hash : hashes_left)
      hash.update(n - 1 - pos, c);
  };

  auto check_hashes = [&] (int right_start, int right_end, int left_end, int left_start) {
    // out("check", right_start, right_end, left_end, left_start);
    for (int i = 0; i < hashes_right.size(); i++) {
      auto right = hashes_right[i].get(right_start, right_end);
      auto left = hashes_left[i].get(n - 1 - left_end, n - 1 - left_start);
      if (left != right)
        return false;
    }
    return true;
  };

  for (int i = 0; i < n; i++)
    update(i, s[i] - 'a');

  int m = in.ni();
  for (int i = 0; i < m; i++) {
    auto query = in.ns();
    if (query == "change") {
      int pos = in.ni() - 1;
      int c = in.ns()[0] - 'a';
      update(pos, c);
    } else {
      int l = in.ni() - 1;
      int r = in.ni() - 1;
      int len = r - l + 1;
      int mid = l + len / 2;
      bool is_palindrome;
      if (len % 2) {
        is_palindrome = check_hashes(mid + 1, r, mid - 1, l);
      } else {
        is_palindrome = check_hashes(mid, r, mid - 1, l);
      }
      out(is_palindrome ? "Yes" : "No");
    }
  }
}
