//
// Created by willb on 2/5/2026.
//

#include "BBDeCasteljauPolynomial.h"
#include "BBDeCasteljauPolynomial.h"
#include "BBDeCasteljauPolynomial.h"
#include "BBDeCasteljauPolynomial.h"
#include "BBDeCasteljauPolynomial.h"
#include "BBDeCasteljauPolynomial.h"
#include "BBDeCasteljauPolynomial.h"
#include "BBDeCasteljauPolynomial.h"
#include "BBDeCasteljauPolynomial.h"
#include "BBDeCasteljauPolynomial.h"
#include "BBDeCasteljauPolynomial.h"
#include "Graphing/PascalCache.h"

#include <algorithm>

BBDeCasteljauPolynomial::~BBDeCasteljauPolynomial() {
}

void BBDeCasteljauPolynomial::generate(std::span<glm::vec4> inputs) {
    coef = {
        inputs.begin(), inputs.end()
    };

    if (coef.size() <= 1)
        return;

    std::sort(coef.begin(), coef.end(), [&](glm::vec2 const& a, glm::vec2 const& b) { return a.x < b.x; });

    for (int i = 0; i < coef.size(); ++i) {
        coef[i] *= GetCombination(coef.size() - 1, i);
    }
}

glm::vec4 BBDeCasteljauPolynomial::sample(float t) const {
    if (coef.size() == 0)
        return glm::vec4{0};
    if (coef.size() == 1)
        return glm::vec4{coef[0].y};

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
        sum += t_vals[i] * it_vals[coef.size() - i - 1] * coef[i].y;
    }
    return glm::vec4{sum};
}



Curve BBDeCasteljauPolynomial::generate_curve_obj() {
    return template_generate_curve_obj<BBDeCasteljauPolynomial>();
}
