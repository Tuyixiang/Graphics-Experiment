//
// Created by Yixiang Tu on 2019/5/24.
//

#ifndef LAB2_TRIANGLECOLOR_H
#define LAB2_TRIANGLECOLOR_H

#include "TriangleBase.h"
#include "../../Basics.h"

class BasicTriangleColor : public virtual Triangle {
protected:
  Vec color;

public:
  explicit BasicTriangleColor(Vec color) : color(color) {}

  void solveColor(SolveContext& ctx) override {
    ctx.itsColor = color;
  }
};

class TextureTriangleColor : public virtual Triangle {
protected:
  Texture& texture;

public:
  explicit TextureTriangleColor(const std::string& filename) : texture(*Texture::load(filename)) {}

  void solveColor(SolveContext& ctx) override {
    assert(ctx.solverData);
    ctx.itsColor = texture.dat(ctx.solverData[0], ctx.solverData[1]);
  }
};

class AnchorTriangleColor : public virtual Triangle {
protected:
  const Texture& texture;
  const Point anchor;
  const Point edge[2];

public:
  AnchorTriangleColor(const Texture& texture, Point p1, Point p2, Point p3)
    : texture(texture), anchor(p1), edge{p2 - p1, p3 - p1} {
    //assert(anchor.x >= 0 and anchor.y >= 0 and edge[0].x >= 0 and edge[0].y >= 0 and edge[1].x >= 0 and edge[2].x >= 0);
  }

  void solveColor(SolveContext& ctx) override {
    ctx.itsColor = texture.dat(solveAnchor(ctx));
  }

  Point solveAnchor(SolveContext& ctx) override {
    return anchor + edge[0] * ctx.solverData[0] + edge[1] * ctx.solverData[1];
  }


};


#endif //LAB2_TRIANGLECOLOR_H
