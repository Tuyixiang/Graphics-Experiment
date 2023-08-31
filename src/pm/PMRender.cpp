//
// Created by Yixiang Tu on 2019/6/24.
//

#include "PMRender.h"

#ifdef __APPLE__
constexpr int EPOCH = 2;
constexpr int ANIMATION_STEPS = 10;
#else
constexpr int EPOCH = 100;
constexpr int ANIMATION_STEPS = 100;
#endif //__APPLE__

void PMRender::run() {
  for (auto i = 0; i < objects.size(); i += 1) {
    objects[i]->PMLoadTexture(std::to_string(i) + ".pt");
  }

  for (int epoch = 1; epoch <= EPOCH; epoch += 1) {
#pragma omp parallel for schedule(dynamic, PM_BATCH_SIZE / 1000) num_threads(threads)
    for (int n = 0; n < PM_BATCH_SIZE; n += 1) {
      if (n % (PM_BATCH_SIZE / 1000) == 0) {
#pragma omp critical(print_progress)
        std::cout << "epoch " << epoch << "/" << EPOCH
                  << " " << n / (PM_BATCH_SIZE / 100.) << "%\r" << std::flush;
      }
      castPhoton();
    }
    for (int i = 0; i < 5; i += 1) {
      objects[i]->PMSamples(epoch);
    }
    //image.clear();
    //source = {0, 0, 0};
    //scan();
    //image.save(false);

    if (epoch % 2 == 0) {
      for (auto i = 0; i < objects.size(); i += 1) {
        objects[i]->PMSamples(epoch);
        objects[i]->PMSaveTexture(std::to_string(i) + ".pt");
      }
    }
  }
  source = {0, 0, 0};
  std::cout << "rendering\r" << std::flush;
  scan();
  image.save(false);
  return;
  for (int c = 0; c <= ANIMATION_STEPS; c += 1) {
    std::cout << "frame " << c << " " << std::flush;
    source = {0, (-3 + c * 6. / ANIMATION_STEPS) * .9, 0};
    elevation_cos = 5 / (Vec{0, 0, 5} - source).length();
    elevation_sin = -source.y / (Vec{0, 0, 5} - source).length();
    image.clear();
    scan();
    image.save(false, "a" + std::to_string(c) + ".ppm");
  }
  for (int c = 0; c <= ANIMATION_STEPS; c += 1) {
    std::cout << "frame " << c << " " << std::flush;
    source = {-2 + c * 4. / ANIMATION_STEPS, -2 + c * 4. / ANIMATION_STEPS, 2};
    elevation_cos = 1;
    elevation_sin = 0;
    image.clear();
    scan();
    image.save(false, "b" + std::to_string(c) + ".ppm");
  }

}

void PMRender::castPhoton() {
  auto ctx = photonGenerator();
  auto iteration = 0;

  while (true) {
    Object* closest = nullptr;
    for (auto* o : objects) {
      if (o->solvePosition(ctx)) {
        closest = o;
      }
    }
    // 找到击中物体并 PMSolve，其中如果漫反射，会把光子烧进去
    if (closest) {
      closest->PMCast(ctx);
    } else {
      break;
    }

    // 光子被吸收则停止
    if (not ctx.photon or not ctx.nxtDirection) {
      break;
    }

    if (++iteration > 5) {
      if (iteration > 20) {
        //break;
        auto v = min(.5, ctx.photon.luminosity());
        if (not ctx.photon.roulette(v)) {
          break;
        }
      }
      if (not ctx.photon.roulette(ctx.photon.luminosity())) {
        break;
      }
    }

    // 本次的位置作为下次起点
    ctx.lgtSource = ctx.itsPosition;
    ctx.lgtDirection = ctx.nxtDirection;
    ctx.distance = NO_INTERSECTION;
    ctx.target = nullptr;
    ctx.itsPosition = ctx.itsColor = ctx.itsNormal = {};
  }
}

Object* PMRender::findObject(SolveContext& ctx) {
  Object* best = nullptr;
  for (auto* o : objects) {
    if (o->solvePosition(ctx)) {
      best = o;
    }
  }
  return best;
  //if (best) {
  //  best->PMView(ctx);
  //}
  //return ctx;
}

Vec PMRender::PMCast(const Vec& vec, Vec source, int depth) {
  Vec result{0, 0, 0};
  Vec mask{1, 1, 1};
  Vec direction = vec;
  SolveContext ctx = {source, direction};

  auto object = findObject(ctx);
  if (not object) {
    return {};
  }

  if (depth < 16) {
    /// 遍历三种反射类型，递归

    auto diffuse = object->deflection.diffuse;
    auto reflect = object->deflection.reflect - object->deflection.diffuse;
    auto refract = object->deflection.refract - object->deflection.reflect;

    if (diffuse > EMACH) {
      ctx.type = Diffuse;
      object->PMView(ctx);
      result += ctx.itsIllumination & mask * diffuse;
    }
    if (reflect > EMACH) {
      ctx.type = Reflect;
      object->PMView(ctx);
      result += PMCast(ctx.nxtDirection, ctx.itsPosition, depth + 1) & mask * reflect;
    }
    if (refract > EMACH) {
      ctx.type = Refract;
      object->PMView(ctx);
      result += PMCast(ctx.nxtDirection, ctx.itsPosition, depth + 1) & mask * refract;
    }
  }
  /// 不再使用 Russian Roulette，因为只扫描很少次数，随机没有多大意义

  return result;
}

void PMRender::scan() {
  auto zDepth = 1.0;
  auto xBound = (Width > Height ? (double) Width / Height : 1) * tan(fov / 360. * M_PI);
  auto yBound = (Width > Height ? 1 : (double) Height / Width) * tan(fov / 360. * M_PI);
  auto xStep = xBound * 2 / Width;
  auto yStep = yBound * 2 / Height;

  for (auto x = 0; x < Width; x += 1) {
#pragma omp parallel for schedule(dynamic, 1) num_threads(threads)
    for (auto y = 0; y < Height; y += 1) {
      // 每个像素采样四次
      for (auto dx: {.25, .75}) {
        for (auto dy: {.25, .75}) {
          Vec vec = {-xBound + (x + dx) * xStep, -yBound + (y + dy) * yStep, zDepth};
          vec = {vec.x, vec.y * elevation_cos + vec.z * elevation_sin, vec.z * elevation_cos - vec.y * elevation_sin};
          vec = {vec.x * pan_cos + vec.z * pan_sin, vec.y, vec.z * pan_cos - vec.x * pan_sin};

          // field depth
          //double dx = 0;
          //double dy = 0;
          //auto d = blur_radius * rnd();
          //auto angle = rnd() * 2 * M_PI;
          //dx = d * sin(angle);
          //dy = d * cos(angle);
          //vec.x += dx;
          //vec.y += dy;

          vec.normalize();

          //auto c = PMCast(vec, source - focus * Vec{dx, dy, 0}, 0);
          auto c = PMCast(vec, source, 0);
          image.pixel[x][y] += c;
        }
      }
    }
  }
}