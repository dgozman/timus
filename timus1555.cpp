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
using u8 = uint8_t;

struct Input {
  Input(istream &in) : in(&in) {}
  istream *in;
  template<class T> T next() const { T x; *in >> x; return x; }
  int ni() const { return next<int>(); }
  i64 ni64() const { return next<i64>(); }
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
  const int south = 0;
  const int west = 1;
  const int east = 2;
  const int north = 3;
  vec<int> drow({ 1,  0, 0, -1 });
  vec<int> dcol({ 0, -1, 1,  0 });
  vec<int> dreverse({ north, east, west, south });
  const int pass = 8;
  const int unknown = 1000000000;

  int n = stoi(in.getline());

  vec<vec<int>> cells(n * n, vec<int>(4, -1));
  vec<int> walls;
  int start0row = 0, start0col = 0, start1row = 0, start1col = 0, chestRow = 0, chestCol = 0;
  for (int row = 0; row < n; row++) {
    string s1 = in.getline();
    string s2 = in.getline();
    string s3 = in.getline();
    for (int col = 0; col < n; col++) {
      int index = row * n + col;
      if (s1[col * 3 + 1] == '-') {
        cells[index][north] = walls.size();
        walls.push_back(index * 4 + north);
      }
      if (s3[col * 3 + 1] == '-') {
        cells[index][south] = walls.size();
        walls.push_back(index * 4 + south);
      }
      if (s2[col * 3] == '|') {
        cells[index][west] = walls.size();
        walls.push_back(index * 4 + west);
      }
      if (s2[col * 3 + 2] == '|') {
        cells[index][east] = walls.size();
        walls.push_back(index * 4 + east);
      }
      if (s2[col * 3 + 1] == '*') {
        chestRow = row;
        chestCol = col;
      }
      if (s2[col * 3 + 1] == '1') {
        start0row = row;
        start0col = col;
      }
      if (s2[col * 3 + 1] == '2') {
        start1row = row;
        start1col = col;
      }
    }
  }
  const int hasRocket = walls.size();
  const int noRocket = walls.size() + 1;
  const int wallCount = walls.size() + 2;

  const int noOwner = 2;
  auto swapChestOwner = [&] (int chestOwner) {
    return chestOwner == noOwner ? noOwner : 1 - chestOwner;
  };

  const int vs = n * n * n * n * wallCount * wallCount * 3;
  auto encode = [&] (int row0, int col0, int row1, int col1, int wall0, int wall1, int chestOwner) {
    return row0 + n * (col0 + n * (row1 + n * (col1 + n * (wall0 + wallCount * (wall1 + wallCount * chestOwner)))));
  };
  auto decode = [&] (int pos, int& row0, int& col0, int& row1, int& col1, int& wall0, int& wall1, int& chestOwner) {
    row0 = pos % n;
    pos /= n;
    col0 = pos % n;
    pos /= n;
    row1 = pos % n;
    pos /= n;
    col1 = pos % n;
    pos /= n;
    wall0 = pos % wallCount;
    pos /= wallCount;
    wall1 = pos % wallCount;
    chestOwner = pos / wallCount;
  };

  const int hitPlayer = wallCount;
  auto hit = [&] (int row, int col, int d, int wall0, int wall1, int rowOther, int colOther) {
    // Either wall, noRocket or hitPlayer.
    int r = row;
    int c = col;
    while (r >= 0 && r < n && c >= 0 && c < n) {
      int index = r * n + c;
      if (r != row || c != col) {
        int wall = cells[index][dreverse[d]];
        if (wall != -1 && wall != wall0 && wall != wall1)
          return wall;
      }
      if (r == rowOther && c == colOther)
        return hitPlayer;
      int wall = cells[index][d];
      if (wall != -1 && wall != wall0 && wall != wall1)
        return wall;
      r += drow[d];
      c += dcol[d];
    }
    return noRocket;
  };

  int maxMoves = 2 * 4 + 1 + 4;
  auto generateIncomingMoves = [&] (int pos, vec<int>& fromPos, vec<int>& fromMove) {
    fromPos.resize(0);
    fromMove.resize(0);

    int row0, col0, row1, col1, wall0, wall1, chestOwner;
    decode(pos, row0, col0, row1, col1, wall0, wall1, chestOwner);

    for (int d = 0; d < 4; d++) {
      int row = row1 - drow[d];
      int col = col1 - dcol[d];
      if (row < 0 || row >= n || col < 0 || col >= n)
        continue;
      if (row == row0 && col == col0)
        continue;
      if (row == chestRow && col == chestCol && chestOwner == noOwner)
        continue;
      int index = row * n + col;
      int wall = cells[index][d];
      if (wall != -1 && wall != wall0 && wall != wall1)
        continue;
      index = row1 * n + col1;
      wall = cells[index][dreverse[d]];
      if (wall != -1 && wall != wall0 && wall != wall1)
        continue;

      fromMove.push_back(d);
      fromPos.push_back(encode(row, col, row0, col0, wall1, wall0, swapChestOwner(chestOwner)));
      if (chestRow == row1 && chestCol == col1 && chestOwner == 1) {
        fromMove.push_back(d);
        fromPos.push_back(encode(row, col, row0, col0, wall1, wall0, noOwner));
      }
    }

    fromMove.push_back(pass);
    fromPos.push_back(encode(row1, col1, row0, col0, wall1, wall0, swapChestOwner(chestOwner)));

    if (wall1 != hasRocket) {
      for (int d = 0; d < 4; d++) {
        int hitTarget = hit(row1, col1, d, wall0, -1, row0, col0);
        if (hitTarget == hitPlayer)
          continue;
        if (hitTarget == wall1) {
          fromMove.push_back(d + 4);
          fromPos.push_back(encode(row1, col1, row0, col0, hasRocket, wall0, swapChestOwner(chestOwner)));
        }
      }
    }
  };

  vec<i32> dist(vs, unknown);
  vec<u8> degree(vs, 0);
  vec<u8> winningMove(vs, 0);
  vec<i32> q;
  q.reserve(vs);

  vec<int> fromPos(maxMoves);
  vec<int> fromMove(maxMoves);

  for (int pos = 0; pos < vs; pos++) {
    int row0, col0, row1, col1, wall0, wall1, chestOwner;
    decode(pos, row0, col0, row1, col1, wall0, wall1, chestOwner);
    if (row0 == row1 && col0 == col1)
      continue;
    if (row0 == chestRow && col0 == chestCol && chestOwner == noOwner)
      continue;
    if (row1 == chestRow && col1 == chestCol && chestOwner == noOwner)
      continue;
    int winning = -1;
    if (chestOwner == 0) {
      int index = row0 * n + col0;
      for (int d = 0; d < 4; d++) {
        if (row0 + drow[d] < 0 || row0 + drow[d] >= n || col0 + dcol[d] < 0 || col0 + dcol[d] >= n) {
          int wall = cells[index][d];
          if (wall == -1 || wall == wall0 || wall == wall1)
            winning = d;
        }
      }
    }
    if (wall0 == hasRocket) {
      for (int d = 0; d < 4; d++) {
        int hitTarget = hit(row0, col0, d, wall0, wall1, row1, col1);
        if (hitTarget == hitPlayer)
          winning = d + 4;
      }
    }
    if (winning != -1) {
      dist[pos] = 1;
      winningMove[pos] = winning;
      q.push_back(pos);
    }

    generateIncomingMoves(pos, fromPos, fromMove);
    for (int from : fromPos)
      degree[from]++;
  }

  int current = 0;
  while (current < q.size()) {
    int pos = q[current++];
    int dpos = dist[pos];
    generateIncomingMoves(pos, fromPos, fromMove);
    for (int i = 0; i < fromPos.size(); i++) {
      int from = fromPos[i];
      if (dpos < 0) {
        if (dist[from] == unknown) {
          dist[from] = -dpos + 1;
          winningMove[from] = fromMove[i];
          q.push_back(from);
        } else if (dist[from] < 0) {
          panic();
        }
      } else {
        degree[from]--;
        if (degree[from] == 0) {
          if (dist[from] < 0)
            panic();
          if (dist[from] == unknown) {
            dist[from] = -(dpos + 1);
            q.push_back(from);
          }
        } else if (degree[from] < 0) {
          panic();
        }
      }
    }
  }

  int startPos = encode(start0row, start0col, start1row, start1col, hasRocket, hasRocket, noOwner);
  if (dist[startPos] == unknown) {
    out("Draw");
  } else if (dist[startPos] > 0) {
    out.print("Win ");
    out(dist[startPos]);
    int move = winningMove[startPos];
    if (move == pass) {
      out("M5");
    } else if (move < 4) {
      out.print("M");
      out(move * 2 + 2);
    } else {
      out.print("S");
      out((move - 4) * 2 + 2);
    }
  } else {
    out("Lose 1");
    out("S5");
  }
}
