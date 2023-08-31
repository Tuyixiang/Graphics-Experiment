//
// Created by Yixiang Tu on 2019/5/31.
//

#ifndef LAB2_SPHERE_H
#define LAB2_SPHERE_H


#include "../Object.h"
#include "../../Basics.h"

class Sphere : public Object {

protected:
  Vec color;
  Vec illumination;
  double radius, radius2;

public:
  Sphere(Vec position, double radius, Vec color, Vec illumination, Deflection deflection);

  bool solvePosition(SolveContext& ctx) override;

  void solveNormal(SolveContext& ctx) override {
    ctx.itsNormal = normalized(ctx.itsPosition - position);
  }

  void solveColor(SolveContext& ctx) override {
    auto c = Vec::spectrum(normalized(ctx.itsNormal + Vec{0, 0, .6}) *
                           Vec{.35355339059327373, -.35355339059327373, 0} * .8 + .5);
    ctx.itsColor = c * .4 + Vec{.6};
  }

  void solveIllumination(SolveContext& ctx) override {
    ctx.itsIllumination = illumination;
  }

};


#endif //LAB2_SPHERE_H
