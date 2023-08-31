//
// Created by Yixiang Tu on 2019/5/30.
//

#include "Basics.h"
#include "object/Object.h"

template struct Mat<Pix>;

double rnd() {
  static std::random_device rd;
  static std::mt19937 mt(rd());
  return mt() / (double)std::mt19937::max();
}

double pn_rnd() {
  static std::random_device rd;
  static std::mt19937 mt(rd());
  return mt() / (double)std::mt19937::max() * 2 - 1;
}

/*double rnd() {
  static std::random_device rd;
  static uint64_t s[4] = {((uint64_t) rd() << 32) | rd(), ((uint64_t) rd() << 32) | rd(),
                          ((uint64_t) rd() << 32) | rd(), ((uint64_t) rd() << 32) | rd()};

  const auto t = s[1] << 17;
  s[2] ^= s[0];
  s[3] ^= s[1];
  s[1] ^= s[2];
  s[0] ^= s[3];
  s[2] ^= t;
  s[3] = (s[3] << 45) | (s[3] >> 19);

  const auto r = (s[0] + s[3]) >> 12 | 0x3ff0000000000000ull;
  return *reinterpret_cast<const double*>(&r) - 1;
}

double pn_rnd() {
  static std::random_device rd;
  static uint64_t s[4] = {((uint64_t) rd() << 32) | rd(), ((uint64_t) rd() << 32) | rd(),
                          ((uint64_t) rd() << 32) | rd(), ((uint64_t) rd() << 32) | rd()};

  const auto t = s[1] << 17;
  s[2] ^= s[0];
  s[3] ^= s[1];
  s[1] ^= s[2];
  s[0] ^= s[3];
  s[2] ^= t;
  s[3] = (s[3] << 45) | (s[3] >> 19);

  const auto k = s[0] + s[3];
  const auto r = k >> 12 | 0x3ff0000000000000ull;
  return k & 0x10 ? (*reinterpret_cast<const double*>(&r) - 1) : (1 - *reinterpret_cast<const double*>(&r));
}*/

double AABB::intersect(const Vec& source, const Vec& direction) const {
  double t[] = {-1.0, -1.0, -1.0};
  for (auto d: {0, 1, 2}) {
    if (direction[d] > EMACH) {
      if (source[d] > (*this)[d].right) {
        return NO_INTERSECTION;
      }
      t[d] = ((*this)[d].left - source[d]) / direction[d];
    } else if (direction[d] < -EMACH) {
      if (source[d] < (*this)[d].left) {
        return NO_INTERSECTION;
      }
      t[d] = ((*this)[d].right - source[d]) / direction[d];
    }
  }
  auto dmax = t[0] > t[1] ? (t[0] > t[2] ? 0 : 2) : (t[1] > t[2] ? 1 : 2);
  if (t[dmax] < EMACH) {
    return (x.left - source.x > EMACH or source.x - x.right > EMACH or
            y.left - source.y > EMACH or source.y - y.right > EMACH or
            z.left - source.z > EMACH or source.z - z.right > EMACH) ? NO_INTERSECTION : EMACH;
  } else {
    auto possible = source + t[dmax] * direction;
    for (auto d: {0, 1, 2}) {
      if ((*this)[d].left - possible[d] > EMACH or possible[d] - (*this)[d].right > EMACH) {
        return NO_INTERSECTION;
      }
    }
    return t[dmax];
  }
}

Vec Vec::spectrum(double x) {
  x *= 6;
  //if (x < 1) {
  //  return {1, 0, 0};
  //} else if (x < 2) {
  //  return {1, 1, 0};
  //} else if (x < 3) {
  //  return {0, 1, 0};
  //} else if (x < 4) {
  //  return {0, 1, 1};
  //} else if (x < 5) {
  //  return {0, 0, 1};
  //} else {
  //  return {1, 0, 1};
  //};
  if (x < 1) {
    return {1, x, 0};
  } else if (x < 2) {
    return {2 - x, 1, 0};
  } else if (x < 3) {
    return {0, 1, x - 2};
  } else if (x < 4) {
    return {0, 4 - x, 1};
  } else if (x < 5) {
    return {x - 4, 0, 1};
  } else {
    return {1, 0, 6 - x};
  };
}
