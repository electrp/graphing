//
// Created by willb on 2/5/2026.
//

#ifndef GRAPHING_BBDECASTELJAUPOLYNOMIAL_H
#define GRAPHING_BBDECASTELJAUPOLYNOMIAL_H
#include "BaseFunction.h"


struct BBDeCasteljauPolynomial : BaseFunction {
    virtual ~BBDeCasteljauPolynomial();
    void generate(std::span<float> inputs) override;
    float sample(float t) const override;
    Curve generate_curve_obj() override;

    std::vector<float> coef;
};


#endif //GRAPHING_BBDECASTELJAUPOLYNOMIAL_H