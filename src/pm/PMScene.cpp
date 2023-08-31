//
// Created by Yixiang Tu on 2019/6/25.
//

#include "PMScene.h"
#include "PMRender.h"
#include "PMRect.h"


PMRender* getPMScene() {
  const Vec wallColor = {.8};
  const Deflection wallDef = {1, 1, 1, 1, 0};
  constexpr double blockZ = .4;
  const Vec blockColor = {1, .98, 1};
  const Deflection blockDef = {0, .025, 1, 1.1, 0, Deflection::Ordinary};

  std::vector<Object*> objects = {
    /// 1. back of box
    new PMSingleBBBRect(
      // vertices
      {-3, 3, 5}, {3, 3, 5}, {-3, -3, 5},
      // texture
      wallColor,
      // deflection
      wallDef,
      // photon texture size
      2000, 2000
    ),

    /// 2. top of box
    new PMSingleBBBRect(
      // vertices
      {-3, 3, -1}, {3, 3, -1}, {-3, 3, 5},
      // color
      wallColor,
      // deflection
      wallDef,
      // photon texture size
      2000, 2000
    ),

    /// 4. left of box
    new PMSingleBBBRect(
      // vertices
      {-3, -3, -1}, {-3, 3, -1}, {-3, -3, 5},
      // color
      Vec{1, .6, .6} & wallColor,
      // deflection
      wallDef,
      // photon texture size
      2000, 2000
    ),

    //* .2. right of box
    new PMSingleBBBRect(
      // vertices
      {3, -3, -1}, {3, -3, 5}, {3, 3, -1},
      // color
      Vec{.6, .6, 1} & wallColor,
      // deflection
      wallDef,
      // photon texture size
      2000, 2000
    ),

    /// 6. bottom of box
    new PMSingleBBBRect(
      // vertices
      {-3, -3, -1}, {-3, -3, 5}, {3, -3, -1},
      // color
      wallColor,
      // deflection
      wallDef,
      // photon texture size
      2000, 2000
    ),

    //new PMPureBRect(
    //  // vertices
    //  {-3, 2.999, 4.9}, {-3, 2.999, 5}, {-2.99, 3, 4.9},
    //  // color
    //  {1},
    //  // deflection
    //  {0, 0, 1, 1.3}
    //),

    /// (0, 0)
    new PMPureBRect(
      {-2.5, 2.5, 5 - blockZ}, {-2.5, 2.5, 5}, {-1.5, 2.5, 5 - blockZ},
      blockColor, blockDef
    ),
    new PMPureBRect(
      {-2.5, 1.5, 5 - blockZ}, {-2.5, 1.5, 5}, {-2.5, 2.5, 5 - blockZ},
      blockColor, blockDef
    ),
    new PMPureBRect(
      {-1.5, 1.5, 5 - blockZ}, {-1.5, 1.5, 5}, {-2.5, 1.5, 5 - blockZ},
      blockColor, blockDef
    ),
    new PMPureBRect(
      {-1.5, 2.5, 5 - blockZ}, {-1.5, 2.5, 5}, {-1.5, 1.5, 5 - blockZ},
      blockColor, blockDef
    ),
    new PMPureBRect(
      {-2.5, 1.5, 5 - blockZ}, {-2.5, 2.5, 5 - blockZ}, {-1.5, 1.5, 5 - blockZ},
      blockColor, blockDef
    ),

    /// (1, 1)
    new PMPureBRect(
      {-1.5, 1.5, 5 - blockZ}, {-1.5, 1.5, 5}, {-0.5, 1.5, 5 - blockZ},
      blockColor, blockDef
    ),
    new PMPureBRect(
      {-1.5, 0.5, 5 - blockZ}, {-1.5, 0.5, 5}, {-1.5, 1.5, 5 - blockZ},
      blockColor, blockDef
    ),
    new PMPureBRect(
      {-0.5, 0.5, 5 - blockZ}, {-0.5, 0.5, 5}, {-1.5, 0.5, 5 - blockZ},
      blockColor, blockDef
    ),
    new PMPureBRect(
      {-0.5, 1.5, 5 - blockZ}, {-0.5, 1.5, 5}, {-0.5, 0.5, 5 - blockZ},
      blockColor, blockDef
    ),
    new PMPureBRect(
      {-1.5, 0.5, 5 - blockZ}, {-1.5, 1.5, 5 - blockZ}, {-0.5, 0.5, 5 - blockZ},
      blockColor, blockDef
    ),

    /// (4, 0)
    new PMPureBRect(
      {1.5, 2.5, 5 - blockZ}, {1.5, 2.5, 5}, {2.5, 2.5, 5 - blockZ},
      blockColor, blockDef
    ),
    new PMPureBRect(
      {1.5, 1.5, 5 - blockZ}, {1.5, 1.5, 5}, {1.5, 2.5, 5 - blockZ},
      blockColor, blockDef
    ),
    new PMPureBRect(
      {2.5, 1.5, 5 - blockZ}, {2.5, 1.5, 5}, {1.5, 1.5, 5 - blockZ},
      blockColor, blockDef
    ),
    new PMPureBRect(
      {2.5, 2.5, 5 - blockZ}, {2.5, 2.5, 5}, {2.5, 1.5, 5 - blockZ},
      blockColor, blockDef
    ),
    new PMPureBRect(
      {1.5, 1.5, 5 - blockZ}, {1.5, 2.5, 5 - blockZ}, {2.5, 1.5, 5 - blockZ},
      blockColor, blockDef
    ),

    /// (3, 1)
    new PMPureBRect(
      {0.5, 1.5, 5 - blockZ}, {0.5, 1.5, 5}, {1.5, 1.5, 5 - blockZ},
      blockColor, blockDef
    ),
    new PMPureBRect(
      {0.5, 0.5, 5 - blockZ}, {0.5, 0.5, 5}, {0.5, 1.5, 5 - blockZ},
      blockColor, blockDef
    ),
    new PMPureBRect(
      {1.5, 0.5, 5 - blockZ}, {1.5, 0.5, 5}, {0.5, 0.5, 5 - blockZ},
      blockColor, blockDef
    ),
    new PMPureBRect(
      {1.5, 1.5, 5 - blockZ}, {1.5, 1.5, 5}, {1.5, 0.5, 5 - blockZ},
      blockColor, blockDef
    ),
    new PMPureBRect(
      {0.5, 0.5, 5 - blockZ}, {0.5, 1.5, 5 - blockZ}, {1.5, 0.5, 5 - blockZ},
      blockColor, blockDef
    ),

    /// (2, 2) & (3, 2)
    new PMPureBRect(
      {-.5, .5, 5 - blockZ}, {-.5, .5, 5}, {.5, .5, 5 - blockZ},
      blockColor, blockDef
    ),
    new PMPureBRect(
      {-.5, -1.5, 5 - blockZ}, {-.5, -1.5, 5}, {-.5, .5, 5 - blockZ},
      blockColor, blockDef
    ),
    new PMPureBRect(
      {.5, -1.5, 5 - blockZ}, {.5, -1.5, 5}, {-.5, -1.5, 5 - blockZ},
      blockColor, blockDef
    ),
    new PMPureBRect(
      {.5, .5, 5 - blockZ}, {.5, .5, 5}, {.5, -1.5, 5 - blockZ},
      blockColor, blockDef
    ),
    new PMPureBRect(
      {-.5, -1.5, 5 - blockZ}, {-.5, .5, 5 - blockZ}, {.5, -1.5, 5 - blockZ},
      blockColor, blockDef
    ),

    /// bottom-left
    new PMPureBRect(
      {-2.5, -.5, 5 - blockZ}, {-2.5, -.5, 5}, {-1.5, -.5, 5 - blockZ},
      blockColor, blockDef
    ),
    new PMPureBRect(
      {-2.5, -2.5, 5 - blockZ}, {-2.5, -2.5, 5}, {-2.5, -.5, 5 - blockZ},
      blockColor, blockDef
    ),
    new PMPureBRect(
      {-.5, -2.5, 5 - blockZ}, {-.5, -2.5, 5}, {-2.5, -2.5, 5 - blockZ},
      blockColor, blockDef
    ),
    new PMPureBRect(
      {-.5, -1.5, 5 - blockZ}, {-.5, -1.5, 5}, {-.5, -2.5, 5 - blockZ},
      blockColor, blockDef
    ),
    new PMPureBRect(
      {-1.5, -1.5, 5 - blockZ}, {-1.5, -1.5, 5}, {-.5, -1.5, 5 - blockZ},
      blockColor, blockDef
    ),
    new PMPureBRect(
      {-1.5, -.5, 5 - blockZ}, {-1.5, -.5, 5}, {-1.5, -1.5, 5 - blockZ},
      blockColor, blockDef
    ),
    new PMPureBRect(
      {-2.5, -2.5, 5 - blockZ}, {-2.5, -.5, 5 - blockZ}, {-1.5, -2.5, 5 - blockZ},
      blockColor, blockDef
    ),
    new PMPureBRect(
      {-1.5, -2.5, 5 - blockZ}, {-1.5, -1.5, 5 - blockZ}, {-.5, -2.5, 5 - blockZ},
      blockColor, blockDef
    ),

    /// bottom-left
    new PMPureBRect(
      {-2.5, -.5, 5 - blockZ}, {-2.5, -.5, 5}, {-1.5, -.5, 5 - blockZ},
      blockColor, blockDef
    ),
    new PMPureBRect(
      {-2.5, -2.5, 5 - blockZ}, {-2.5, -2.5, 5}, {-2.5, -.5, 5 - blockZ},
      blockColor, blockDef
    ),
    new PMPureBRect(
      {-.5, -2.5, 5 - blockZ}, {-.5, -2.5, 5}, {-2.5, -2.5, 5 - blockZ},
      blockColor, blockDef
    ),
    new PMPureBRect(
      {-.5, -1.5, 5 - blockZ}, {-.5, -1.5, 5}, {-.5, -2.5, 5 - blockZ},
      blockColor, blockDef
    ),
    new PMPureBRect(
      {-1.5, -1.5, 5 - blockZ}, {-1.5, -1.5, 5}, {-.5, -1.5, 5 - blockZ},
      blockColor, blockDef
    ),
    new PMPureBRect(
      {-1.5, -.5, 5 - blockZ}, {-1.5, -.5, 5}, {-1.5, -1.5, 5 - blockZ},
      blockColor, blockDef
    ),
    new PMPureBRect(
      {-2.5, -2.5, 5 - blockZ}, {-2.5, -.5, 5 - blockZ}, {-1.5, -2.5, 5 - blockZ},
      blockColor, blockDef
    ),
    new PMPureBRect(
      {-1.5, -2.5, 5 - blockZ}, {-1.5, -1.5, 5 - blockZ}, {-.5, -2.5, 5 - blockZ},
      blockColor, blockDef
    ),

    /// bottom-right
    new PMPureBRect(
      {1.5, -.5, 5 - blockZ}, {1.5, -.5, 5}, {2.5, -.5, 5 - blockZ},
      blockColor, blockDef
    ),
    new PMPureBRect(
      {2.5, -.5, 5 - blockZ}, {2.5, -.5, 5}, {2.5, -2.5, 5 - blockZ},
      blockColor, blockDef
    ),
    new PMPureBRect(
      {2.5, -2.5, 5 - blockZ}, {2.5, -2.5, 5}, {.5, -2.5, 5 - blockZ},
      blockColor, blockDef
    ),
    new PMPureBRect(
      {.5, -2.5, 5 - blockZ}, {.5, -2.5, 5}, {.5, -1.5, 5 - blockZ},
      blockColor, blockDef
    ),
    new PMPureBRect(
      {.5, -1.5, 5 - blockZ}, {.5, -1.5, 5}, {1.5, -1.5, 5 - blockZ},
      blockColor, blockDef
    ),
    new PMPureBRect(
      {1.5, -1.5, 5 - blockZ}, {1.5, -1.5, 5}, {1.5, -.5, 5 - blockZ},
      blockColor, blockDef
    ),
    new PMPureBRect(
      {1.5, -2.5, 5 - blockZ}, {1.5, -.5, 5 - blockZ}, {2.5, -2.5, 5 - blockZ},
      blockColor, blockDef
    ),
    new PMPureBRect(
      {.5, -2.5, 5 - blockZ}, {.5, -1.5, 5 - blockZ}, {1.5, -2.5, 5 - blockZ},
      blockColor, blockDef
    ),
  };
  auto generator = []()->PMSolveContext {
    PMSolveContext ctx;

    //ctx.photon = {1};
    //ctx.lgtSource = {pn_rnd() * 2, 2.999, pn_rnd() * 2 + 2};
    //ctx.lgtDirection = {0, 1, 0};
    //return ctx;

    auto delta = (45 + 2.5 * pn_rnd()) * (M_PI / 180);

    switch ((int) (rnd() * 2)) {
      case 0: {
        ctx.lgtSource = {-2.999, -2.999, 5 - rnd() * .1};
        ctx.lgtDirection = Vec{cos(delta), sin(delta), .01} * .9999500037496877;
        break;
      }
      case 1: {
        ctx.lgtSource = {2.999, -2.999, 5 - rnd() * .1};
        ctx.lgtDirection = Vec{-cos(delta), sin(delta), .01} * .9999500037496877;
        break;
      }
      case 2: {
        ctx.lgtSource = {-2.999, 2.999, 5 - rnd() * .1};
        ctx.lgtDirection = normalized(Vec{cos(delta), -sin(delta), .01});
        break;
      }
      default: {
        ctx.lgtSource = {2.999, 2.999, 5 - rnd() * .1};
        ctx.lgtDirection = normalized(Vec{-cos(delta), -sin(delta), .01});
        break;
      }
    }
    if (rnd() < .1) {
      ctx.lgtDirection = -ctx.lgtDirection;
    }
    ctx.photon = {.5};
    return ctx;
  };

  return new PMRender(objects, generator);
}