//
// Created by Yixiang Tu on 2019/5/21.
//

#include "Triangle.h"
#include "surface/Surface.h"

SurfaceTriangle::SurfaceTriangle(Surface* surface, const Point& v1, const Point& v2, const Point& v3)
  : Triangle(surface->func(v1.x, v1.y) + surface->position,
             surface->func(v2.x, v2.y) + surface->position,
             surface->func(v3.x, v3.y) + surface->position),
    SurfaceTriangleSpacial(surface, v1, v2, v3),
    BasicTriangleColor({}),
    BasicTriangleIllumination(illumination) {}
