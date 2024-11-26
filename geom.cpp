// long double section

using D = long double;
const D eps = 1e-10;
const D PI = 3.1415926535897932384626433832795028841971;

D sqrt_(D x) {
  if (x > -eps && x < 0)
    return 0;
  return sqrt(x);
}

D abs_(D x) {
  return fabsl(x);
}

bool less_(D x, D y) {
  return x <= y - eps;
}

bool less_eq_(D x, D y) {
  return x < y + eps;
}

bool eq_(D x, D y) {
  return fabsl(x - p.x) < eps;
}

// rational section

i64 gcd_(i64 a, i64 b) {
  a = abs(a);
  b = abs(b);
  while (b > 0) {
    i64 c = a % b;
    a = b;
    b = c;
  }
  return a;
}

struct Rat {
  i64 p, q;
  Rat(): p(0), q(1) {}
  Rat(i64 x): p(x), q(1) {}
  Rat(i64 np, i64 nq): p(np), q(nq) {
    if (q == 0)
      panic();
    if (q < 0) {
      p = -p;
      q = -q;
    }
    i64 g = gcd_(p, q);
    p /= g;
    q /= g;
  }

  Rat abs() const {
    return Rat(abs(p), q);
  }

  Rat operator-() const {
    return Rat(-p, q);
  }
  Rat operator+(const Rat& other) const {
    return Rat(p * other.q + q * other.p, q * other.q);
  }
  Rat operator-(const Rat& other) const {
    return Rat(p * other.q - q * other.p, q * other.q);
  }
  Rat operator*(i64 d) const {
    return Rat(p * d, q);
  }
  Rat operator/(i64 d) const {
    return Rat(p, q * d);
  }
  Rat operator*(const Rat& other) const {
    return Rat(p * other.p, q * other.q);
  }
  Rat operator/(const Rat& other) const {
    return Rat(p * other.q, q * other.p);
  }
  bool operator<(const Rat& other) const {
    return p * other.q < q * other.p;
  }
  bool operator>(const Rat& other) const {
    return p * other.q > q * other.p;
  }
  bool operator==(const Rat& other) const {
    return p == other.p && q == other.q;
  }
  bool operator<=(const Rat& other) const {
    return p * other.q <= q * other.p;
  }
  bool operator>=(const Rat& other) const {
    return p * other.q >= q * other.p;
  }
};

Rat sqrt_(Rat x) {
  throw 42;
}

Rat abs_(Rat x) {
  return x.abs();
}

bool less_(Rat x, Rat y) {
  return x < y;
}

bool less_eq_(Rat x, Rat y) {
  return x <= y;
}

bool eq_(Rat x, Rat y) {
  return x == y;
}


// Change here!!!
using Num = D;
const Num zero = 0;
const Num one = 1;
// End of change here!!!


// common section

Num sqr_(Num x) {
  return x * x;
}

bool more_(Num x, Num y) {
  return !less_eq_(x, y);
}

bool more_eq_(Num x, Num y) {
  return !less_(x, y);
}

struct Point {
  Num x, y;
  Point(): x(zero), y(zero) {}
  Point(Num x, Num y): x(x), y(y) {}
  Point operator-(const Point& other) const {
    return Point(x - other.x, y - other.y);
  }
  Point operator+(const Point& other) const {
    return Point(x + other.x, y + other.y);
  }
  Point operator*(const Num& d) const {
    return Point(x * d, y * d);
  }
  Point operator/(const Num& d) const {
    return Point(x / d, y / d);
  }
  bool operator<(const Point& p) const {
    return less_(x, p.x) || (eq_(x, p.x) && less_(y, p.y));
  }
  Point rotate(Num cosa, Num sina) const {
    return Point(x * cosa - y * sina, y * cosa + x * sina);
  }
  Point rotate(Num ang) const {
    return rotate(cos(ang), sin(ang));
  }
  Num norm2() const {
    return sqr_(x) + sqr_(y);
  }
  Num norm() const {
    return sqrt_(norm2());
  }
  Num dot(const Point& p) const {
    return x * p.y - y * p.x;
  }
  void out(Output& out) const {
    out.println(x, y);
  }
};

struct Range {
  Num a, b;
  Range(): a(zero), b(zero) {}
  Range(Num l, Num r): a(min(l, r)), b(max(l, r)) {}
  bool contains_inclusive(D x) const {
    return less_eq_(a, x) && less_eq_(x, b);
  }
  bool contains_exclusive(D x) const {
    return less_(a, x) && less_(x, b);
  }
};

struct Line {
  Num a, b, c;
  Line(): a(one), b(one), c(zero) {}
  Line(D a, D b, D c): a(a), b(b), c(c) {}
  Line(const Point& p1, const Point& p2) {
    a = p1.y - p2.y;
    b = p2.x - p1.x;
    c = -a * p1.x - b * p1.y;
  }
  void out(Output& out) const {
    out.println(a, b, c);
  }
  Num apply(const Point& p) const {
    return a * p.x + b * p.y + c;
  }
  bool contains(const Point& p) const {
    return eq_(apply(p), zero);
  }
  Num norm() const {
    return sqrt_(norm2());
  }
  Num norm2() const {
    return sqr_(a) + sqr_(b);
  }
  Line shift_to(const Point& to, D d) const {
    return apply(to) < zero ? Line(a, b, c + d * norm()) : Line(a, b, c - d * norm());
  }
  Line shift_from(const Point& from, D d) const {
    return apply(from) > zero ? Line(a, b, c + d * norm()) : Line(a, b, c - d * norm());
  }
  Point orth(const Point& p) const {
    Num t = -apply(p) / norm2();
    return Point(a, b) * t + p;
  }
  bool same_side_inclusive(const Point& p, const Point& q) const {
    Num pv = apply(p);
    Num qv = apply(q);
    return (less_eq_(zero, pv) && less_eq_(zero, qv)) || (less_eq_(pv, zero) && less_eq_(qv, zero));
  }
  bool same_side_exclusive(const Point& p, const Point& q) const {
    Num pv = apply(p);
    Num qv = apply(q);
    return (less_(zero, pv) && less_(zero, qv)) || (less_(pv, zero) && less_(qv, zero));
  }
  bool intersect(const Line& line, Point& result) const {
    Num d = a * line.b - b * line.a;
    if (eq_(d, zero))
      return false;
    result.x = (-c * line.b + b * line.c) / d;
    result.y = (-a * line.c + c * line.a) / d;
    return true;
  }
};

struct Segment {
  Point p, q;
  Segment() {}
  Segment(const Point& p, const Point& q): p(p), q(q) {}
  Line line() const {
    return Line(p, q);
  }
  bool contains_inclusive(const Point& r) const {
    return line().contains(r) &&
      Range(p.x, q.x).contains_inclusive(r.x) &&
      Range(p.y, q.y).contains_inclusive(r.y);
  }
  bool contains_exclusive(const Point& r) const {
    return line().contains(r) &&
      Range(p.x, q.x).contains_exclusive(r.x) &&
      Range(p.y, q.y).contains_exclusive(r.y);
  }
  Point closest(const Point& r) const {
    Point o = line().orth(r);
    if (contains_inclusive(o))
      return o;
    return (p - r).norm2() < (q - r).norm2() ? p : q;
  }
  Num distance(const Point& r) const {
    return (r - closest(r)).norm();
  }
  bool intersect_inclusive(const Segment& s, Point& result) const {
    if (!line().intersect(s.line(), result))
      return false;
    return contains_inclusive(result) && s.contains_inclusive(result);
  }
};

struct Circle {
  Point c;
  Num r;
  Circle(): r(zero) {}
  Circle(const Point& c, D r): c(c), r(r) {}
  Circle(D x, D y, D r): c(x, y), r(r) {}
  void out(Output& out) const {
    out.println(c.x, c.y, r);
  }
  bool intersect(const Line& line, Segment& result) const {
    Num norm2 = line.norm2();
    Num t = -line.apply(c) / norm2;
    Num r2 = r * r - t * t * norm2;
    if (less_(r2, zero))
      return false;
    r2 = sqrt_(r2 / norm2);
    Point p(c.x + line.a * t, c.y + line.b * t);
    result = Segment(
      Point(p.x + line.b * r2, p.y - line.a * r2),
      Point(p.x - line.b * r2, p.y + line.a * r2)
    );
    return true;
  }
  bool intersect(const Circle& circle, Segment& result) const {
    Num la = 2 * (circle.c.x - c.x);
    Num lb = 2 * (circle.c.y - c.y);
    if (eq_(la, zero) && eq_(lb, zero))
      return false;
    Num lc = sqr_(circle.r) - sqr_(r) + sqr_(c.x) - sqr_(circle.c.x) + sqr_(c.y) - sqr_(circle.c.y);
    return intersect(Line(la, lb, lc), result);
  }
};

struct Polygon {
  vec<Point> p;
  int n = 0;
  bool closed = false;

  Polygon(): closed(false) {}
  Polygon& add(const Point& point) {
    if (closed)
      panic();
    p.push_back(point);
    return *this;
  }
  Polygon& close() {
    if (closed)
      panic();
    n = p.size();
    if (n)
      p.push_back(p[0]);
    closed = true;
    return *this;
  }
  void ensure_closed() const {
    if (!closed)
      panic();
  }
  bool empty() const {
    return !n;
  }

  Polygon(const vec<Point>& points) {
    p = points;
    close();
  }
  Polygon(vec<Point>&& points) {
    p = std::move(points);
    close();
  }

  Num convex_area() const {
    ensure_closed();
    Num area = 0;
    for (int i = 0; i < n; i++)
      area += p[i].dot(p[i + 1]);
    return abs_(area) / 2;
  }
  bool convex_contains_inclusive(const Point& q) const {
    ensure_closed();
    if (empty())
      return false;
    Point center;
    for (int i = 0; i < n; i++)
      center = center + p[i];
    center = center / n;
    for (int i = 0; i < n; i++) {
      if (!Line(p[i], p[i + 1]).same_side_inclusive(q, center))
        return false;
    }
    return true;
  }

  Polygon convex_intersect(const Polygon& poly) const {
    ensure_closed();
    poly.ensure_closed();
    if (empty() || poly.empty())
      return Polygon().close();
    vec<Point> points;
    for (int i = 0; i < n; i++) {
      if (poly.convex_contains_inclusive(p[i]))
        points.push_back(p[i]);
    }
    for (int i = 0; i < poly.n; i++) {
      if (convex_contains_inclusive(poly.p[i]))
        points.push_back(poly.p[i]);
    }
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < poly.n; j++) {
        Point r;
        if (Segment(p[i], p[i + 1]).intersect_inclusive(Segment(poly.p[j], poly.p[j + 1]), r))
          points.push_back(r);
      }
    }
    if (!points.size())
      return Polygon().close();
    Point mn = points[0];
    for (Point& ps : points)
      mn = ps < mn ? ps : mn;
    sort(points.begin(), points.end(), [&mn](const Point& a, const Point& b) -> bool {
      if (eq_(a.x, mn.x) && eq_(a.y, mn.y))
        return true;
      if (eq_(b.x, mn.x) && eq_(b.y, mn.y))
        return false;
      Num dot = (a - mn).dot(b - mn);
      if (more_(dot, zero))
        return true;
      if (less_(dot, zero))
        return false;
      return (a - mn).norm2() < (b - mn).norm2();
    });
    Polygon result;
    result.add(points[0]);
    for (int i = 0; i < points.size(); i++) {
      Point& a = result.p[result.p.size() - 1];
      Point& b = points[i];
      if (!eq_(a.x, b.x) || !eq_(a.y, b.y))
        result.add(b);
    }
    return result.close();
  }

  void bounding_box(Point& mn, Point& mx) {
    mn = p[0];
    mx = p[0];
    for (int k = 1; k < n; k++) {
      mn.x = min(mn.x, p[k].x);
      mx.x = max(mx.x, p[k].x);
      mn.y = min(mn.y, p[k].y);
      mx.y = max(mx.y, p[k].y);
    }
  }
};
