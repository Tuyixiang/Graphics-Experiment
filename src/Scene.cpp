//
// Created by Yixiang Tu on 2019/5/25.
//

#include "Scene.h"
#include "Render.h"
#include "object/Parallelogram.h"
#include "object/mesh/ObjLoader.h"
#include "object/other/SkyBox.h"
#include "object/mesh/Mesh.h"
#include "object/surface/Surface.h"
#include "object/other/Sphere.h"

std::vector<Object*> myBox(bool dark = true, bool aisle = true) {

  Vec whiteWall = {.8};
  Vec light = {3};
  Deflection wallDef = {1, 1, 1, 1, 0};

  std::vector<Object*> objects = {

    /// 0. front of box (back of camera)
    //new BasicParallelogram(
    //  // vertices
    //  {-3, -3, -1}, {-3, 3, -1}, {3, -3, -1},
    //  // color
    //  whiteWall,
    //  // illumination
    //  {0, 0, 0},
    //  // deflection
    //  wallDef
    //),

    /// 1. back of box
    new BTBParallelogram(
      // vertices
      {-3, 3, 5}, {3, 3, 5}, {-3, -3, 5},
      // normal
      dark ? "back.pbm" : "back2.pbm",
      // illumination
      {0, 0, 0},
      // deflection
      wallDef
    ),

    /// back picture
    //new TextureParallelogram(
    //  // vertices
    //  {-1.4, 2, 4.9999}, {1.4, 2, 4.9999}, {-1.4, -2, 4.9999},
    //  // color
    //  "t.pbm",
    //  // illumination
    //  {0, 0, 0},
    //  // deflection
    //  {1, 1, 1}
    //),

    /// 2. top of box
    new BBBParallelogram(
      // vertices
      {-3, 3, -1}, {3, 3, -1}, {-3, 3, 5},
      // color
      whiteWall,
      // illumination
      {0, 0, 0},
      // deflection
      wallDef
    ),

    /// 3. light on top
    new BBBParallelogram(
      // vertices
      {-2, 2.9, 0}, {2, 2.9, 0}, {-2, 2.9, 4},
      // color
      {0, 0, 0},
      // illumination
      light,
      // deflection
      {0, 0, 0}
    ),
    new BBBParallelogram(
      // vertices
      {-2, 2.9, 4}, {2, 2.9, 4}, {-2, 3, 4},
      // color
      {0, 0, 0},
      // illumination
      light,
      // deflection
      {0, 0, 0}
    ),
    new BBBParallelogram(
      // vertices
      {-2, 2.9, 0}, {-2, 3, 0}, {-2, 2.9, 4},
      // color
      {0, 0, 0},
      // illumination
      light,
      // deflection
      {0, 0, 0}
    ),
    new BBBParallelogram(
      // vertices
      {2, 2.9, 0}, {2, 3, 0}, {2, 2.9, 4},
      // color
      {0, 0, 0},
      // illumination
      light,
      // deflection
      {0, 0, 0}
    ),

    /// 4. left of box
    new BBBParallelogram(
      // vertices
      {-3, -3, -1}, {-3, 3, -1}, {-3, -3, 5},
      // color
      Vec{1, .6, .6} & whiteWall,
      // illumination
      {0, 0, 0},
      // deflection
      wallDef
    ),

    //* .2. right of box
    new BBBParallelogram(
      // vertices
      {3, -3, -1}, {3, -3, 5}, {3, 3, -1},
      // color
      Vec{.6, .6, 1} & whiteWall,
      // illumination
      {0, 0, 0},
      // deflection
      wallDef
    ),

    /// 6. bottom of box
    new BBBParallelogram(
      // vertices
      {-3, -3, -1}, {-3, -3, 5}, {3, -3, -1},
      // color
      whiteWall,
      // illumination
      {0},
      // deflection
      wallDef
    ),

  };

  /// 7. aisle light
  if (aisle) {
    objects.push_back(new BBBParallelogram(
      // vertices
      {-2.8, -3, -1}, {-3, -2.8, -1}, {-2.8, -3, 5},
      // color
      {0},
      // illumination
      light,
      // deflection
      {0, 0, 0}
    ));
    objects.push_back(new BBBParallelogram(
      // vertices
      {2.8, -3, -1}, {3, -2.8, -1}, {2.8, -3, 5},
      // color
      {0},
      // illumination
      light,
      // deflection
      {0, 0, 0}
    ));
  }

  return objects;
}

std::vector<Object*> myBox2(bool dark = true) {

  Vec whiteWall = {.8};
  Vec light = {1.8};
  Deflection wallDef = {1, 1, 1, 1, 0};

  std::vector<Object*> objects = {

    new BBBParallelogram(
      // vertices
      {-3, -3, -1}, {3, -3, -1}, {-3, 3, -1},
      // color
      .2 * whiteWall,
      // illumination
      {0, 0, 0},
      // deflection
      wallDef
    ),

    /// 1. back of box
    new BTBParallelogram(
      // vertices
      {-3, 3, 5}, {3, 3, 5}, {-3, -3, 5},
      // normal
      dark ? "back.pbm" : "back2.pbm",
      // illumination
      {0, 0, 0},
      // deflection
      wallDef
    ),

    /// 2. top of box
    new BBBParallelogram(
      // vertices
      {-3, 3, -1}, {3, 3, -1}, {-3, 3, 5},
      // color
      whiteWall,
      // illumination
      {0, 0, 0},
      // deflection
      wallDef
    ),

    /// 3. light on top
    new BBBParallelogram(
      // vertices
      //{-1.5, 2.9999, .5}, {1.5, 2.9999, .5}, {-1.5, 2.9999, 3.5},
      {-3, 2.9999, -1}, {3, 2.9999, -1}, {-3, 2.9999, 5},
      // color
      {0, 0, 0},
      // illumination
      light,
      // deflection
      {0, 0, 0}
    ),

    /// 4. left of box
    new BBBParallelogram(
      // vertices
      {-3, -3, -1}, {-3, 3, -1}, {-3, -3, 5},
      // color
      Vec{1, .6, .6} & whiteWall,
      // illumination
      {0, 0, 0},
      // deflection
      wallDef
    ),

    /// 4.2. right of box
    new BBBParallelogram(
      // vertices
      {3, -3, -1}, {3, -3, 5}, {3, 3, -1},
      // color
      Vec{.6, .6, 1} & whiteWall,
      // illumination
      {0, 0, 0},
      // deflection
      wallDef
    ),

    /// 6. bottom of box
    new BBBParallelogram(
      // vertices
      {-3, -3, -1}, {-3, -3, 5}, {3, -3, -1},
      // color
      whiteWall,
      // illumination
      {0},
      // deflection
      wallDef
    ),

  };

  return objects;
}

Render* apollo() {
  auto render = new Render(myBox(false));
  ObjLoader loader("apollo.obj");
  loader.rotateY(M_PI);
  loader.normalize(.5, 0, 0);
  loader.scale(5);
  loader.translate({0, -3, 2});
  render->objects.push_back(new Mesh(loader.constructI(Vec{.9}, Vec{0}, Deflection{.9, 1, 1, 1, .5})));
  render->objects.push_back(new BBBParallelogram({-3, -.2, 1.5}, {3, -.2, 1.5}, {-3, -.4, 1.5}, {1}, {0}, {0, 0, 1, 1, .5}));
  return render;
}

Render* louis() {
  auto render = new Render(myBox(false));
  ObjLoader loader("model.obj", {"louis.pbm"});
  loader.rotateX(M_PI / 2);
  loader.rotateY(M_PI * 1.02);
  loader.normalize(.5, 0, 0);
  loader.scale(6);
  loader.translate({.15, -4.5, 2});
  render->objects.push_back(new Mesh(loader.constructI(Vec{.9}, Vec{0}, Deflection{.9, 1, 1, 1, .5})));
  return render;
}

Render* man() {
  auto render = new Render();
  ObjLoader loader("man.obj", std::initializer_list<std::string>());
  loader.rotateY(M_PI * 1.15);
  loader.normalize(.5, 0, 0);
  loader.scale(6);
  loader.translate({-.5, -3, 1});
  render->objects.push_back(new Mesh(loader.constructI(Vec{.9}, Vec{0}, Deflection{.9, 1, 1, 1, .5})));
  render->objects.push_back(new SkyBox("storm", M_PI * .8));
  return render;
}

Render* grass() {
  auto render = new Render();
  render->setElevation(-asin(.15));
  render->setPan(M_PI / 9);
  render->source = {.5, 0, 1};

  render->blur_radius = 0.03;
  render->focus = 1.8;

  ObjLoader loader("grass.obj", {"grass.pbm"});
  loader.rotateZ(M_PI * 0.03);
  loader.normalize(.5, 0, 0);
  loader.scale(10);
  loader.translate({0, -1.5, -3});

  constexpr auto x = 1.8 * sin(M_PI / 9);
  constexpr auto z = 1.8 * cos(M_PI / 9);
  //render->objects.push_back(new Sphere({.5 + x, 0, 1 + z}, .3, {1}, {0}, {0, .1, 1, 1.25}));
  auto f = [](double x)->double { return sqrt(.09 - x * x) * (x + .6) * 1.513888974740474; };
  auto d = [](double x)->double { return (.0455 - (x + .3) * x) * 3.027777949480948 / sqrt(.09 - x * x); };
  render->objects.push_back(new Surface(
    // f(u, v)
    [f](double u, double v)->Vec {
      auto fu = f(u);
      return {fu * sin(v) + x + .5, u - .15, fu * cos(v) + z + 1};
    },
    // df(u, v) / du
    [d](double u, double v)->Vec {
      auto du = d(u);
      return {du * sin(v), 1, du * cos(v)};
    },
    // df(u, v) / dv
    [f](double u, double v)->Vec {
      auto fu = f(u);
      return {fu * cos(v), 0, -fu * sin(v)};
    },
    // u, v domain
    {-.3, .3}, {0, 2 * M_PI},
    // color
    {1},
    // illumination
    {0},
    // position
    {0},
    // deflection
    {0, 0, 1, 1.3, 0, Deflection::Fresnel},
    // u, v steps
    100, 100,
    true
  ));

  render->objects.push_back(
    new BBBParallelogram({-5, -1.3, -2}, {5, -1.3, -2}, {-5, -1.3, 5}, {.1, .05, .05}, {}, {1, 1, 1}));
  render->objects.push_back(new Mesh(loader.constructI(Vec{1}, Vec{0}, Deflection{.9, 1, 1, 1, .5})));
  render->objects.push_back(SkyBox::loadImage("prairie.pbm", M_PI / 9));
  return render;
}

std::vector<Object*> gideon() {
  ObjLoader loader("gideon.obj", {});
  loader.normalize(.5, 0, .5);
  loader.rotateY(M_PI * 0.83);
  loader.scale(5);
  loader.translate({0, -3, 2});
  auto objects = myBox(false);
  //auto objects = std::vector<Object*>();
  objects.push_back(new Mesh(loader.constructI(Vec{.9}, Vec{0}, Deflection{.93, 1, 1, 1, .5})));
  //objects.push_back(new SkyBox("storm", M_PI * .4));
  return objects;
}

Render* surface() {
  auto render = new Render();
  //auto render = new Render(myBox(true));
  auto f = [](double x)->double { return sqrt(1 - (x - 1) * (x - 1)) * (x + 1) / 2.201834737520806; };
  auto d = [](double x)->double { return ((1 - x) * x + .5) * .9083333848442843 / sqrt(x * (2 - x)); };
  render->objects.push_back(new Surface(
    // f(u, v)
    [f](double u, double v)->Vec { return {f(u) * sin(v), u, f(u) * cos(v)}; },
    // df(u, v) / du
    [d](double u, double v)->Vec { return {d(u) * sin(v), 1, d(u) * cos(v)}; },
    // df(u, v) / dv
    [f](double u, double v)->Vec { return {f(u) * cos(v), 0, -f(u) * sin(v)}; },
    // u, v domain
    {0, 2}, {0, 2 * M_PI},
    // color
    {1},
    // illumination
    {0},
    // position
    {0, -1, 3},
    // deflection
    {0, 0, 1, 1.2},
    // u, v steps
    100, 100
  ));
  render->objects.push_back(new SkyBox("storm", M_PI / 2));
  return render;
}

Render* ring() {
  auto render = new Render(myBox2(false));
  render->objects.push_back(new Surface(
    // f(u, v)
    /*
     * cos(v) * (6 + (1.25 + sin(3 * u)) * sin(u + 3 * v)) * .2,
     * sin(v) * (6 + (1.25 + sin(3 * u)) * sin(u + 3 * v)) * .2,
     * cos(u + 3 * v) * (1.25 + sin(3 * u)) * .2,
     */
    [](double v, double u)->Vec {
      auto a = 1.25 + fast_sin(3 * u);
      double b, c, sv, cv;
      fast_sin_cos(u + 3 * v, b, c);
      fast_sin_cos(v, sv, cv);
      b = (6 + a * b) * .3;
      Vec vec = {
        cv * b,
        sv * b,
        c * a * .3,
      };
      return vec;
      //return {vec.x, vec.y * .8 + vec.z * .6, vec.z * .8 - vec.y * .6};
    },
    // df(u, v) / dv
    /*
     * (3 * cos(v) * cos(u + 3 * v) * (1.25 + sin(3 * u)) - sin(v) * (6 + (1.25 + sin(3 * u)) * sin(u + 3 * v))) * .2,
     *  (3 * cos(u + 3 * v) * (1.25 + sin(3 * u)) * sin(v) + cos(v) * (6 + (1.25 + sin(3 * u)) * sin(u + 3 * v))) * .2,
     *  -3 * (1.25 + sin(3 * u)) * sin(u + 3 * v) * .2,
     */
    [](double v, double u)->Vec {
      double cv, sv;
      fast_sin_cos(v, sv, cv);
      auto a = 1.25 + fast_sin(3 * u);
      double b, c;
      fast_sin_cos(u + 3 * v, b, c);
      b *= a;
      c *= 3 * a;
      Vec vec = {
        (cv * c - sv * (6 + b)) * .3,
        (sv * c + cv * (6 + b)) * .3,
        -.9 * b,
      };
      return vec;
    },
    // df(u, v) / du
    /*
     * cos(v) * (cos(u + 3 * v) * (1.25 + sin(3 * u)) + 3 * cos(3 * u) * sin(u + 3 * v)) * .2,
     * sin(v) * (cos(u + 3 * v) * (1.25 + sin(3 * u)) + 3 * cos(3 * u) * sin(u + 3 * v)) * .2,
     * (3 * cos(3 * u) * cos(u + 3 * v) - (1.25 + sin(3 * u)) * sin(u + 3 * v)) * .2,
     */
    [](double v, double u)->Vec {
      double a, b, c, d, sv, cv;
      fast_sin_cos(v, sv, cv);
      fast_sin_cos(3 * u, a, b);
      fast_sin_cos(u + 3 * v, c, d);
      a += 1.25;
      b *= 3;
      c *= .3;
      d *= .3;
      auto e = d * a + b * c;
      Vec vec = {
        cv * e,
        sv * e,
        (b * d - a * c),
      };
      return vec;
    },
    // u, v domain
    {0, 2 * M_PI}, {0, 2 * M_PI},
    // texture
    //"spectrum.pbm",
    //[](double u, double v)->Point {
    //  return {clamp(v * M_1_PI * .5), .5};
    //  //auto x = (u + v) * .5 * M_1_PI;
    //  //return {clamp(x > 1 ? x - 1 : x), .5};
    //},
    // color
    {1},
    // illumination
    {0},
    // position
    {0, 0, 4},
    // deflection
    {0, 1, 1, 1.25, 0, Deflection::Fresnel},
    // u, v steps
    1000, 1000,
    // use mesh only
    false
  ));
  //render->objects.pop_back();
  render->objects.push_back(new Sphere({0, 0, 4}, .8, {1}, {0}, {0, 1, 1, 1.25, 0, Deflection::Fresnel}));
  //render->objects.push_back(new SkyBox("storm", M_PI));
  return render;
}

Render* ball() {
  auto render = new Render(myBox2(false));
  render->objects.push_back(new Surface(
    // f(u, v)
    /*
     * Cos[b] (Cos[a + 2 b] (5/4 + Sin[3 a]) + 3 Cos[3 a] Sin[a + 2 b]),
     * Sin[b] (Cos[a + 2 b] (5/4 + Sin[3 a]) + 3 Cos[3 a] Sin[a + 2 b]),
     * 3 Cos[3 a] Cos[a + 2 b] - (5/4 + Sin[3 a]) Sin[a + 2 b]
     */
    [](double u, double v)->Vec {
      Vec vec = {
        cos(v) * (cos(u + 2 * v) * (1.25 + sin(3 * u)) + 3 * cos(3 * u) * sin(u + 2 * v)),
        3 * cos(3 * u) * cos(u + 2 * v) - (1.25 + sin(3 * u)) * sin(u + 2 * v),
        sin(v) * (cos(u + 2 * v) * (1.25 + sin(3 * u)) + 3 * cos(3 * u) * sin(u + 2 * v)),
      };
      return vec / 3;
    },
    // df(u, v) / dv
    /*
     * 1/4 Cos[b] (-8 Cos[2 (a - b)] + 32 Cos[2 (2 a + b)] - 5 Sin[a + 2 b]),
     * 1/4 Sin[b] (-8 Cos[2 (a - b)] + 32 Cos[2 (2 a + b)] - 5 Sin[a + 2 b]),
     * -(5/4) Cos[a + 2 b] - 2 (Sin[2 (a - b)] + 4 Sin[2 (2 a + b)])
     */
    [](double u, double v)->Vec {
      Vec vec = {
        .25 * cos(v) * (-8 * cos(2 * (u - v)) + 32 * cos(4 * u + 2 * v) - 5 * sin(u + 2 * v)),
        -1.25 * cos(u + 2 * v) - 2 * (sin(2 * (u - v)) + 4 * sin(4 * u + 2 * v)),
        .25 * sin(v) * (-8 * cos(2 * (u - v)) + 32 * cos(4 * u + 2 * v) - 5 * sin(u + 2 * v)),
      };
      return v > M_PI ? vec / 3 : vec / -3;
    },
    // df(u, v) / du
    /*
     * 1/8 (12 Cos[2 a - 3 b] + 4 Cos[2 a - b] + 8 Cos[4 a + b] + 24 Cos[4 a + 3 b] - 5 Sin[a + b] - 15 Sin[a + 3 b]),
     * 1/8 (-5 Cos[a + b] + 15 Cos[a + 3 b] + 4 (-3 Sin[2 a - 3 b] + Sin[2 a - b] - 2 Sin[4 a + b] + 6 Sin[4 a + 3 b])),
     * -2 Cos[a + 2 b] (5/4 + Sin[3 a]) - 6 Cos[3 a] Sin[a + 2 b]
     */
    [](double u, double v)->Vec {
      Vec vec = {
        .125 *
        (12 * cos(2 * u - 3 * v) + 4 * cos(2 * u - v) + 8 * cos(4 * u + v) + 24 * cos(4 * u + 3 * v) - 5 * sin(u + v) -
         15 * sin(u + 3 * v)),
        -2 * cos(u + 2 * v) * (1.25 + sin(3 * u)) - 6 * cos(3 * u) * sin(u + 2 * v),
        .125 *
        (-5 * cos(u + v) + 15 * cos(u + 3 * v) +
         4 * (-3 * sin(2 * u - 3 * v) + sin(2 * u - v) - 2 * sin(4 * u + v) + 6 * sin(4 * u + 3 * v))),
      };
      return vec / 3;
    },
    // u, v domain
    {0, 2 * M_PI}, {0, 2 * M_PI},
    // texture
    //"spectrum.pbm",
    //[](double u, double v)->Point {
    //  return {clamp(v * M_1_PI * .5), .5};
    //  //auto x = (u + v) * .5 * M_1_PI;
    //  //return {clamp(x > 1 ? x - 1 : x), .5};
    //},
    // color
    {1},
    // illumination
    {0},
    // position
    {0, 0, 3},
    // deflection
    {1, 1, 1, 1.25, 0, Deflection::Fresnel},
    // u, v steps
    1000, 1000,
    // use mesh only
    true
  ));
  //render->objects.pop_back();
  //render->objects.push_back(new Sphere({0, 0, 4}, .8, {1}, {0}, {0, 1, 1, 1.25, 0, Deflection::Fresnel}));
  //render->objects.push_back(new SkyBox("storm", M_PI));
  return render;
}

Render* klein() {
  auto render = new Render(myBox(false));
  Vec color = {1};
  Deflection deflection = {.2, 0, 1, 1.2, 0, Deflection::Fresnel};
  render->objects.push_back(new Surface(
    // f(u, v)
    [](double u, double v)->Vec {
      return {
        0.25667830632459043 - 0.295 * (-2. + cos(u)) * cos(u) * cos(v) + 0.5 * cos(u) * (1 + sin(u)),
        2.1697863390130157 + (-1.56 + (-0.59 + 0.295 * cos(u)) * cos(v)) * sin(u),
        -0.295 * (-2 + cos(u)) * sin(v),
      };
    },
    // df(u, v) / du
    [](double u, double v)->Vec {
      return {
        0.5 * pow<2>(cos(u)) + 0.59 * cos(u) * cos(v) * sin(u) + (-0.5 - 0.59 * cos(v) - 0.5 * sin(u)) * sin(u),
        cos(u) * (-1.56 - 0.59 * cos(v)) + 0.295 * pow<2>(cos(u)) * cos(v) - 0.295 * cos(v) * pow<2>(sin(u)),
        0.295 * sin(u) * sin(v),
      };
    },
    // df(u, v) / dv
    [](double u, double v)->Vec {
      return {
        0.295 * (-2. + cos(u)) * cos(u) * sin(v),
        -0.295 * (-2. + cos(u)) * sin(u) * sin(v),
        -0.295 * (-2. + cos(u)) * cos(v),
      };
    },
    // u, v domain
    {0, M_PI}, {0, 2 * M_PI},
    // color
    color,
    // illumination
    {0},
    // position
    {0, -2.9, 4},
    // deflection
    deflection,
    // u, v steps
    50, 100
  ));
  render->objects.push_back(new Surface(
    // f(u, v)
    [](double u, double v)->Vec {
      return {
        0.25667830632459043 + 0.59 * cos(v) + cos(u) * (-0.5 + 0.295 * cos(v) + 0.5 * sin(u)),
        2.1697863390130157 + 1.56 * sin(u),
        -0.295 * (2. + cos(u)) * sin(v),
      };
    },
    // df(u, v) / du
    [](double u, double v)->Vec {
      return {
        0.5 * pow<2>(cos(u)) + (0.5 - 0.295 * cos(v) - 0.5 * sin(u)) * sin(u),
        1.56 * cos(u),
        0.295 * sin(u) * sin(v),
      };
    },
    // df(u, v) / dv
    [](double u, double v)->Vec {
      return {
        -0.295 * (2. + cos(u)) * sin(v),
        0,
        -0.295 * (2. + cos(u)) * cos(v),
      };
    },
    // u, v domain
    {0, M_PI}, {0, 2 * M_PI},
    // color
    color,
    // illumination
    {0},
    // position
    {0, -2.9, 4},
    // deflection
    deflection,
    // u, v steps
    50, 100
  ));
  return render;
}

Render* pms() {
  const Vec wallColor = {.8};
  const Deflection wallDef = {1, 1, 1, 1, 0};
  constexpr double blockZ = .4;
  const Vec blockColor = {1, .98, 1};
  const Deflection blockDef = {0, .025, 1, 1.1, 0, Deflection::Ordinary};

  std::vector<Object*> objects = {
    new Sphere({-3, -3, 5}, .1, {0}, {500}, {0, 0, 0}),
    new Sphere({3, -3, 5}, .1, {0}, {500}, {0, 0, 0}),
    /// 1. back of box
    new BBBParallelogram(
      // vertices
      {-3, 3, 5}, {3, 3, 5}, {-3, -3, 5},
      wallColor,
      {},
      wallDef
    ),

    /// 2. top of box
    new BBBParallelogram(
      // vertices
      {-3, 3, -1}, {3, 3, -1}, {-3, 3, 5},
      wallColor,
      {},
      wallDef
    ),

    /// 4. left of box
    new BBBParallelogram(
      // vertices
      {-3, -3, -1}, {-3, 3, -1}, {-3, -3, 5},
      wallColor,
      {},
      wallDef
    ),

    //* .2. right of box
    new BBBParallelogram(
      // vertices
      {3, -3, -1}, {3, -3, 5}, {3, 3, -1},
      wallColor,
      {},
      wallDef
    ),

    /// 6. bottom of box
    new BBBParallelogram(
      // vertices
      {-3, -3, -1}, {-3, -3, 5}, {3, -3, -1},
      wallColor,
      {},
      wallDef
    ),

    //new BBBParallelogram(
    //  // vertices
    //  {-3, 2.999, 4.9}, {-3, 2.999, 5}, {-2.99, 3, 4.9},
    //  // color
    //  {1},
    //  // deflection
    //  {0, 0, 1, 1.3}
    //),

    /// (0, 0)
    new BBBParallelogram(
      {-2.5, 2.5, 5 - blockZ}, {-2.5, 2.5, 5}, {-1.5, 2.5, 5 - blockZ},
      blockColor, {}, blockDef
    ),
    new BBBParallelogram(
      {-2.5, 1.5, 5 - blockZ}, {-2.5, 1.5, 5}, {-2.5, 2.5, 5 - blockZ},
      blockColor, {}, blockDef
    ),
    new BBBParallelogram(
      {-1.5, 1.5, 5 - blockZ}, {-1.5, 1.5, 5}, {-2.5, 1.5, 5 - blockZ},
      blockColor, {}, blockDef
    ),
    new BBBParallelogram(
      {-1.5, 2.5, 5 - blockZ}, {-1.5, 2.5, 5}, {-1.5, 1.5, 5 - blockZ},
      blockColor, {}, blockDef
    ),
    new BBBParallelogram(
      {-2.5, 1.5, 5 - blockZ}, {-2.5, 2.5, 5 - blockZ}, {-1.5, 1.5, 5 - blockZ},
      blockColor, {}, blockDef
    ),

    /// (1, 1)
    new BBBParallelogram(
      {-1.5, 1.5, 5 - blockZ}, {-1.5, 1.5, 5}, {-0.5, 1.5, 5 - blockZ},
      blockColor, {}, blockDef
    ),
    new BBBParallelogram(
      {-1.5, 0.5, 5 - blockZ}, {-1.5, 0.5, 5}, {-1.5, 1.5, 5 - blockZ},
      blockColor, {}, blockDef
    ),
    new BBBParallelogram(
      {-0.5, 0.5, 5 - blockZ}, {-0.5, 0.5, 5}, {-1.5, 0.5, 5 - blockZ},
      blockColor, {}, blockDef
    ),
    new BBBParallelogram(
      {-0.5, 1.5, 5 - blockZ}, {-0.5, 1.5, 5}, {-0.5, 0.5, 5 - blockZ},
      blockColor, {}, blockDef
    ),
    new BBBParallelogram(
      {-1.5, 0.5, 5 - blockZ}, {-1.5, 1.5, 5 - blockZ}, {-0.5, 0.5, 5 - blockZ},
      blockColor, {}, blockDef
    ),

    /// (4, 0)
    new BBBParallelogram(
      {1.5, 2.5, 5 - blockZ}, {1.5, 2.5, 5}, {2.5, 2.5, 5 - blockZ},
      blockColor, {}, blockDef
    ),
    new BBBParallelogram(
      {1.5, 1.5, 5 - blockZ}, {1.5, 1.5, 5}, {1.5, 2.5, 5 - blockZ},
      blockColor, {}, blockDef
    ),
    new BBBParallelogram(
      {2.5, 1.5, 5 - blockZ}, {2.5, 1.5, 5}, {1.5, 1.5, 5 - blockZ},
      blockColor, {}, blockDef
    ),
    new BBBParallelogram(
      {2.5, 2.5, 5 - blockZ}, {2.5, 2.5, 5}, {2.5, 1.5, 5 - blockZ},
      blockColor, {}, blockDef
    ),
    new BBBParallelogram(
      {1.5, 1.5, 5 - blockZ}, {1.5, 2.5, 5 - blockZ}, {2.5, 1.5, 5 - blockZ},
      blockColor, {}, blockDef
    ),

    /// (3, 1)
    new BBBParallelogram(
      {0.5, 1.5, 5 - blockZ}, {0.5, 1.5, 5}, {1.5, 1.5, 5 - blockZ},
      blockColor, {}, blockDef
    ),
    new BBBParallelogram(
      {0.5, 0.5, 5 - blockZ}, {0.5, 0.5, 5}, {0.5, 1.5, 5 - blockZ},
      blockColor, {}, blockDef
    ),
    new BBBParallelogram(
      {1.5, 0.5, 5 - blockZ}, {1.5, 0.5, 5}, {0.5, 0.5, 5 - blockZ},
      blockColor, {}, blockDef
    ),
    new BBBParallelogram(
      {1.5, 1.5, 5 - blockZ}, {1.5, 1.5, 5}, {1.5, 0.5, 5 - blockZ},
      blockColor, {}, blockDef
    ),
    new BBBParallelogram(
      {0.5, 0.5, 5 - blockZ}, {0.5, 1.5, 5 - blockZ}, {1.5, 0.5, 5 - blockZ},
      blockColor, {}, blockDef
    ),

    /// (2, 2) & (3, 2)
    new BBBParallelogram(
      {-.5, .5, 5 - blockZ}, {-.5, .5, 5}, {.5, .5, 5 - blockZ},
      blockColor, {}, blockDef
    ),
    new BBBParallelogram(
      {-.5, -1.5, 5 - blockZ}, {-.5, -1.5, 5}, {-.5, .5, 5 - blockZ},
      blockColor, {}, blockDef
    ),
    new BBBParallelogram(
      {.5, -1.5, 5 - blockZ}, {.5, -1.5, 5}, {-.5, -1.5, 5 - blockZ},
      blockColor, {}, blockDef
    ),
    new BBBParallelogram(
      {.5, .5, 5 - blockZ}, {.5, .5, 5}, {.5, -1.5, 5 - blockZ},
      blockColor, {}, blockDef
    ),
    new BBBParallelogram(
      {-.5, -1.5, 5 - blockZ}, {-.5, .5, 5 - blockZ}, {.5, -1.5, 5 - blockZ},
      blockColor, {}, blockDef
    ),

    /// bottom-left
    new BBBParallelogram(
      {-2.5, -.5, 5 - blockZ}, {-2.5, -.5, 5}, {-1.5, -.5, 5 - blockZ},
      blockColor, {}, blockDef
    ),
    new BBBParallelogram(
      {-2.5, -2.5, 5 - blockZ}, {-2.5, -2.5, 5}, {-2.5, -.5, 5 - blockZ},
      blockColor, {}, blockDef
    ),
    new BBBParallelogram(
      {-.5, -2.5, 5 - blockZ}, {-.5, -2.5, 5}, {-2.5, -2.5, 5 - blockZ},
      blockColor, {}, blockDef
    ),
    new BBBParallelogram(
      {-.5, -1.5, 5 - blockZ}, {-.5, -1.5, 5}, {-.5, -2.5, 5 - blockZ},
      blockColor, {}, blockDef
    ),
    new BBBParallelogram(
      {-1.5, -1.5, 5 - blockZ}, {-1.5, -1.5, 5}, {-.5, -1.5, 5 - blockZ},
      blockColor, {}, blockDef
    ),
    new BBBParallelogram(
      {-1.5, -.5, 5 - blockZ}, {-1.5, -.5, 5}, {-1.5, -1.5, 5 - blockZ},
      blockColor, {}, blockDef
    ),
    new BBBParallelogram(
      {-2.5, -2.5, 5 - blockZ}, {-2.5, -.5, 5 - blockZ}, {-1.5, -2.5, 5 - blockZ},
      blockColor, {}, blockDef
    ),
    new BBBParallelogram(
      {-1.5, -2.5, 5 - blockZ}, {-1.5, -1.5, 5 - blockZ}, {-.5, -2.5, 5 - blockZ},
      blockColor, {}, blockDef
    ),

    /// bottom-left
    new BBBParallelogram(
      {-2.5, -.5, 5 - blockZ}, {-2.5, -.5, 5}, {-1.5, -.5, 5 - blockZ},
      blockColor, {}, blockDef
    ),
    new BBBParallelogram(
      {-2.5, -2.5, 5 - blockZ}, {-2.5, -2.5, 5}, {-2.5, -.5, 5 - blockZ},
      blockColor, {}, blockDef
    ),
    new BBBParallelogram(
      {-.5, -2.5, 5 - blockZ}, {-.5, -2.5, 5}, {-2.5, -2.5, 5 - blockZ},
      blockColor, {}, blockDef
    ),
    new BBBParallelogram(
      {-.5, -1.5, 5 - blockZ}, {-.5, -1.5, 5}, {-.5, -2.5, 5 - blockZ},
      blockColor, {}, blockDef
    ),
    new BBBParallelogram(
      {-1.5, -1.5, 5 - blockZ}, {-1.5, -1.5, 5}, {-.5, -1.5, 5 - blockZ},
      blockColor, {}, blockDef
    ),
    new BBBParallelogram(
      {-1.5, -.5, 5 - blockZ}, {-1.5, -.5, 5}, {-1.5, -1.5, 5 - blockZ},
      blockColor, {}, blockDef
    ),
    new BBBParallelogram(
      {-2.5, -2.5, 5 - blockZ}, {-2.5, -.5, 5 - blockZ}, {-1.5, -2.5, 5 - blockZ},
      blockColor, {}, blockDef
    ),
    new BBBParallelogram(
      {-1.5, -2.5, 5 - blockZ}, {-1.5, -1.5, 5 - blockZ}, {-.5, -2.5, 5 - blockZ},
      blockColor, {}, blockDef
    ),

    /// bottom-right
    new BBBParallelogram(
      {1.5, -.5, 5 - blockZ}, {1.5, -.5, 5}, {2.5, -.5, 5 - blockZ},
      blockColor, {}, blockDef
    ),
    new BBBParallelogram(
      {2.5, -.5, 5 - blockZ}, {2.5, -.5, 5}, {2.5, -2.5, 5 - blockZ},
      blockColor, {}, blockDef
    ),
    new BBBParallelogram(
      {2.5, -2.5, 5 - blockZ}, {2.5, -2.5, 5}, {.5, -2.5, 5 - blockZ},
      blockColor, {}, blockDef
    ),
    new BBBParallelogram(
      {.5, -2.5, 5 - blockZ}, {.5, -2.5, 5}, {.5, -1.5, 5 - blockZ},
      blockColor, {}, blockDef
    ),
    new BBBParallelogram(
      {.5, -1.5, 5 - blockZ}, {.5, -1.5, 5}, {1.5, -1.5, 5 - blockZ},
      blockColor, {}, blockDef
    ),
    new BBBParallelogram(
      {1.5, -1.5, 5 - blockZ}, {1.5, -1.5, 5}, {1.5, -.5, 5 - blockZ},
      blockColor, {}, blockDef
    ),
    new BBBParallelogram(
      {1.5, -2.5, 5 - blockZ}, {1.5, -.5, 5 - blockZ}, {2.5, -2.5, 5 - blockZ},
      blockColor, {}, blockDef
    ),
    new BBBParallelogram(
      {.5, -2.5, 5 - blockZ}, {.5, -1.5, 5 - blockZ}, {1.5, -2.5, 5 - blockZ},
      blockColor, {}, blockDef
    ),
  };
  return new Render(objects);
}

Render* getScene() {
  return grass();
}
