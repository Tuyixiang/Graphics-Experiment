//
// Created by Yixiang Tu on 2019/5/24.
//

#ifndef LAB2_TRIANGLESPACIALI_H
#define LAB2_TRIANGLESPACIALI_H

#include "TriangleBase.h"

class Surface;

class BasicTriangleSpacial : public virtual Triangle {
public:
  Vec normal;

public:
  BasicTriangleSpacial() : normal(normalized(edge1 % edge2)) {}

  explicit BasicTriangleSpacial(Vec normal) : normal(normal) {}

  bool solvePosition(SolveContext& ctx) override;

  void solveNormal(SolveContext& ctx) override {
    ctx.itsNormal = normal;
  }
};

class TextureTriangleSpacial : public BasicTriangleSpacial {
protected:
  Texture& texture;

public:
  explicit TextureTriangleSpacial(const std::string& filename)
    : BasicTriangleSpacial(),
      texture(*Texture::load(filename)) {
    auto vx = normalized(edge1);
    auto vy = normalized(edge2 - (edge1 * edge2) * edge1);
    texture.toNorm(vx, vy);
  }

  void solveNormal(SolveContext& ctx) override {
    ctx.itsNormal = texture.dat(ctx.solverData[0], ctx.solverData[1]);
  }
};

class InterpolateTriangleSpacial : public BasicTriangleSpacial {
protected:
  Vec nDiff1, nDiff2, nDiff3;

public:
  explicit InterpolateTriangleSpacial(Vec n1, Vec n2, Vec n3)
    : BasicTriangleSpacial(),
      nDiff1(n1 - normal), nDiff2(n2 - normal), nDiff3(n3 - normal) {}

  void solveNormal(SolveContext& ctx) override;

};

class BasicInterpolateTriangleSpacial : public BasicTriangleSpacial {
protected:
  Vec nDiff1, nDiff2;

public:
  explicit BasicInterpolateTriangleSpacial(Vec n1, Vec n2, Vec n3)
    : BasicTriangleSpacial(n1),
      nDiff1(n2 - n1), nDiff2(n3 - n1) {}

  void solveNormal(SolveContext& ctx) override;

};

class SurfaceTriangleSpacial : public BasicTriangleSpacial {
protected:
  Surface* const surface;
  const Point v1, v2, v3;
  const Range u_range, v_range;

public:
  explicit SurfaceTriangleSpacial(Surface* surface, Point v1, Point v2, Point v3);

  Point solveAnchor(SolveContext& ctx) override;

  void solveNormal(SolveContext& ctx) override {}

};


#endif //LAB2_TRIANGLESPACIALI_H
