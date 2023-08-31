//
// Created by Yixiang Tu on 2019/6/24.
//

#ifndef LAB2_PHOTON_H
#define LAB2_PHOTON_H

#include "../Basics.h"

#define DISPERSION
#ifdef DISPERSION

/// 带色散的版本
class Photon {

public:
  double wavelength = 0; // 0 ~ 1: red ~ purple
  Vec color;

  /**
   * 计算两个颜色的吸收量的时候，首先将颜色映射到与 (1, 1, 1) 正交的单位圆上，做点积，然后加一个函数作为反射比
   */
  Vec mapped_color; // color used in absorption calculation

  double _luminosity;

  static double f(double v) {
    auto v2 = pow<2>(v);
    return .011393229166666666 * (87.42857142857143 - v2 * (17.857142857142858 + v2));
  }

  Photon() = default;

  Photon(double luminosity)
    : wavelength(rnd()), _luminosity(luminosity),
      color(computeColor()), mapped_color(normalized(color ^ Vec{0.5773502691896258})) {}

  Photon(double wavelength, double luminosity)
    : wavelength(wavelength), _luminosity(luminosity),
      color(computeColor()), mapped_color(normalized(color ^ Vec{0.5773502691896258})) {}

  operator Vec() const {
    return color * _luminosity * 2;
  }

  explicit operator bool() const {
    return _luminosity > EMACH;
  }

  Vec computeColor() const {
    auto w = wavelength * 7;
    auto v = w - floor(w);
    if (w < 1) {
      return {f(1 - v) - f(1 + v), 0, 0};
    } else if (w < 2) {
      return {f(v), f(2 - v), 0};
    } else if (w < 3) {
      return {f(1 + v), f(1 - v), 0};
    } else if (w < 4) {
      return {0, f(v), f(2 - v)};
    } else if (w < 5) {
      return {0, f(1 + v), f(1 - v)};
    } else if (w < 6) {
      return {f(2 - v), 0, f(v)};
    } else {
      return {f(1 + v), 0, f(1 + v)};
    }
  }

  /**
   * 在某一个颜色上被反射，吸收一定比例
   */
  Photon& operator&=(const Vec& vec) {
    // 灰色，直接乘以明度
    if (is0(max(vec) - min(vec))) {
      _luminosity *= vec.x;
      return *this;
    }

    /*
     * 把颜色分解成两部分，一部分和 (1, 1, 1) 平行，另一部分垂直
     * luminosity *= 平行部分 + func(垂直部分 * 光子颜色mapped)
     */
    auto light = min(vec);
    auto mapped = (vec - light) ^ Vec{0.5773502691896258};
    auto prod = normalized(mapped) * mapped_color;
    if (prod < 0) {
      _luminosity *= light;
      return *this;
    }

    _luminosity *= light + (1 - light) * prod;
    return *this;
  }

  double luminosity() const {
    return _luminosity;
  }

  bool roulette(double possibility) {
    if (rnd() < possibility) {
      _luminosity /= possibility;
      return true;
    } else {
      return false;
    }
  }

};

#else

/// 不带色散
class Photon {

public:
  constexpr static double wavelength = 0;
  Vec color;

  Photon() = default;

  Photon(double luminosity)
    : color(luminosity) {}

  operator Vec() const {
    return color;
  }

  explicit operator bool() const {
    return max(color) > EMACH;
  }

  /**
   * 在某一个颜色上被反射，吸收一定比例
   */
  Photon& operator&=(const Vec& vec) {
    color &= vec;
    return *this;
  }

  double luminosity() {
    return max(color);
  }

  bool roulette(double possibility) {
    if (rnd() < possibility) {
      color /= possibility;
      return true;
    } else {
      return false;
    }
  }

};
#endif //DISPERSION


#endif //LAB2_PHOTON_H
