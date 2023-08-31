//
// Created by Yixiang Tu on 2019/5/21.
//

#ifndef LAB2_TRIANGLE_H
#define LAB2_TRIANGLE_H

#include "triangle/TriangleColor.h"
#include "triangle/TriangleSpacial.h"
#include "triangle/TriangleIllumination.h"

class BBBTriangle
  : public BasicTriangleSpacial,
    public BasicTriangleColor,
    public BasicTriangleIllumination {
public:
  BBBTriangle(Vec v1, Vec v2, Vec v3,
              Vec color = {0, 0, 0},
              Vec illumination = {0, 0, 0},
              Deflection deflection = {0, 0, 0})
    : Triangle(v1, v2, v3, deflection),
      BasicTriangleSpacial(),
      BasicTriangleColor(color),
      BasicTriangleIllumination(illumination) {}
};

class BTBTriangle
  : public BasicTriangleSpacial,
    public TextureTriangleColor,
    public BasicTriangleIllumination {
public:
  BTBTriangle(Vec v1, Vec v2, Vec v3,
              const std::string& filename,
              Vec illumination = {0, 0, 0},
              Deflection deflection = {0, 0, 0})
    : Triangle(v1, v2, v3, deflection),
      BasicTriangleSpacial(),
      TextureTriangleColor(filename),
      BasicTriangleIllumination(illumination) {}
};

class IBBTriangle
  : public InterpolateTriangleSpacial,
    public BasicTriangleColor,
    public BasicTriangleIllumination {
public:
  IBBTriangle(Vec v1, Vec v2, Vec v3,
              Vec n1, Vec n2, Vec n3,
              Vec color = {0, 0, 0},
              Vec illumination = {0, 0, 0},
              Deflection deflection = {0, 0, 0})
    : Triangle(v1, v2, v3, deflection),
      InterpolateTriangleSpacial(n1, n2, n3),
      BasicTriangleColor(color),
      BasicTriangleIllumination(illumination) {}
};

class BABTriangle
  : public BasicTriangleSpacial,
    public AnchorTriangleColor,
    public BasicTriangleIllumination {
public:
  BABTriangle(Vec v1, Vec v2, Vec v3,
              const Texture& texture,
              const Point& p1, const Point& p2, const Point& p3,
              Vec illumination = {0, 0, 0},
              Deflection deflection = {0, 0, 0})
    : Triangle(v1, v2, v3, deflection),
      BasicTriangleSpacial(),
      AnchorTriangleColor(texture, p1, p2, p3),
      BasicTriangleIllumination(illumination) {}
};

class IABTriangle
  : public InterpolateTriangleSpacial,
    public AnchorTriangleColor,
    public BasicTriangleIllumination {
public:
  IABTriangle(Vec v1, Vec v2, Vec v3,
              Vec n1, Vec n2, Vec n3,
              const Texture& texture,
              const Point& p1, const Point& p2, const Point& p3,
              Vec illumination = {0, 0, 0},
              Deflection deflection = {0, 0, 0})
    : Triangle(v1, v2, v3, deflection),
      InterpolateTriangleSpacial(n1, n2, n3),
      AnchorTriangleColor(texture, p1, p2, p3),
      BasicTriangleIllumination(illumination) {}
};

class MeshSurfaceTriangle
  : public BasicInterpolateTriangleSpacial,
    public AnchorTriangleColor,
    public BasicTriangleIllumination {
public:
  MeshSurfaceTriangle(Vec v1, Vec v2, Vec v3,
                      Vec n1, Vec n2, Vec n3,
                      const Texture& texture,
                      const Point& p1, const Point& p2, const Point& p3,
                      Vec illumination = {0, 0, 0},
                      Deflection deflection = {0, 0, 0})
    : Triangle(v1, v2, v3, deflection),
      BasicInterpolateTriangleSpacial(n1, n2, n3),
      AnchorTriangleColor(texture, p1, p2, p3),
      BasicTriangleIllumination(illumination) {}
};

class SurfaceTriangle
  : public SurfaceTriangleSpacial,
    public BasicTriangleColor,
    public BasicTriangleIllumination {
public:
  SurfaceTriangle(Surface* surface, const Point& v1, const Point& v2, const Point& v3);
};

#endif //LAB2_TRIANGLE_H
