//
// Created by Yixiang Tu on 2019/5/30.
//

#include "Object.h"
#include "../config.h"

#ifdef USE_PHONG

void phong(SolveContext& ctx) {
  static double ambient = .2;
  static double diffuse = .6;
  static double specular = .4;
  static Vec light_color = {1};
  //static Vec light_source = {1e4, 1e4, -3e4};
  static Vec light_source = {0, 2.5, 2};
  if (ctx.requireHD) {
    return;
  }
  auto light_direction = normalized(ctx.itsPosition - light_source);
  if (light_direction * ctx.itsNormal > 0) {
    ctx.itsIllumination += ctx.itsColor & (light_color * ambient);
  } else {
    auto reflect_direction = light_direction - ctx.itsNormal * light_direction * 2 * ctx.itsNormal;
    auto specular_coeff = -(reflect_direction * ctx.lgtDirection);
    ctx.itsIllumination += ctx.itsColor & (light_color * (
      ambient -
      light_direction * ctx.itsNormal * diffuse +
      (specular_coeff > 0 ? specular_coeff * specular_coeff : 0) * specular
    ));
  }
  ctx.nxtDirection = {};
}

#endif

DeflectionType Deflection::spin() const {
  auto r = rnd();
  if (r <= diffuse) {
    return Diffuse;
  } else if (r <= reflect) {
    return Reflect;
  } else if (r <= refract) {
    return Refract;
  } else {
    return NoDeflection;
  }
}

void Deflection::solve(SolveContext& ctx) const {
  auto& direction = ctx.lgtDirection;
  auto& normal = ctx.itsNormal;
  auto& type = ctx.type;
  auto prod = direction * normal; // always negative
  auto fromOutside = (prod < 0);
  auto _normal = fromOutside ? normal : -normal; // always inverse with direction
  Vec disturb = {0};

  if (rugged > 0 and type != Diffuse) {
    ctx.requireHD = false;
    disturb = Vec::random() * rugged;
    //_normal = normalized(_normal + disturb);
    prod = direction * _normal;
  } else {
    if (!fromOutside) {
      prod = -prod;
    }
  }

  switch (type) {
    case Diffuse: {
      auto x = normalized(Vec{pn_rnd(), pn_rnd(), pn_rnd()} ^ _normal);
      auto y = _normal % x;
      auto r2 = rnd();
      auto r = sqrt(r2);
      auto theta = rnd() * 2 * M_PI;
      ctx.nxtDirection = (x * (r * sin(theta)) + y * (r * cos(theta))) + sqrt(1 - r2) * _normal;
      ctx.requireHD = false;
      ctx.type = Diffuse;
      return;
      //return normalized(_normal + Vec::random() * 2);
    }
    case Refract: {
      if (n * ctx.refraction_index() == 1) {
        ctx.nxtDirection = rugged > 0 ? normalized(direction + 2 * disturb) : direction;
        return;
      }
      auto cos_i2 = prod * prod;
      auto n_ = fromOutside ? 1 / (n * ctx.refraction_index()) : (n * ctx.refraction_index());
      auto cos_r2 = 1 - (1 - cos_i2) * pow<2>(n_);

      if (cos_r2 > 0) {
        if (flag & Fresnel) {
          // Fresnel
          auto cos_ir = sqrt(cos_i2 * cos_r2);
          auto a = cos_ir * 4 * n_;
          auto rp2 = a / ((cos_i2 * n_ + cos_ir * 2) * n_ + cos_r2);
          auto rv2 = a / ((cos_r2 * n_ + cos_ir * 2) * n_ + cos_i2);

          if (rnd() * 2 < rp2 + rv2) {
            auto precise = (direction - prod * _normal) * n_ - _normal * sqrt(cos_r2);
            if (rugged) {
              ctx.nxtDirection = normalized(disturb + precise);
            } else {
              ctx.nxtDirection = precise;
            }
            ctx.type = Refract;
            return;
          }
        } else {
          //if (rnd() / 8 < cos_r2) {
          auto precise = (direction - prod * _normal) * n_ - _normal * sqrt(cos_r2);
          if (rugged) {
            ctx.nxtDirection = normalized(disturb + precise);
          } else {
            ctx.nxtDirection = precise;
          }
          ctx.type = Refract;
          return;
          //}
        }
      }
    }
    case Reflect: {
      auto precise = direction - 2 * prod * _normal;
      if (rugged > 0) {
        auto possible = precise + disturb;
        if (possible * normal < 0 == fromOutside) {
          ctx.nxtDirection = {};
        } else {
          ctx.nxtDirection = normalized(possible);
        }
      } else {
        ctx.nxtDirection = precise;
      }
      ctx.type = Reflect;
    }
    default: {
      return;
    }
  }
}

Deflection::Deflection(double diffuse, double reflect, double refract, double n, double rugged, unsigned flag)
  : n(n), rugged(rugged), diffuse(diffuse), reflect(reflect), refract(refract), flag(flag) {}

Texture* Texture::load(const std::string& filename) {
  if (filename.empty()) {
    return nullptr;
  }
  printf("loading texture file \"%s\"\n", filename.data());
  auto f = open(filename);
  int width, height;
  fscanf(f, "P6\n%d\n%d\n255\n", &width, &height);

  auto t = new Texture{width, height};
  for (auto y = 0; y < height; y += 1) {
    for (auto x = 0; x < width; x += 1) {
      assert(fread(&(*t)[y][x], 1, 3, f) == 3);
    }
  }
  fclose(f);
  return t;
}

void Texture::inverse() {
  each([](Pix& v)->void {
    for (auto i = 0; i < 3; i += 1) {
      v.v[i] = (uchar) 255u - v.v[i];
    }
  });
}

void Texture::mean(int radius) {
  Texture newImage = {(int) width(), (int) height()};
  for (auto y = 0; y < height(); y += 1) {
    for (auto x = 0; x < width(); x += 1) {
      Vec sum = {0, 0, 0};
      int count = 0;
      for (auto dx = -radius; dx <= radius; dx += 1) {
        for (auto dy = -radius; dy <= radius; dy += 1) {
          auto nx = x + dx;
          auto ny = y + dy;
          if (!inRange(nx, ny)) {
            continue;
          }
          sum += Mat<Pix>::at(nx, ny);
          count += 1;
        }
      }
      newImage.at(x, y) = Pix(sum / count);
    }
  }
  swap(newImage);
}

void Texture::toNorm(const Vec& vx, const Vec& vy) {
  mean(2);
  //inverse();
  Vec mat[5][5] = {
    {
      -2 * vx + 2 * vy,
      -vx + 2 * vy,
      2 * vy,
      vx + 2 * vy,
      2 * vx + 2 * vy,
    },
    {
      -2 * vx + vy,
      -vx + vy,
      vy,
      vx + vy,
      2 * vx + vy,
    },
    {
      -2 * vx,
      -vx,
      {0},
      vx,
      2 * vx,
    },
    {
      -2 * vx - vy,
      -vx - vy,
      -vy,
      vx - vy,
      2 * vx - vy,
    },
    {
      -2 * vx - 2 * vy,
      -vx - 2 * vy,
      -2 * vy,
      vx - 2 * vy,
      2 * vx - 2 * vy,
    },
  };
  Texture norm = {(int) width(), (int) height()};
  for (auto y = 0; y < height(); y += 1) {
    for (auto x = 0; x < width(); x += 1) {
      auto n = (vx % vy) * 16;
      for (auto dx = 0; dx < 5; dx += 1) {
        for (auto dy = 0; dy < 5; dy += 1) {
          n += mat[dx][dy] * sum(at(x + dx - 2, y + dy - 2));
        }
      }
      norm.at(x, y) = Pix(normalized(n));
    }
  }
  swap(norm);
}

Texture* Texture::scaleDown(int v) const {
  auto t = new Texture(static_cast<int>(width() / v), static_cast<int>(height() / v));
  for (auto x = 0; x < t->width(); x += 1) {
    for (auto y = 0; y < t->height(); y += 1) {
      Vec sum;
      for (auto dx = 0; dx < v; dx += 1) {
        for (auto dy = 0; dy < v; dy += 1) {
          sum += (*this)[x + dx][y + dy];
        }
      }
      (*t)[x][y] = sum / (v * v);
    }
  }
  return t;
}

void Object::solve(SolveContext& ctx) {
  hit_count += 1;
  /*
  if (!ctx.itsPosition) {
    solvePosition(ctx);
  }
  if (!ctx) { // no intersection
    return;
  }
  assert(!ctx.itsNormal and !ctx.itsColor and !ctx.itsIllumination and !ctx.nxtDirection);
  */
  if (!ctx.itsNormal) { solveNormal(ctx); }
  if (!ctx.itsColor) { solveColor(ctx); }
  if (!ctx.itsIllumination) { solveIllumination(ctx); }
  ctx.type = deflection.spin();
  if (!ctx.nxtDirection) { solveDirection(ctx); }
#ifdef USE_PHONG
  phong(ctx);
#endif
}

void Object::solveDirection(SolveContext& ctx) {
  deflection.solve(ctx);
}
