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

void solve(Input& in, Output& out) {
  int small_blind = in.ni();
  int big_blind = in.ni();
  int ante = in.ni();
  int n = in.ni();
  int dealer = in.ni() - 1;
  int bot_player = in.ni() - 1;
  string target_street = in.ns();

  auto next = [&] (int p, int add = 1) {
    return (p + add) % n;
  };

  auto next_street = [&] (string street) {
    if (street == "preflop") return "flop";
    if (street == "flop") return "turn";
    if (street == "turn") return "river";
    panic();
    return "";
  };

  vec<string> players(n);
  vec<int> stacks(n);
  for (int i = 0; i < n; i++) {
    players[i] = in.ns();
    stacks[i] = in.ni();
  }

  vec<int> bets(n, 0);
  vec<bool> folded(n, false);

  vec<int> current_bets(n, 0);
  int big_blind_player;
  int current_bet = big_blind;
  int current_player = 0;
  int expected_move_count = n;
  string current_street = "preflop";

  auto fold = [&] (int p) {
    if (folded[p])
      panic();
    folded[p] = true;
    expected_move_count--;
    return players[p] + " folds";
  };

  auto maybe_all_in = [&] (int p) {
    if (bets[p] + current_bets[p] >= stacks[p]) {
      folded[p] = true;
      return true;
    }
    return false;
  };

  auto all_equal = [&] () {
    for (int i = 0; i < n; i++) {
      if (!folded[i] && current_bets[i] != current_bet)
        return false;
    }
    return true;
  };

  auto is_time_to_deal = [&] () {
    return expected_move_count <= 0 && all_equal();
  };

  auto can_check = [&] (int p) {
    bool regular_check = current_bet == 0;
    bool big_blind_check = current_bet == big_blind && current_street == "preflop" && p == big_blind_player;
    return regular_check || big_blind_check;
  };

  auto bet_up_to = [&] (int p, int b) {
    if (b < current_bet)
      panic();
    if (folded[p])
      panic();
    if (b == current_bets[p] && !can_check(p))
      panic();

    int actual_bet = min(stacks[p] - bets[p], b);
    string result;
    if (b == current_bets[p])
      result = players[p] + " checks";
    else if (current_bet == 0)
      result = players[p] + " bets " + to_string(actual_bet);
    else if (b == current_bet)
      result = players[p] + " calls " + to_string(actual_bet - current_bets[p]);
    else
      result = players[p] + " raises " + to_string(b - current_bet) + " to " + to_string(b);

    current_bet = b;
    current_bets[p] = actual_bet;
    maybe_all_in(p);
    expected_move_count--;
    return result;
  };

  auto deal_next = [&] () {
    if (!is_time_to_deal())
      panic();
    if (!all_equal())
      panic();
    expected_move_count = n;
    for (int i = 0; i < n; i++) {
      bets[i] += current_bets[i];
      current_bets[i] = 0;
      if (folded[i])
        expected_move_count--;
    }
    current_player = next(dealer);
    current_bet = 0;
    current_street = next_street(current_street);
    return "dealing " + current_street;
  };

  for (int i = 0; i < n; i++)
    bets[i] = min(ante, stacks[i]);
  int small_blind_player = n == 2 ? dealer : next(dealer);
  current_bets[small_blind_player] += min(small_blind, stacks[small_blind_player] - bets[small_blind_player]);
  big_blind_player = next(small_blind_player);
  current_bets[big_blind_player] += min(big_blind, stacks[big_blind_player] - bets[big_blind_player]);
  current_player = next(big_blind_player);
  expected_move_count = n;
  for (int i = 0; i < n; i++) {
    if (maybe_all_in(i))
      expected_move_count--;
  }

  int actions = in.ni();
  for (int i = 0; i < actions; i++) {
    string player = in.ns();
    if (player == "dealing") {
      deal_next();
      if (current_street != in.ns())
        panic();
      continue;
    }
    while (folded[current_player])
      current_player = next(current_player);
    if (player != players[current_player])
      panic();

    string action = in.ns();
    if (action == "folds") {
      fold(current_player);
    } else if (action == "checks") {
      if (bet_up_to(current_player, current_bet) != player + " checks")
        panic();
    } else if (action == "calls") {
      int amount = in.ni();
      int up_to = max(current_bet, current_bets[current_player] + amount);
      if (bet_up_to(current_player, up_to) != player + " calls " + to_string(amount))
        panic();
    } else if (action == "bets") {
      int amount = in.ni();
      int up_to = max(current_bet, current_bets[current_player] + amount);
      if (bet_up_to(current_player, up_to) != player + " bets " + to_string(amount))
        panic();
    } else if (action == "raises") {
      int amount = in.ni();
      if (in.ns() != "to")
        panic();
      int total = in.ni();
      if (bet_up_to(current_player, current_bet + amount) != player + " raises " + to_string(amount) + " to " + to_string(total))
        panic();
    } else {
      panic();
    }
    current_player = next(current_player);
  }

  vec<int> target_stacks = in.nvi(n);

  auto stacks_equal = [&] () {
    bool all_equal = true;
    for (int i = 0; i < n; i++) {
      if (target_stacks[i] != stacks[i] - bets[i] - current_bets[i]) {
        if (folded[i])
          panic();
        all_equal = false;
      }
    }
    return all_equal;
  };

  vec<string> result;
  bool seen_bot_move = false;
  bool dealed = false;
  while (current_player != bot_player || current_street != target_street || !stacks_equal()) {
    if (is_time_to_deal()) {
      dealed = true;
      result.push_back(deal_next());
      continue;
    }
    if (folded[current_player]) {
      current_player = next(current_player);
      continue;
    }
    int target = stacks[current_player] - bets[current_player] - target_stacks[current_player];
    if (seen_bot_move && !dealed && current_street != target_street)
      target = min(target, current_bet);
    if (target > current_bets[current_player] && target < current_bet)
      target = current_bet;
    if (target < current_bets[current_player])
      panic();
    if (target == current_bets[current_player]) {
      if (can_check(current_player))
        result.push_back(bet_up_to(current_player, target));
      else
        result.push_back(fold(current_player));
    } else {
      result.push_back(bet_up_to(current_player, target));
    }
    if (current_player == bot_player)
      seen_bot_move = true;
    current_player = next(current_player);
  }
  if (current_street != target_street)
    panic();

  out(result.size());
  for (auto& s : result)
    out(s);
}
