//
// Created by Yixiang Tu on 2019/6/25.
//

#ifndef LAB2_PMRECT_H
#define LAB2_PMRECT_H

#include "PMObject.h"
#include "../object/Parallelogram.h"

class PMSingleBBBRect : public BBBParallelogram, public PMSingleRect {

public:
  PMSingleBBBRect(Vec v1, Vec v2, Vec v3, Vec color, Deflection deflection,
                  unsigned pt_width, unsigned pt_height)
    : Triangle(v1, v2, v3, deflection),
      PMSingleRect(((v2 - v1) % (v3 - v1)).length(), pt_width, pt_height),
      BBBParallelogram(v1, v2, v3, color, {}, deflection) {}
};

class PMSingleBTBRect : public BTBParallelogram, public PMSingleRect {

public:
  PMSingleBTBRect(Vec v1, Vec v2, Vec v3, const std::string& filename, Deflection deflection,
                  unsigned pt_width, unsigned pt_height)
    : Triangle(v1, v2, v3, deflection),
      PMSingleRect(((v2 - v1) % (v3 - v1)).length(), pt_width, pt_height),
      BTBParallelogram(v1, v2, v3, filename, {}, deflection) {}
};

class PMPureBRect : public BBBParallelogram, public PMPureRefRect {

public:
  PMPureBRect(Vec v1, Vec v2, Vec v3, Vec color, Deflection deflection)
    : Triangle(v1, v2, v3, deflection),
      PMPureRefRect(),
      BBBParallelogram(v1, v2, v3, color, {}, deflection) {}

};

#endif //LAB2_PMRECT_H
