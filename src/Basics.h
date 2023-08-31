//
// Created by Yixiang Tu on 2019/5/21.
//

#ifndef LAB2_BASICS_H
#define LAB2_BASICS_H

#include <initializer_list>
#include <cmath>
#include <cassert>
#include <random>
#include <cstring>
#include <stdexcept>
#include <functional>
#include <iostream>
#include <vector>
#include <map>
#include <string>

#define NO_INTERSECTION (-1)
#define EMACH (1.0 / (1 << 26))

#define MAYBE(x) if (rnd() < 0.00001) { x; }

//static_assert(sizeof(double) == sizeof(double*));

static FILE* open(const std::string& filename) {
  return fopen(("src/" + filename).data(), "rb");
}

template<typename ...Args>
constexpr double max(double a, double b, Args&& ...args) {
  return max(a < b ? b : a, std::forward<Args&&>(args)...);
}

template<>
constexpr double max(double a, double b) {
  return a < b ? b : a;
}

template<typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
constexpr T max(T a, T b) {
  return a < b ? b : a;
};

template<typename ...Args>
constexpr double min(double a, double b, Args&& ...args) {
  return min(a < b ? a : b, std::forward<Args&&>(args)...);
}

template<>
constexpr double min(double a, double b) {
  return a < b ? a : b;
}

template<typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
constexpr T min(T a, T b) {
  return a < b ? a : b;
};

template<int N>
constexpr double pow(double x) {
  return x * pow<N - 1>(x);
}

template<>
constexpr double pow<1>(double x) {
  return x;
}

// 生成 0 ~ 1 之间随机数
double rnd();

// 生成 -1 ~ 1 之间随机数
double pn_rnd();

constexpr bool is0(double v) {
  return v < EMACH and v > -EMACH;
}

constexpr double clamp(double x) {
  return x < 0 ? 0 : x > 1 ? 1 : x;
}

static double safe_sqrt(double x) {
  if (x < EMACH) {
    return 1e-10;
  } else {
    return sqrt(x);
  }
}

static int fast_round(double x) {
  static double k;
  k = x + 6755399441055744.0;
  return *(int*) &k;
}

// fast sin using 7th polynomial fit, ~2x acceleration
// max relative error = 2.5e-6
static double fast_sin(double x) {
  auto k = fast_round(x * M_1_PI);
  x -= k * M_PI;
  auto x2 = x * x;
  auto z = 45.04512197443198 - x2;
  z = z * x2 - 903.4038153403246;
  z = z * x2 + 5420.896824160936;
  x *= z * .0001844708576170712;
  return k & 1 ? -x : x;
}

static double fast_cos(double x) {
  return fast_sin(x + M_PI_2);
}

static void fast_sin_cos(double theta, double& sin_theta, double& cos_theta) {
  sin_theta = fast_sin(theta);
  auto k = fast_round(theta * M_1_PI + .5);
  cos_theta = k & 1 ? -sqrt(1 - pow<2>(sin_theta)) : sqrt(1 - pow<2>(sin_theta));
}

static void gen_sin_cos(double& x, double& y) {
  auto v = rnd() * M_PI * 2;
  x = sin(v);
  y = cos(v);
}

enum DeflectionType {
  NoDeflection,
  Diffuse,
  Reflect,
  Refract,
};

/**
 * 一个向量，或者代表空间一个位置，或者代表一个颜色
 */
struct Vec {
  bool enabled{false};
  double x{0};
  double y{0};
  double z{0};
  double _length{-1};

  Vec() = default;

  Vec(double x) : enabled(true), x(x), y(x), z(x) {}

  Vec(double x, double y, double z) : enabled(not std::isnan(x)), x(x), y(y), z(z) {}

  // element-wise multiplication
  Vec operator*(double v) const {
    return {v * x, v * y, v * z};
  }

  // element-wise multiplication
  Vec& operator*=(double v) {
    *this = *this * v;
    return *this;
  }

  // element-wise division
  Vec operator/(double v) const {
    return {x / v, y / v, z / v};
  }

  // element-wise division
  Vec& operator/=(double v) {
    *this = *this / v;
    return *this;
  }

  // element-wise multiplication
  friend Vec operator*(double v, const Vec& vec) {
    return vec * v;
  }

  // dot product
  double operator*(const Vec& vec) const {
    auto result = x * vec.x + y * vec.y + z * vec.z;
    return is0(result) ? 0 : result;
  }

  // cross product
  Vec operator%(const Vec& vec) const {
    return {y * vec.z - z * vec.y, z * vec.x - x * vec.z, x * vec.y - y * vec.x};
  }

  // get orthogonal part
  Vec operator^(const Vec& vec) const {
    return *this - (*this * vec) * vec;
  }

  // element-wise multiplication
  Vec operator&(const Vec& vec) const {
    return {x * vec.x, y * vec.y, z * vec.z};
  }

  // element-wise multiplication
  Vec& operator&=(const Vec& vec) {
    *this = *this & vec;
    return *this;
  }

  // element-wise addition
  Vec operator+(const Vec& vec) const {
    return {x + vec.x, y + vec.y, z + vec.z};
  }

  // element-wise addition
  Vec& operator+=(const Vec& vec) {
    *this = *this + vec;
    return *this;
  }

  // element-wise subtraction
  Vec operator-(const Vec& vec) const {
    return {x - vec.x, y - vec.y, z - vec.z};
  }

  // element-wise subtraction
  Vec& operator-=(const Vec& vec) {
    *this = *this - vec;
    return *this;
  }

  // opposite
  Vec operator-() const {
    return {-x, -y, -z};
  }

  explicit operator bool() const {
    return enabled;
  }

  double length() {
    if (_length < 0) {
      _length = sqrt(squareLength());
    }
    return _length;
  }

  double length() const {
    if (_length < 0) {
      return sqrt(squareLength());
    } else {
      return _length;
    }
  }

  double squareLength() const {
    return x * x + y * y + z * z;
  }

  void normalize() {
    x /= length();
    y /= length();
    z /= length();
    _length = 1;
  }

  friend Vec normalized(const Vec& vec) {
    double len;
    if (vec._length < 0) {
      len = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
    } else {
      len = vec._length;
    }
    return {vec.x / len, vec.y / len, vec.z / len};
  }

  friend Vec normalized(Vec& vec) {
    return {vec.x / vec.length(), vec.y / vec.length(), vec.z / vec.length()};
  }

  friend double sum(const Vec& vec) {
    return vec.x + vec.y + vec.z;
  }

  friend double max(const Vec& vec) {
    return max(vec.x, vec.y, vec.z);
  }

  friend double min(const Vec& vec) {
    return min(vec.x, vec.y, vec.z);
  }

  friend double maxabs(const Vec& vec) {
    return max(fabs(vec.x), fabs(vec.y), fabs(vec.z));
  }

  friend std::ostream& operator<<(std::ostream& os, const Vec& vec) {
    os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
    return os;
  }

  double operator[](int i) const {
    return (&x)[i];
  }

  double& operator[](int i) {
    return (&x)[i];
  }

  static Vec random_unit() {
    double s, c;
    gen_sin_cos(s, c);
    auto h = pn_rnd();
    auto x = sqrt(1 - h * h);
    return {x * s, x * c, h};
  }

  static Vec uniform_random() {
    return {2 * rnd() - 1, 2 * rnd() - 1, 2 * rnd() - 1};
  }

  static Vec random() {
    return rnd() * random_unit();
  }

  // x in [0, 1]
  static Vec spectrum(double x);
};

struct Point {
  double x, y;

  Point() = default;

  Point(double x, double y) : x(x), y(y) {}

  Point operator+(const Point& other) const {
    return {x + other.x, y + other.y};
  }

  Point operator-(const Point& other) const {
    return {x - other.x, y - other.y};
  }

  Point operator*(double v) const {
    return {x * v, y * v};
  }
};

template<typename T>
struct Mat : public std::vector<std::vector<T>> {
  Mat() = default;

  Mat(unsigned width, unsigned height) : std::vector<std::vector<T>>(height, std::vector<T>(width, T{})) {}

  T& at(int x, int y) {
    assert(x >= 0 and x < width() and y >= 0 and y < height());
    return (*this)[y][x];
  }

  const T& at(int x, int y) const {
    assert(x >= 0 and x < width() and y >= 0 and y < height());
    return (*this)[y][x];
  }

  T& dat(double x, double y) {
    assert(x >= 0 and x <= 1 and y >= 0 and y <= 1);
    return at((int) (x * (width() - 1)), (int) (y * (height() - 1)));
  }

  const T& dat(double x, double y) const {
    assert(not(std::isnan(x) or std::isnan(y)));
    assert(x >= 0 and x <= 1 and y >= 0 and y <= 1);
    return at((int) (x * (width() - 1)), (int) (y * (height() - 1)));
  }

  const T& dat(const Point& p) const {
    return dat(p.x, p.y);
  }

  unsigned long width() const {
    return (*this)[0].size();
  }

  unsigned long height() const {
    return std::vector<std::vector<T>>::size();
  }

  void each(const std::function<void(T&)>& func) {
    for (auto& row: *this) {
      for (auto& val: row) {
        func(val);
      }
    }
  }
};

/**
 * 一个实数区间，用于包围盒
 */
struct Range {
  double left{INFINITY};
  double right{-INFINITY};

  Range() = default;

  Range(double left, double right) : left(left), right(right) {}

  Range(std::initializer_list<double>&& list) {
    for (auto v: list) {
      *this += v;
    }
  }

  Range operator+(const Range& other) const {
    return {min(left, other.left), max(right, other.right)};
  }

  Range& operator+=(const Range& other) {
    *this = (*this) + other;
    return *this;
  }

  Range& operator+=(double v) {
    left = left > v ? v : left;
    right = right < v ? v : right;
    return *this;
  }

  double length() const {
    return right - left;
  }

  bool operator<(double v) const {
    return right < v;
  }

  bool contain(double v) const {
    return v >= left and v <= right;
  }
};

/**
 * 包围盒
 */
struct AABB {
  Range x;
  Range y;
  Range z;

  AABB() = default;

  AABB(Range x, Range y, Range z) : x(x), y(y), z(z) {}

  AABB(const std::initializer_list<Vec>& list) {
    for (const auto& v: list) {
      x += v.x;
      y += v.y;
      z += v.z;
    }
  }

  AABB& operator+=(const AABB& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
  }

  AABB& operator+=(const Vec& vec) {
    x += vec.x;
    y += vec.y;
    z += vec.z;
    return *this;
  }

  Range& operator[](int i) {
    return (&x)[i];
  }

  const Range& operator[](int i) const {
    return (&x)[i];
  }

  // query whether the ray will intersect with this box
  // returns distance
  // returns 0 if the source is within the box
  // returns -1 if no intersection
  double intersect(const Vec& source, const Vec& direction) const;

  bool contain(const Vec& vec) const {
    return x.contain(vec.x) and y.contain(vec.y) and z.contain(vec.z);
  }

};

class Object;

/**
 * 解光线交点的数据包
 */
struct SolveContext {

  union Data {
    double v;
    void* p;

    Data(double v) : v(v) {}

    Data(void* p) : p(p) {}
  };

  Object* target = nullptr;

  Vec lgtSource;
  Vec lgtDirection;

  double distance = NO_INTERSECTION;
  Vec itsPosition;
  Vec itsNormal;
  Vec itsColor;
  Vec itsIllumination;

  Vec nxtDirection;
  DeflectionType type = NoDeflection;

  bool requireHD = true;  // set to false after diffuse

  double solverData[4] = {0, 0, 0, 0};  // solver-dependent data

  SolveContext() = default;

  SolveContext(Vec source, Vec direction) : lgtSource(source), lgtDirection(direction) {}

  void clearData() {
    solverData[0] = solverData[1] = solverData[2] = solverData[3] = 0;
  }

  SolveContext& operator=(bool) {
    distance = NO_INTERSECTION;
    return *this;
  }

  explicit operator bool() const {
    return distance > EMACH;
  }

  bool operator<(const SolveContext& other) const {
    return *this and (!other or distance < other.distance);
  }

  virtual double refraction_index() const {
    return 1;
  }

};

#endif //LAB2_BASICS_H
