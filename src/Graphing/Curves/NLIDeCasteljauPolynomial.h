//
// Created by willb on 2/5/2026.
//

#ifndef GRAPHING_NLIBERNSTEIN_H
#define GRAPHING_NLIBERNSTEIN_H

#include <vector>
#include <flecs.h>
#include <span>

#include "BaseFunction.h"
#include "BBDeCasteljauPolynomial.h"
#include "BBDeCasteljauPolynomial.h"
#include "Graphing/Curve.h"

struct NLIDeCasteljauPolynomial : BaseFunction {
    virtual ~NLIDeCasteljauPolynomial();
    void generate(std::span<glm::vec4> inputs) override;
    glm::vec4 sample(float t) const override;
    Curve generate_curve_obj() override;

    std::vector<glm::vec2> coef;
};


#endif //GRAPHING_NLIBERNSTEIN_H