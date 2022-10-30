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

void solve(Input& in, Output& out) {
  int vadim = 0;
  int lesha_e = 0;
  int misha = 0;

  // alarm
  // Боевым крещением стал чемпионат УрГУ
  vadim++;
  lesha_e++;
  misha++;
  // четвертьфинал чемпионата мира
  vadim++;
  lesha_e++;
  misha++;
  // полуфинал в Санкт-Петербург
  vadim++;
  lesha_e++;
  misha++;
  // команда выступила на чемпионате Урала
  vadim++;
  lesha_e++;
  misha++;
  // чемпионат Татарстана
  vadim++;
  lesha_e++;
  misha++;

  // На эту роль определили Сашу
  int sasha = 0;

  // Team.GOV
  // Пришло время чемпионата УрГУ, на контест не пришёл Саша
  vadim++;
  lesha_e++;
  // На четвертьфинале Саша появился
  vadim++;
  lesha_e++;
  sasha++;

  // место Саши занял Ваня Б
  int vanya_b = 0;
  // поехала на полуфинал от УрГУ
  vadim++;
  lesha_e++;
  vanya_b++;

  // был приглашён Никита
  int nikita = 0;
  // На предшествующем чемпионату отборе
  vadim++;
  lesha_e++;
  nikita++;
  // на чемпионат Урала, где почему-то не оказалось Лёши
  vadim++;
  nikita++;

  // Четверокурсник Федя был очень опытен
  int fedya = 0;
  // двенадцатое место на четвертьфинале
  vadim++;
  lesha_e++;
  fedya++;
  // стала третьей на чемпионате УрГУ.
  vadim++;
  lesha_e++;
  fedya++;
  // Полуфинал принёс диплом
  vadim++;
  lesha_e++;
  fedya++;

  // Битве поколений. Правда, Лёша Е. не смог прийти. вместо него взяли Ваню К
  int vanya_k = 0;
  vadim++;
  vanya_k++;
  fedya++;

  // Никита, узнав об уходе Лёши, с радостью откликнулся на приглашение в команду
  // отбор на чемпионат Урала, Федя с Никитой отыграли отбор вдвоём
  fedya++;
  nikita++;
  // Чемпионат Урала
  vadim++;
  fedya++;
  nikita++;

  // Ден принят
  int den = 0;
  // остался в одиночестве Егор
  int egor = 0;
  // На чемпионате УрГУ
  vadim++;
  den++;
  egor++;
  // Четвертьфинал отыграли
  vadim++;
  den++;
  egor++;
  // Всесибирская олимпиада
  vadim++;
  den++;
  egor++;
  // На полуфинале
  vadim++;
  den++;
  egor++;

  // приглашает в команду китаянку Сяохун
  int syaohun = 0;
  // взял на игровой тур Виталия
  int vitaliy = 0;
  // Поэтому Вадик по старой дружбе взял в команду Сашу
  vadim++;
  syaohun++;
  sasha++;

  // Тимоте и Александр
  int timote = 0;
  int alexander = 0;
  // поездку в Валенсию на SWERC
  vadim++;
  timote++;
  alexander++;

  vec<int> all({ misha, vadim, lesha_e, sasha, vanya_b, nikita, fedya, vanya_k, den, egor, syaohun, vitaliy, timote, alexander });
  int i = in.ni();
  out(all[i]);
}
