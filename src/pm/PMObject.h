//
// Created by Yixiang Tu on 2019/6/24.
//

#ifndef LAB2_PMOBJECT_H
#define LAB2_PMOBJECT_H

#include "PhotonTexture.h"

class PMSingleRect: public virtual Triangle {
public:
  RectPhotonTexture photonTexture;

  void PMReceivePhoton(PMSolveContext& ctx) override {
    photonTexture.receive(ctx.solverData[0], ctx.solverData[1], ctx.photon);
  }

  void PMGetIllumination(SolveContext& ctx) override {
    ctx.itsIllumination = photonTexture.get(ctx.solverData[0], ctx.solverData[1]);
  }

public:
  PMSingleRect(double area, unsigned pt_width, unsigned pt_height): photonTexture(area, pt_width, pt_height) {}

  void PMSamples(int samples) override {
    photonTexture.samples = samples;
  }

  void PMLoadTexture(const std::string& filename) override {
    photonTexture.load(filename);
  }

  void PMSaveTexture(const std::string& filename) override {
    photonTexture.save(filename);
  }

};

class PMPureRefRect: public virtual Triangle {

public:
  void PMReceivePhoton(PMSolveContext&) override {}

  void PMGetIllumination(SolveContext& ctx) override {}

  void PMSamples(int samples) override {}

};




#endif //LAB2_PMOBJECT_H
