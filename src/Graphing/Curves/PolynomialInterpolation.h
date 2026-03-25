//
// Created by willb on 2/25/2026.
//

#ifndef GRAPHING_POLYNOMIALINTERPOLATION_H
#define GRAPHING_POLYNOMIALINTERPOLATION_H
#include "BaseFunction.h"
#include "Graphing/DividedDifference.h"


struct PolynomialInterpolation : BaseFunction {
    virtual ~PolynomialInterpolation();
    void generate(std::span<glm::vec4> inputs) override;
    glm::vec4 sample(float t) override;
    Curve generate_curve_obj() override;

    DividedDifference diff;
};


#endif //GRAPHING_POLYNOMIALINTERPOLATION_H