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

struct IntervalTree {
 private:
  // Type of the item stored in each cell.
  // Default constructor should be "zero".
  using ItemType = int;

  // Zero value for the item, of ItemType.
  static const ItemType ZeroItem = 0;

  // Updates a single cell by adding delta.
  static void AddToItem(ItemType& v, const ItemType& delta) {
    v += delta;
  }

  // Calculated type over a range of cells.
  // Default constructor should be "zero".
  struct RangeType {
    i64 left, right, full, sum;
    int from, to;
  };
  struct OptionalRange {
    RangeType r;
    bool valid;
    OptionalRange(): valid(false) {}
    explicit OptionalRange(const RangeType& r): r(r), valid(true) {}
  };


  // Updates the range by adding delta, range is [frame, to] inclusive.
  static void AddToRange(RangeType& r, const ItemType& delta) {
    i64 n = r.to - r.from + 1;
    i64 sq = n * (n + 1) / 2;
    r.left += sq * delta;
    r.right += sq * delta;
    r.full += n * (2 + n * (3 + n)) / 6 * delta;
    r.sum += n * delta;
  }

  // Merges two consecutive ranges (two.from == one.to + 1).
  static OptionalRange MergeOptionalRanges(const OptionalRange& one, const OptionalRange& two) {
    if (!one.valid)
      return two;
    if (!two.valid)
      return one;
    i64 n_one = one.r.to - one.r.from + 1;
    i64 n_two = two.r.to - two.r.from + 1;
    RangeType r;
    r.sum = one.r.sum + two.r.sum;
    r.left = one.r.left + two.r.left + one.r.sum * n_two;
    r.right = one.r.right + two.r.right + two.r.sum * n_one;
    r.full = one.r.full + two.r.full + one.r.right * n_two + two.r.left * n_one;
    r.from = one.r.from;
    r.to = two.r.to;
    return OptionalRange(r);
  }

 public:
  void update_range(int from, int to, const ItemType& delta) {
    update_range(root, from, to, delta);
  }

  RangeType query_range(int from, int to) {
    return query_range(root, from, to).r;
  }

  IntervalTree(int n) {
    size = 1;
    while (size < n)
      size = size * 2;
    nodes.resize(size * 2);

    int nc = 0;
    function<int(int, int)> build = [&] (int from, int to) {
      if (from > to)
        return -1;
      int index = nc++;
      int mid = (from + to) / 2;
      Node& node = nodes[index];
      node.r.from = from;
      node.r.to = to;
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

 private:
  struct Node {
    int left, right;
    ItemType move_down;
    RangeType r;
  };

  int size, root;
  vec<Node> nodes;

  RangeType update_range(int u, int from, int to, const ItemType& delta) {
    Node& node = nodes[u];
    if (from > node.r.to || node.r.from > to)
      return node.r;
    if (from <= node.r.from && to >= node.r.to) {
      AddToRange(node.r, delta);
      AddToItem(node.move_down, delta);
      return node.r;
    }
    move_down(node);
    OptionalRange left;
    if (node.left != -1)
      left = OptionalRange(update_range(node.left, from, to, delta));
    OptionalRange right;
    if (node.right != -1)
      right = OptionalRange(update_range(node.right, from, to, delta));
    node.r = MergeOptionalRanges(left, right).r;
    return node.r;
  }

  void move_down(Node& node) {
    if (node.move_down == ZeroItem)
      return;
    if (node.left != -1)
      update_range(node.left, node.r.from, node.r.to, node.move_down);
    if (node.right != -1)
      update_range(node.right, node.r.from, node.r.to, node.move_down);
    node.move_down = ZeroItem;
  }

  OptionalRange query_range(int u, int from, int to) {
    Node& node = nodes[u];
    if (from > node.r.to || node.r.from > to)
      return OptionalRange();
    if (from <= node.r.from && to >= node.r.to)
      return OptionalRange(node.r);
    move_down(node);
    OptionalRange left;
    if (node.left != -1)
      left = query_range(node.left, from, to);
    OptionalRange right;
    if (node.right != -1)
      right = query_range(node.right, from, to);
    return MergeOptionalRanges(left, right);
  }
};

void solve(Input& in, Output& out) {
  out.setPrecision(10);
  int n = in.ni();
  IntervalTree tree(n - 1);
  for (int m = in.ni(); m > 0; m--) {
    if (in.ns() == "change") {
      int from = in.ni() - 1;
      int to = in.ni() - 2;
      int delta = in.ni();
      tree.update_range(from, to, delta);
    } else {
      int from = in.ni() - 1;
      int to = in.ni() - 2;
      auto range = tree.query_range(from, to);
      i64 n = to - from + 1;
      n = n * (n + 1) / 2;
      double result = 1.0 * range.full / n;
      out(result);
    }
  }
}
