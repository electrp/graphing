//
// Created by willb on 2/5/2026.
//

#include "NLIDeCasteljauPolynomial.h"

NLIDeCasteljauPolynomial::~NLIDeCasteljauPolynomial() {
}

void NLIDeCasteljauPolynomial::generate(std::span<float> inputs) {
    coef = {inputs.begin(), inputs.end()};
}

float NLIDeCasteljauPolynomial::sample(float t) const {
    if (coef.size() == 0)
        return 0;
    if (coef.size() == 1)
        return coef[0];

    std::vector<float> main = coef;
    std::vector<float> sub;
    float inv = 1 - t;
    while (true) {
        for (int i = 0; i < main.size() - 1; ++i) {
            sub.push_back(main[i] * inv + main[i + 1] * t);
        }
        std::swap(main, sub);
        if (main.size() == 1)
            return main[0];
        sub.clear();
    }
}

Curve NLIDeCasteljauPolynomial::generate_curve_obj() {
    return template_generate_curve_obj<NLIDeCasteljauPolynomial>();
}
