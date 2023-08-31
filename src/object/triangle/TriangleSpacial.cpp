//
// Created by Yixiang Tu on 2019/5/24.
//

#include "TriangleSpacial.h"
#include "../surface/Surface.h"


bool solveTriangleIntersection(SolveContext& ctx, Triangle& triangle) {
  auto P = ctx.lgtDirection % triangle.edge2;
  auto det = triangle.edge1 * P;
  auto T = ctx.lgtSource - triangle.position;
  if (det < 0) {
    det = -det;
    T = -T;
  }
  if (det < EMACH) {
    return false;
  }
  auto u = T * P;
  if (u < -EMACH or u > det + EMACH) {
    return false;
  }
  auto Q = T % triangle.edge1;
  auto v = ctx.lgtDirection * Q;
  if (v < -EMACH or u + v > det + EMACH) {
    return false;
  }

  auto invDet = 1 / det;
  auto distance = triangle.edge2 * Q * invDet;
  if (distance < 1e-4) {
    return false;
  }
  if (ctx and ctx.distance < distance) {
    return false;
  }
  if (ctx) {
    ctx.clearData();
  }
  ctx.distance = distance;
  u *= invDet;
  v *= invDet;
  ctx.itsPosition = ctx.lgtSource + ctx.lgtDirection * ctx.distance;
  ctx.target = &triangle;
  ctx.solverData[0] = u;
  ctx.solverData[1] = v;

  return true;
}


bool BasicTriangleSpacial::solvePosition(SolveContext& ctx) {
  return solveTriangleIntersection(ctx, *this);
}

void InterpolateTriangleSpacial::solveNormal(SolveContext& ctx) {
  auto v1 = ctx.solverData[0];
  auto v2 = ctx.solverData[1];
  //ctx.itsNormal = normal + v1 * nDiff2 + v2 * nDiff3 + (1 - v1 - v2) * nDiff1;
  //return;
  if (v1 > v2) {
    if (v1 + v2 * 2 < 1) {
      // [v1.e1 + v2.e2] - [1/3e1 + 1/3e2] =
      // (v1 - 1/3).e1 + (3v2 - v1 - 2/3).(1/3e1 + 1/3e2)
      ctx.itsNormal = normalized(
        //normal + nDiff1 + (v1 - v2) * (nDiff2 - nDiff1) - 3 * v2 * (nDiff1)
        normal + nDiff1 * (1 - v1 - 2 * v2) + nDiff2 * (v1 - v2)
      );
    } else {
      // [v1.e1 + v2.e2] - [1/3e1 + 1/3e2] =
      // [(2v1 + v2 - 1).(2/3e1 - 1/3e2) + (2v2 + v1 - 1).(2/3e2 - 1/3e1)]
      ctx.itsNormal = normalized(
        normal + (2 * v1 + v2 - 1) * nDiff2 + (v1 + 2 * v2 - 1) * nDiff3
      );
    }
  } else {
    if (v1 * 2 + v2 < 1) {
      ctx.itsNormal = normalized(
        normal + nDiff1 * (1 - 2 * v1 - v2) + nDiff3 * (v2 - v1)
      );
    } else {
      ctx.itsNormal = normalized(
        normal + (2 * v1 + v2 - 1) * nDiff2 + (v1 + 2 * v2 - 1) * nDiff3
      );
    }
  }
}

void BasicInterpolateTriangleSpacial::solveNormal(SolveContext& ctx) {
  auto v1 = ctx.solverData[0];
  auto v2 = ctx.solverData[1];
  ctx.itsNormal = normalized(normal + v1 * nDiff1 + v2 * nDiff2);
}

SurfaceTriangleSpacial::SurfaceTriangleSpacial(Surface* surface, Point v1, Point v2, Point v3)
  : BasicTriangleSpacial(normalized(surface->df_dv((v1.x + v2.x + v3.x) / 3, (v1.y + v2.y + v3.y) / 3) %
                                    surface->df_du((v1.x + v2.x + v3.x) / 3, (v1.y + v2.y + v3.y) / 3))),
    surface(surface),
    v1(v1), v2(v2), v3(v3),
    u_range({v1.x, v2.x, v3.x}), v_range({v1.y, v2.y, v3.y}) {
  for (auto i = 0; i <= 10; i += 1) {
    for (auto j = 0; j <= 10 - i; j += 1) {
      aabb += surface->func((v1.x * (10 - i - j) + v2.x * i + v3.x * j) * .1,
                            (v1.y * (10 - i - j) + v2.y * i + v3.y * j) * .1) + surface->position;
    }
  }
}

Point SurfaceTriangleSpacial::solveAnchor(SolveContext& ctx) {
  if (ctx.solverData[2]) {
    return {ctx.solverData[2], ctx.solverData[3]};
  } else {
    return v1 * (1 - ctx.solverData[0] - ctx.solverData[1]) + v2 * ctx.solverData[0] + v3 * ctx.solverData[1];
  }
}