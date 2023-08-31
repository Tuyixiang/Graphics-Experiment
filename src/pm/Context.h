//
// Created by Yixiang Tu on 2019/6/24.
//

#ifndef LAB2_CONTEXT_H
#define LAB2_CONTEXT_H

#include "../Basics.h"
#include "Photon.h"

struct PMSolveContext: public SolveContext {
  Photon photon;

  PMSolveContext() = default;

  double refraction_index() const override {
    return photon.wavelength * .1 + 1;
  }
};

#endif //LAB2_CONTEXT_H
