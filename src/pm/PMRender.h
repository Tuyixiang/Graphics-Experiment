//
// Created by Yixiang Tu on 2019/6/24.
//

#ifndef LAB2_PMRENDER_H
#define LAB2_PMRENDER_H

#include "PMObject.h"
#include "../Render.h"


class PMRender : public Render {

public:
  std::function<PMSolveContext(void)> photonGenerator;

public:
  PMRender(std::vector<Object*> objects, std::function<PMSolveContext(void)> photonGenerator)
    : Render(std::move(objects)), photonGenerator(std::move(photonGenerator)) {}

  void run();

  void scan() override;

  void castPhoton();

  Object* findObject(SolveContext& ctx);

  Vec PMCast(const Vec& vec, Vec source, int depth = 0);

};


#endif //LAB2_PMRENDER_H
