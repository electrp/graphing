//
// Created by willb on 2/5/2026.
//

#include "BBDeCasteljauPolynomial.h"

#include "Graphing/PascalCache.h"

void BBDeCasteljauPolynomial::generate(std::span<float> inputs) {
    coef = {
        inputs.begin(), inputs.end()
    };

    for (int i = 0; i < coef.size(); ++i) {
        coef[i] *= GetCombination(coef.size() - 1, i);
    }
}

float BBDeCasteljauPolynomial::sample(float t) const {
    if (coef.size() == 0)
        return 0;
    if (coef.size() == 1)
        return coef[0];

    std::vector<float> t_vals;
    std::vector<float> it_vals;
    t_vals.resize(coef.size());
    it_vals.resize(coef.size());
    t_vals[0] = 1;
    it_vals[0] = 1;
    t_vals[1] = t;
    it_vals[1] = 1 - t;

    for (int i = 1; i < coef.size(); ++i) {
        t_vals[i] = t_vals[i - 1] * t;
        it_vals[i] = it_vals[i - 1] * (1 - t);
    }

    float sum = 0;
    for (int i = 0; i < coef.size(); ++i) {
        sum += t_vals[i] * it_vals[coef.size() - i - 1] * coef[i];
    }
    return sum;
}



Curve BBDeCasteljauPolynomial::generate_curve_obj() {
    return template_generate_curve_obj<BBDeCasteljauPolynomial>();
}
