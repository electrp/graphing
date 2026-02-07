//
// Created by willb on 2/5/2026.
//

#ifndef GRAPHING_BBDECASTELJAUPOLYNOMIAL_H
#define GRAPHING_BBDECASTELJAUPOLYNOMIAL_H
#include "BaseFunction.h"
#include "BBDeCasteljauPolynomial.h"
#include "BBDeCasteljauPolynomial.h"


struct BBDeCasteljauPolynomial : BaseFunction {
    virtual ~BBDeCasteljauPolynomial();
    void generate(std::span<glm::vec4> inputs) override;
    glm::vec4 sample(float t) const override;
    Curve generate_curve_obj() override;

    std::vector<glm::vec2> coef;
};


#endif //GRAPHING_BBDECASTELJAUPOLYNOMIAL_H