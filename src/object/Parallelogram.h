//
// Created by Yixiang Tu on 2019/5/25.
//

#ifndef LAB2_PARALLELOGARM_H
#define LAB2_PARALLELOGARM_H


#include "triangle/TriangleColor.h"
#include "triangle/ParallelogramSpacial.h"
#include "triangle/TriangleIllumination.h"

class BBBParallelogram
  : public BasicParallelogramSpacial,
    public BasicTriangleColor,
    public BasicTriangleIllumination {
public:
  BBBParallelogram(Vec v1, Vec v2, Vec v3,
                   Vec color = {0, 0, 0},
                   Vec illumination = {0, 0, 0},
                   Deflection deflection = {0, 0, 0})
    : Triangle(v1, v2, v3, deflection),
      BasicParallelogramSpacial(),
      BasicTriangleColor(color),
      BasicTriangleIllumination(illumination) {}
};

class BTBParallelogram
  : public BasicParallelogramSpacial,
    public TextureTriangleColor,
    public BasicTriangleIllumination {
public:
  BTBParallelogram(Vec v1, Vec v2, Vec v3,
                       const std::string& filename,
                       Vec illumination = {0, 0, 0},
                       Deflection deflection = {0, 0, 0})
    : Triangle(v1, v2, v3, deflection),
      BasicParallelogramSpacial(),
      TextureTriangleColor(filename),
      BasicTriangleIllumination(illumination) {}
};

class TBBParallelogram
  : public TextureParallelogramSpacial,
    public BasicTriangleColor,
    public BasicTriangleIllumination {
public:
  TBBParallelogram(Vec v1, Vec v2, Vec v3,
                   const std::string& filename,
                   Vec color = {0, 0, 0},
                   Vec illumination = {0, 0, 0},
                   Deflection deflection = {0, 0, 0})
    : Triangle(v1, v2, v3, deflection),
      TextureParallelogramSpacial(filename),
      BasicTriangleColor(color),
      BasicTriangleIllumination(illumination) {}
};

class TTBParallelogram
  : public TextureParallelogramSpacial,
    public TextureTriangleColor,
    public BasicTriangleIllumination {
public:
  TTBParallelogram(Vec v1, Vec v2, Vec v3,
                   const std::string& normalFilename,
                   const std::string& imageFilename,
                   Vec illumination = {0, 0, 0},
                   Deflection deflection = {0, 0, 0})
    : Triangle(v1, v2, v3, deflection),
      TextureParallelogramSpacial(normalFilename),
      TextureTriangleColor(imageFilename),
      BasicTriangleIllumination(illumination) {}
};


#endif //LAB2_PARALLELOGARM_H
