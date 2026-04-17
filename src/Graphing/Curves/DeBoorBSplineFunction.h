//
// Created by willb on 4/6/2026.
//

#ifndef GRAPHING_DEBOORBSPLINE_H
#define GRAPHING_DEBOORBSPLINE_H
#include "BaseFunction.h"


class DeBoorBSplineFunction : BaseFunction{
public:
    virtual ~DeBoorBSplineFunction();
    void generate(std::span<glm::vec4> inputs) override;
    glm::vec4 sample(float t) override;
    Curve generate_curve_obj() override;
    std::vector<float> coefs;
    float degree = 3;
};


#endif //GRAPHING_DEBOORBSPLINE_H