//
// Created by Yixiang Tu on 2019/5/30.
//

#include "SkyBox.h"

bool SkyBox::solvePosition(SolveContext& ctx) {
  // always solve skybox at last, and skip if there's any hit
  if (ctx) {
    return false;
  }
  if (not ctx.requireHD and lowDefChild) {
    return lowDefChild->solvePosition(ctx);
  }
  ctx.distance = 1e10;
  ctx.target = this;

  // this function will combine all solveXXX

  auto nx = ctx.lgtDirection.x * rotationCos - ctx.lgtDirection.z * rotationSin;
  auto nz = ctx.lgtDirection.z * rotationCos + ctx.lgtDirection.x * rotationSin;

  auto ax = fabs(nx);
  auto ay = fabs(ctx.lgtDirection.y);
  auto az = fabs(nz);

  auto sum = ax + ay + az;
  if (std::isnan(sum) or sum < 1 or sum > 2) {
    printf("%lf %lf %lf\n", ax, ay, az);
    printf("%lf %lf %lf\n", ctx.lgtDirection.x, ctx.lgtDirection.y, ctx.lgtDirection.z);
  }

  Vec illu;

  if (ax > ay) {
    if (ax > az) {
      if (nx > 0) {
        illu = right.dat((1 - nz / ax) / 2, (1 - ctx.lgtDirection.y / ax) / 2);
      } else {
        illu = left.dat((nz / ax + 1) / 2, (1 - ctx.lgtDirection.y / ax) / 2);
      }
    } else {
      if (nz > 0) {
        illu = front.dat((nx / az + 1) / 2, (1 - ctx.lgtDirection.y / az) / 2);
      } else {
        illu = back.dat((1 - nx / az) / 2, (1 - ctx.lgtDirection.y / az) / 2);
      }
    }
  } else {
    if (ay > az) {
      if (ctx.lgtDirection.y > 0) {
        illu = up.dat((1 + nx / ay) / 2, (1 + nz / ay) / 2);
      } else {
        illu = down.dat((1 + nx / ay) / 2, (1 - nz / ay) / 2);
      }
    } else {
      if (nz > 0) {
        illu = front.dat((nx / az + 1) / 2, (1 - ctx.lgtDirection.y / az) / 2);
      } else {
        illu = back.dat((1 - nx / az) / 2, (1 - ctx.lgtDirection.y / az) / 2);
      }
    }
  }

  //ctx.itsIllumination = illu;
  //return true;

  //auto multiplier = pow(max(illu), 8) * 2 + .5;
  if (illu.y < EMACH and illu.z < EMACH) {
    ctx.itsIllumination = {30 * illu.x};
  } else {
    ctx.itsIllumination = illu;
  }

  return true;
}
