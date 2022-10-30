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

enum class Op {
  print = 0,
  assign = 1,
  add_assign = 2,
  sub_assign = 3,
  mul_assign = 4,
  div_assign = 5,
  mod_assign = 6,
  or_assign = 7,
  and_assign = 8,
  xor_assign = 9,
  not_assign = 10,
  just_goto = 11,
  eq_goto = 12,
  ne_goto = 13,
  ge_goto = 14,
  gt_goto = 15,
  le_goto = 16,
  lt_goto = 17,
  end = 18,
};
struct Varnum {
  string s;
  int var; // -1 means num
  i32 num;
};
struct Command {
  Op op;
  Varnum op1, op2;
  string label;
  int goto_target;
  int assign;
};

string to_lower(string s) {
  std::transform(s.begin(), s.end(), s.begin(), [] (char c) {
    return (c >= 'A' && c <= 'Z') ? c - 'A' + 'a' : c;
  });
  return s;
}

bool is_op(char c) {
  return c == '=' || c == '>' || c == '<' || c == '!';
}

i32 mod(i32 a, i32 b) {
  i32 r = a % b;
  if (r < 0)
    r += abs(b);
  return r;
}

void solve(Input& in, Output& out) {
  vec<Command> commands;
  map<string, int> vars;
  vec<int> values;
  vec<int> init;
  map<string, int> labels;

  auto ensure_var = [&] (const string& var) -> int {
    auto it = vars.find(var);
    if (it == vars.end()) {
      vars[var] = values.size();
      values.push_back(0);
      init.push_back(0);
      return values.size() - 1;
    }
    return it->second;
  };

  Command next;
  auto finish = [&] (Op op) {
    next.op = op;
    commands.push_back(next);
    next = Command();
  };

  vec<string> buf;
  string s;
  auto read = [&] () {
    if (buf.empty()) {
      string line;
      if (!(*in.in >> line)) {
        s = "";
        return s;
      }
      bool cur_op = is_op(line[0]);
      int start = 0;
      for (int i = 1; i < line.length(); i++) {
        bool next_op = is_op(line[i]);
        if (next_op != cur_op) {
          buf.push_back(line.substr(start, i - start));
          start = i;
        }
        cur_op = next_op;
      }
      buf.push_back(line.substr(start));
      std::reverse(buf.begin(), buf.end());
    }

    s = buf[buf.size() - 1];
    buf.pop_back();
    return s;
  };

  read();
  while (s != "") {
    if (s[s.length() - 1] == ':') {
      string label = s.substr(0, s.length() - 1);
      labels[to_lower(label)] = commands.size();
      read();
      continue;
    }
    if (to_lower(s) == "end") {
      finish(Op::end);
      read();
      continue;
    }
    if (to_lower(s) == "print") {
      next.op1.s = read();
      finish(Op::print);
      read();
      continue;
    }
    if (to_lower(s) == "goto") {
      next.label = to_lower(read());
      finish(Op::just_goto);
      read();
      continue;
    }
    if (to_lower(s) == "if") {
      next.op1.s = read();
      s = read();
      if (s == "==")
        next.op = Op::eq_goto;
      else if (s == "!=")
        next.op = Op::ne_goto;
      else if (s == ">=")
        next.op = Op::ge_goto;
      else if (s == ">")
        next.op = Op::gt_goto;
      else if (s == "<=")
        next.op = Op::le_goto;
      else if (s == "<")
        next.op = Op::lt_goto;
      else
        panic();
      next.op2.s = read();
      if (to_lower(read()) != "goto")
        panic();
      next.label = to_lower(read());
      finish(next.op);
      read();
      continue;
    }

    string var = s;
    next.assign = ensure_var(var);
    if (read() != "=")
      panic();
    read();
    if (to_lower(s) == "not") {
      next.op1.s = read();
      finish(Op::not_assign);
      read();
      continue;
    }

    next.op1.s = s;
    read();
    if (s == "+") {
      next.op = Op::add_assign;
    } else if (s == "-") {
      next.op = Op::sub_assign;
    } else if (s == "*") {
      next.op = Op::mul_assign;
    } else if (s == "/") {
      next.op = Op::div_assign;
    } else if (s == "%") {
      next.op = Op::mod_assign;
    } else if (to_lower(s) == "or") {
      next.op = Op::or_assign;
    } else if (to_lower(s) == "and") {
      next.op = Op::and_assign;
    } else if (to_lower(s) == "xor") {
      next.op = Op::xor_assign;
    } else {
      finish(Op::assign);
      continue;
    }
    next.op2.s = read();
    finish(next.op);
    read();
  }

  auto process_varnum = [&] (Varnum& v) {
    if (v.s == "")
      return;
    auto it = vars.find(v.s);
    if (it == vars.end()) {
      v.var = -1;
      v.num = std::stoi(v.s);
    } else {
      v.var = it->second;
    }
  };

  for (auto& command : commands) {
    if (command.label != "")
      command.goto_target = labels[command.label];
    process_varnum(command.op1);
    process_varnum(command.op2);
  }

  auto get_value = [&] (const Varnum& v) {
    return v.var == -1 ? v.num : values[v.var];
  };

  const int max_counter = 10000000;
  int ip = 0;
  int counter = 0;
  bool finished = false;
  while (counter < max_counter && ip < commands.size()) {
    counter++;
    auto& command = commands[ip];
    if (command.op == Op::print) {
      out(get_value(command.op1));
      ip++;
    } else if (command.op == Op::assign) {
      values[command.assign] = get_value(command.op1);
      init[command.assign] = 1;
      ip++;
    } else if (command.op == Op::add_assign) {
      values[command.assign] = get_value(command.op1) + get_value(command.op2);
      init[command.assign] = 1;
      ip++;
    } else if (command.op == Op::sub_assign) {
      values[command.assign] = get_value(command.op1) - get_value(command.op2);
      init[command.assign] = 1;
      ip++;
    } else if (command.op == Op::mul_assign) {
      values[command.assign] = get_value(command.op1) * get_value(command.op2);
      init[command.assign] = 1;
      ip++;
    } else if (command.op == Op::div_assign) {
      values[command.assign] = get_value(command.op1) / get_value(command.op2);
      init[command.assign] = 1;
      ip++;
    } else if (command.op == Op::mod_assign) {
      values[command.assign] = mod(get_value(command.op1), get_value(command.op2));
      init[command.assign] = 1;
      ip++;
    } else if (command.op == Op::or_assign) {
      values[command.assign] = get_value(command.op1) | get_value(command.op2);
      init[command.assign] = 1;
      ip++;
    } else if (command.op == Op::and_assign) {
      values[command.assign] = get_value(command.op1) & get_value(command.op2);
      init[command.assign] = 1;
      ip++;
    } else if (command.op == Op::xor_assign) {
      values[command.assign] = get_value(command.op1) ^ get_value(command.op2);
      init[command.assign] = 1;
      ip++;
    } else if (command.op == Op::not_assign) {
      values[command.assign] = ~get_value(command.op1);
      init[command.assign] = 1;
      ip++;
    } else if (command.op == Op::just_goto) {
      ip = command.goto_target;
    } else if (command.op == Op::eq_goto) {
      ip = get_value(command.op1) == get_value(command.op2) ? command.goto_target : ip + 1;
    } else if (command.op == Op::ne_goto) {
      ip = get_value(command.op1) != get_value(command.op2) ? command.goto_target : ip + 1;
    } else if (command.op == Op::ge_goto) {
      ip = get_value(command.op1) >= get_value(command.op2) ? command.goto_target : ip + 1;
    } else if (command.op == Op::gt_goto) {
      ip = get_value(command.op1) > get_value(command.op2) ? command.goto_target : ip + 1;
    } else if (command.op == Op::le_goto) {
      ip = get_value(command.op1) <= get_value(command.op2) ? command.goto_target : ip + 1;
    } else if (command.op == Op::lt_goto) {
      ip = get_value(command.op1) < get_value(command.op2) ? command.goto_target : ip + 1;
    } else if (command.op == Op::end) {
      finished = true;
      break;
    } else {
      panic();
    }
  }

  if (!finished && counter == max_counter) {
    out("Program terminated. Variables state:");
    for (const auto& pair : vars) {
      if (init[pair.second]) {
        out.print(pair.first);
        out.print(": ");
        out(values[pair.second]);
      }
    }
  }
}
