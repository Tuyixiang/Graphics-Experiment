//
// Created by Yixiang Tu on 2019/5/21.
//

#ifndef LAB2_OBJECT_H
#define LAB2_OBJECT_H

#include <cassert>
#include <iostream>
#include "../Basics.h"

struct Deflection {

  enum : unsigned {
    Ordinary = 0,
    Fresnel = 0x1,
  };

  double rugged;
  double n;
  double diffuse;
  double reflect;
  double refract;
  unsigned flag = 0;

  Deflection(double diffuse = 0, double reflect = 0, double refract = 0,
             double n = 1, double rugged = 0, unsigned flag = Ordinary);

  DeflectionType spin() const;

  void solve(SolveContext& ctx) const;

};

struct Pix {
  typedef unsigned char uchar;

  uchar v[3];

  Pix() : v{0, 0, 0} {}

  Pix(const Vec& vec) : v{(uchar) (clamp(vec.x) * 255), (uchar) (clamp(vec.y) * 255), (uchar) (clamp(vec.z) * 255)} {}

  Pix(uchar a, uchar b, uchar c) : v{a, b, c} {}

  operator Vec() const {
    return {v[0] / 255.0, v[1] / 255.0, v[2] / 255.0};
  }

  uchar& operator[](int i) {
    return v[i];
  }

  uchar operator[](int i) const {
    return v[i];
  }

  friend double sum(const Pix& p) {
    return p.v[0] + p.v[1] + p.v[2];
  }
};

struct Texture : public Mat<Pix> {

  typedef unsigned char uchar;

  Texture() = default;

  Texture(int width, int height) : Mat<Pix>(width, height) {}

  bool inRange(int x, int y) const {
    return x >= 0 and x < width() and y >= 0 and y < height();
  }

  static Texture* load(const std::string& filename);

  Texture* scaleDown(int v) const;

  void inverse();

  void mean(int radius = 2);

  // convert an image texture into a normal texture
  void toNorm(const Vec& vx, const Vec& vy);
};

struct PMSolveContext;

class Object {

public:
  Vec position = {};
  AABB aabb = {};
  Deflection deflection = {};
  int hit_count = 0;

  Object() = default;

  Object(Vec position, AABB aabb, Deflection deflection)
    : position(position), aabb(aabb), deflection(deflection) {}

  void solve(SolveContext& ctx);

  // returns whether a better hit is found
  virtual bool solvePosition(SolveContext& ctx) = 0;

  virtual void solveNormal(SolveContext& ctx) = 0;

  virtual void solveColor(SolveContext& ctx) = 0;

  virtual void solveIllumination(SolveContext& ctx) = 0;

  virtual Point solveAnchor(SolveContext& ctx) { return {}; }

  // pruning: true means this object is useless and may be removed
  virtual bool prune(int count) {
    return false;
    //return hit_count * 40 < count;
  }

  virtual void solveDirection(SolveContext& ctx);


  /// for Photon Mapping
protected:

  // 把打上的光子保存在光子贴图
  virtual void PMReceivePhoton(PMSolveContext&) {};

  // 根据光子贴图计算 illumination
  virtual void PMGetIllumination(SolveContext&) {};

public:

  // 读取贴图文件
  virtual void PMLoadTexture(const std::string&) {};

  // 保存贴图文件
  virtual void PMSaveTexture(const std::string&) {};

  // 发出光子
  void PMCast(PMSolveContext& ctx);

  // 扫描结果
  void PMView(SolveContext& ctx);

  // 告知光子贴图样本数
  virtual void PMSamples(int samples) {}

};


#endif //LAB2_OBJECT_H
