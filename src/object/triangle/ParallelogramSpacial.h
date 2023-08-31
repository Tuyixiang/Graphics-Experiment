//
// Created by Yixiang Tu on 2019/5/25.
//

#ifndef LAB2_PARALLELOGRAMSPACIAL_H
#define LAB2_PARALLELOGRAMSPACIAL_H

#include "TriangleBase.h"

class BasicParallelogramSpacial : public virtual Triangle {
protected:
  Vec normal;

public:
  BasicParallelogramSpacial() : normal(normalized(edge1 % edge2)) {}

  explicit BasicParallelogramSpacial(Vec normal) : normal(normal) {}

  bool solvePosition(SolveContext& ctx) override;

  void solveNormal(SolveContext& ctx) override {
    ctx.itsNormal = normal;
  }
};

class TextureParallelogramSpacial : public BasicParallelogramSpacial {
protected:
  Texture& texture;

public:
  explicit TextureParallelogramSpacial(const std::string& filename)
    : BasicParallelogramSpacial(),
      texture(*Texture::load(filename)) {
    auto vx = normalized(edge1);
    auto vy = normalized(edge2 - (edge1 * edge2) * edge1);
    texture.toNorm(vx, vy);
  }

  void solveNormal(SolveContext& ctx) override {
    ctx.itsNormal = texture.dat(ctx.solverData[0], ctx.solverData[1]);
  }
};


#endif //LAB2_PARALLELOGRAMSPACIAL_H
