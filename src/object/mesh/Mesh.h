//
// Created by Yixiang Tu on 2019/5/28.
//

#ifndef LAB2_MESHBASE_H
#define LAB2_MESHBASE_H


#include <algorithm>
#include <climits>
#include <iostream>
#include "../Object.h"
#include "../Triangle.h"
#include "../../Basics.h"

class Mesh : public Object {
public:
  std::vector<Triangle*> triangles;

  Mesh* left = nullptr;
  Mesh* right = nullptr;

  Mesh() = default;

  Mesh(std::vector<Triangle*> _triangle);

  void construct(std::vector<Triangle*>& _triangle, int depth);

  bool solvePosition(SolveContext& ctx) override;

  void solveNormal(SolveContext& ctx) override {
    ctx.target->solveNormal(ctx);
  }

  void solveColor(SolveContext& ctx) override {
    ctx.target->solveColor(ctx);
  }

  void solveIllumination(SolveContext& ctx) override {
    ctx.target->solveIllumination(ctx);
  }

  void solveDirection(SolveContext& ctx) override {
    ctx.type = ctx.target->deflection.spin();
    ctx.target->solveDirection(ctx);
  }

  bool prune(int) override {
    std::vector<Triangle*> useful_triangles;
    for (auto* p: triangles) {
      if (p->hit_count > 0) {
        useful_triangles.push_back(p);
      } else {
        // delete p;
      }
    }
    delete left;
    delete right;
    left = right = nullptr;
    triangles.swap(useful_triangles);
    construct(triangles, 0);
    return false;
  }

  ~Mesh() {
    delete left;
    delete right;
  }

};


#endif //LAB2_MESHBASE_H
