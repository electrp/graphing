//
// Created by willb on 2/5/2026.
//

#ifndef GRAPHING_NLIBERNSTEIN_H
#define GRAPHING_NLIBERNSTEIN_H

#include <vector>
#include <flecs.h>
#include <span>

#include "BaseFunction.h"
#include "Graphing/Curve.h"

struct NLIDeCasteljauPolynomial : BaseFunction {
    void generate(std::span<float> inputs) override;
    float sample(float t) const override;
    Curve generate_curve_obj();

    std::vector<float> coef;
};


#endif //GRAPHING_NLIBERNSTEIN_H