//
// Created by Yixiang Tu on 2019/5/24.
//

#ifndef LAB2_TRIANGLEILLUMINATION_H
#define LAB2_TRIANGLEILLUMINATION_H


#include "TriangleBase.h"

class BasicTriangleIllumination : public virtual Triangle {
protected:
  Vec illumination;

public:
  explicit BasicTriangleIllumination(Vec illumination) : illumination(illumination) {}

  void solveIllumination(SolveContext& ctx) override {
    ctx.itsIllumination = illumination;
  }

};


#endif //LAB2_TRIANGLEILLUMINATION_H
