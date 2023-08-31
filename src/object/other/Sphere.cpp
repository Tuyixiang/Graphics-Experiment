//
// Created by Yixiang Tu on 2019/5/31.
//

#include "Sphere.h"

Sphere::Sphere(Vec position, double radius, Vec color, Vec illumination, Deflection deflection) :
  Object(position,
         AABB({position.x - radius, position.x + radius},
              {position.y - radius, position.y + radius},
              {position.z - radius, position.z + radius}),
         deflection),
  radius(radius), radius2(radius * radius),
  color(color),
  illumination(illumination) {}

bool Sphere::solvePosition(SolveContext& ctx) {
  auto displacement = position - ctx.lgtSource;
  auto distance = fabs(displacement.length() - radius);  // minimum possible distance
  if (ctx and ctx.distance < distance) {
    return false;
  }
  auto prod = ctx.lgtDirection * displacement;
  auto d2 = displacement.squareLength() - prod * prod;
  if (d2 > radius2) {  // no intersection
    return false;
  }

  auto half_chord_length = sqrt(radius2 - d2);

  // the length of ray before it hits the sphere
  distance = prod - half_chord_length;
  if (distance < -EMACH) {
    distance += 2 * half_chord_length;
  }
  if (distance < EMACH) {
    return false;
  }

  if (!ctx or distance < ctx.distance) {
    assert(distance > EMACH);
    ctx.distance = distance;
    ctx.itsPosition = ctx.lgtSource + ctx.lgtDirection * distance;
    ctx.target = this;
    return true;
  }
  return false;
}
