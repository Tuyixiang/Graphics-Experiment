//
// Created by Yixiang Tu on 2019/6/24.
//

#include "PMObject.h"

void Object::PMCast(PMSolveContext& ctx) {
  //hit_count += 1;
  if (!ctx.itsColor) { solveColor(ctx); }

  ctx.photon &= ctx.itsColor;
  if (not ctx.photon) { // 光子完全被吸收，无事发生
    ctx.nxtDirection = false;
    return;
  }

  solveNormal(ctx);
  ctx.type = deflection.spin();
  solveDirection(ctx);

  // 如果触发漫反射，则将光子烧入贴图
  if (ctx.type == Diffuse) {
    PMReceivePhoton(ctx);
  }
}

/**
 * 由 PMRender 指定 type
 */
void Object::PMView(SolveContext& ctx) {
  //ctx.type = deflection.spin();
  if (ctx.type == Diffuse) {
    PMGetIllumination(ctx);
    return;
  }
  if (not ctx.itsNormal) { solveNormal(ctx); }
  if (not ctx.itsColor) { solveColor(ctx); }
  solveDirection(ctx);
}