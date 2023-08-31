//
// Created by Yixiang Tu on 2019/5/28.
//

#include "Mesh.h"

constexpr int STOP_SPLIT_THRESHOLD = 24;
constexpr int ALLOW_SPLIT_THRESHOLD = 4;

int max_depth = 0;
int large_region_count = 0;
int leaf_face_count = 0;
int leaf_count = 0;
int max_region = 0;

Mesh::Mesh(std::vector<Triangle*> _triangle)
  : Object({}, {}, {}), triangles(std::move(_triangle)) {
  construct(triangles, 0);
}

bool Mesh::solvePosition(SolveContext& ctx) {
  if (aabb.intersect(ctx.lgtSource, ctx.lgtDirection) < 0) {
    return false;
  }
  bool status = false;
  if (left) {
    auto li = left->aabb.intersect(ctx.lgtSource, ctx.lgtDirection);
    auto ri = right->aabb.intersect(ctx.lgtSource, ctx.lgtDirection);
    Mesh* best = nullptr;
    Mesh* second = nullptr;
    double bestI = INFINITY, secondI = INFINITY;
    if (li >= 0) {
      if (ri >= 0) {
        best = li < ri ? left : right;
        second = li < ri ? right : left;
        bestI = min(li, ri);
        secondI = max(li, ri);
      } else {
        best = left;
        bestI = li;
      }
    } else if (ri >= EMACH) {
      best = right;
      bestI = ri;
    }
    if (best and (!ctx or ctx.distance > bestI)) {
      status |= best->solvePosition(ctx);
    }
    if (second and (!ctx or ctx.distance > secondI)) {
      status |= second->solvePosition(ctx);
    }
  } else {
    for (auto* o : triangles) {
      status |= o->solvePosition(ctx);
    }
  }
  return status;
}

void Mesh::construct(std::vector<Triangle*>& _triangle, int depth) {
  if (depth == 0) {
    std::cout << "creating mesh" << std::endl;
    max_depth = 0;
    large_region_count = 0;
    max_region = 0;
    leaf_face_count = 0;
    leaf_count = 0;
  }
  max_depth = max_depth > depth ? max_depth : depth;

  for (auto* t: _triangle) {
    aabb += t->aabb;
  }
  if (_triangle.size() < STOP_SPLIT_THRESHOLD) {
    triangles = _triangle;
    leaf_count += 1;
    leaf_face_count += triangles.size();
    return;
  }

  // find the split that minimizes the object count in the larger side
  std::vector<Triangle*> leftTriangles, rightTriangles;

  auto bestRemainder = INT_MAX;
  for (auto d: {0, 1, 2}) {
    auto halfSize = _triangle.size() / 2;
    std::nth_element(_triangle.begin(), _triangle.begin() + halfSize + 1, _triangle.end(),
                     [d](const Triangle* a, const Triangle* b)->bool {
                       return a->aabb[d].left < b->aabb[d].left;
                     });
    auto pivot = _triangle[halfSize + 1]->aabb[d].left;
    auto remainder = 0;
    for (auto i = 0; i <= halfSize; i += 1) {
      if (_triangle[i]->aabb[d].right > pivot) {
        remainder += 1;
      }
    }
    if (remainder < bestRemainder and
        halfSize > ALLOW_SPLIT_THRESHOLD + remainder) {
      bestRemainder = remainder;
      leftTriangles = {_triangle.cbegin(), _triangle.cbegin() + halfSize + 1};
      rightTriangles = {_triangle.cbegin() + halfSize + 1, _triangle.cend()};
      for (auto i = 0; i <= halfSize; i += 1) {
        if (_triangle[i]->aabb[d].right > pivot) {
          rightTriangles.push_back(_triangle[i]);
        }
      }
    }
  }
  if (bestRemainder >= _triangle.size()) {
    // can hardly separate
    large_region_count += _triangle.size() > 100;
    max_region = max_region > _triangle.size() ? max_region : _triangle.size();
    triangles = _triangle;
    leaf_count += 1;
    leaf_face_count += triangles.size();
    return;
  }
  left = new Mesh();
  left->construct(leftTriangles, depth + 1);
  right = new Mesh();
  right->construct(rightTriangles, depth + 1);

  if (depth == 0) {
    printf("mesh created, %lu triangles in total\n", triangles.size());
    printf("tree height: %d, total leafs: %d, large regions: %d, max region size: %d average region size: %lf\n",
           max_depth, leaf_count, large_region_count, max_region, (double) leaf_face_count / leaf_count);
  }
}
