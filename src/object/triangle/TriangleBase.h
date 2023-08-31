//
// Created by Yixiang Tu on 2019/5/24.
//

#ifndef LAB2_TRIANGLEBASE_H
#define LAB2_TRIANGLEBASE_H


#include "../Object.h"

class Triangle : public Object {
public:
  Vec edge1;
  Vec edge2;

public:
  Triangle() = default;

  Triangle(Vec v1, Vec v2, Vec v3, Deflection deflection = {})
    : Object(v1, {v1, v2, v3}, deflection), edge1(v2 - v1), edge2(v3 - v1) {}

};


#endif //LAB2_TRIANGLEBASE_H
