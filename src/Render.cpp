//
// Created by Yixiang Tu on 2019/5/24.
//

#include "Render.h"

Render::Render(std::vector<Object*> objects)
  : objects(std::move(objects)) {}

void Render::scan() {
  auto zDepth = 1.0;
  auto xBound = (Width > Height ? (double) Width / Height : 1) * tan(fov / 360. * M_PI);
  auto yBound = (Width > Height ? 1 : (double) Height / Width) * tan(fov / 360. * M_PI);
  auto xStep = xBound * 2 / Width;
  auto yStep = yBound * 2 / Height;

  for (auto x = 0; x < Width; x += 1) {
#pragma omp parallel for schedule(dynamic, 1) num_threads(threads)
    for (auto y = 0; y < Height; y += 1) {
      if (image.pixel[x][y].skip()) {
        continue;
      }
      //Vec vec = {-xBound * .38 + (x + rnd()) * xStep * .125, -yBound * .633 + (y + rnd()) * yStep * .125, zDepth};
      Vec vec = {-xBound + (x + rnd()) * xStep, -yBound + (y + rnd()) * yStep, zDepth};
      vec = {vec.x, vec.y * elevation_cos + vec.z * elevation_sin, vec.z * elevation_cos - vec.y * elevation_sin};
      vec = {vec.x * pan_cos + vec.z * pan_sin, vec.y, vec.z * pan_cos - vec.x * pan_sin};

      // gideon
      //auto ny = vec.y * .9886859966642595 + vec.z * .15;
      //auto nz = vec.z * .9886859966642595 - vec.y * .15;
      //vec.y = ny;
      //vec.z = nz;

      // field depth
      double dx = 0;
      double dy = 0;
      auto d = blur_radius * rnd();
      auto angle = rnd() * 2 * M_PI;
      dx = d * sin(angle);
      dy = d * cos(angle);
      vec.x += dx;
      vec.y += dy;

      vec.normalize();

      auto c = cast(vec, source - focus * Vec{dx, dy, 0});
      image.pixel[x][y] += c;
    }
  }
}

SolveContext Render::findIntersection(const Vec& source, const Vec& direction) {
  SolveContext ctx = {source, direction};
  Object* best = nullptr;
  for (auto* o : objects) {
    if (o->solvePosition(ctx)) {
      best = o;
    }
  }
  if (best) {
    best->solve(ctx);
  }
  return ctx;
}

Vec Render::cast(const Vec& vec, Vec source) {
  auto iteration = 0;
  Vec result{0, 0, 0};
  Vec mask{1, 1, 1};
  Vec direction = vec;

  while (true) {
    auto ctx = findIntersection(source, direction);
    if (!ctx) {
      break;
    }
    result += ctx.itsIllumination & mask;
    mask &= ctx.itsColor;
    if (!ctx.nxtDirection or max(mask) < EMACH) {
      break;
    }
    if (++iteration > 5) {
      if (iteration > 20) {
        //break;
        if (rnd() < min(.5, max(mask))) {
          mask /= min(.5, max(mask));
        } else {
          break;
        }
      } else if (rnd() < max(mask)) {
        mask /= max(mask);
      } else {
        break;
      }
    }
    source = ctx.itsPosition;
    direction = ctx.nxtDirection;
  }

  return result;
}

double f(double v) {
  //return v;
  return (log(v + exp(-.5)) + .5) / exp(.5);
}

void Image::Pixel::getValue(unsigned char* c) const {
  auto s = (r + g + b) / count / 3 + EMACH;
  auto multiplier = (log(s + exp(-.5)) + .5) / (s * exp(.5));
  //auto multiplier = 1;
  c[0] = (unsigned char) (clamp(f(r / count)) * 255);
  c[1] = (unsigned char) (clamp(f(g / count)) * 255);
  c[2] = (unsigned char) (clamp(f(b / count)) * 255);
}

Image::Pixel& Image::Pixel::operator+=(const Vec& v) {
  if (canSkip and count > 100) {
    auto diff = (fabs(v.x - r / count) + fabs(v.y - g / count) + fabs(v.z - b / count));
    if (maxDiff < diff) {
      canSkip = false;
    } else if (maxDiff > 2 * diff) {
      maxDiff *= .8;
    }
  }
  r += v.x;
  g += v.y;
  b += v.z;
  count += 1;
  return *this;
}
