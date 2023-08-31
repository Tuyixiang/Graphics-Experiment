//
// Created by Yixiang Tu on 2019/5/25.
//

#include "ParallelogramSpacial.h"

bool BasicParallelogramSpacial::solvePosition(SolveContext& ctx) {

  auto P = ctx.lgtDirection % edge2;
  auto det = edge1 * P;
  auto T = ctx.lgtSource - position;
  if (det < 0) {
    det = -det;
    T = -T;
  }
  if (det < EMACH) {
    return false;
  }
  auto u = T * P;
  if (u < 0 or u > det) {
    return false;
  }
  auto Q = T % edge1;
  auto v = ctx.lgtDirection * Q;
  if (v < 0 or v > det) {
    return false;
  }

  auto invDet = 1 / det;
  auto distance = edge2 * Q * invDet;
  if (distance < EMACH) {
    return false;
  }
  if (ctx and ctx.distance < distance) {
    return false;
  }
  ctx.distance = distance;
  u *= invDet;
  v *= invDet;
  ctx.itsPosition = ctx.lgtSource + ctx.lgtDirection * ctx.distance;
  ctx.target = this;
  ctx.solverData[0] = u;
  ctx.solverData[1] = v;

  return true;
}
