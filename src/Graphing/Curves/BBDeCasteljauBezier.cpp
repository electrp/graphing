//
// Created by willb on 2/7/2026.
//

#include "BBDeCasteljauBezier.h"
#include <algorithm>

#include "Graphing/PascalCache.h"

BBDeCasteljauBezier::~BBDeCasteljauBezier() {
}

void BBDeCasteljauBezier::generate(std::span<glm::vec4> inputs) {
    coef = {inputs.begin(), inputs.end()};
    // w is the order
    std::sort(coef.begin(), coef.end(), [&](glm::vec4 a, glm::vec4 b) {
        return a.w < b.w;
    });

    for (int i = 0; i < coef.size(); ++i) {
        coef[i].x *= GetCombination(coef.size() - 1, i);
        coef[i].y *= GetCombination(coef.size() - 1, i);
    }
}

glm::vec4 BBDeCasteljauBezier::sample(float t) const {
    if (coef.size() == 0)
        return glm::vec4{0};
    if (coef.size() == 1)
        return glm::vec4{coef[0].x, coef[0].y, 0, 0};

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

    glm::vec2 sum = {0, 0};
    for (int i = 0; i < coef.size(); ++i) {
        sum += glm::vec2{coef[i].x, coef[i].y} * t_vals[i] * it_vals[coef.size() - i - 1];
    }
    return glm::vec4{sum.x, sum.y, 0, 0};
}

Curve BBDeCasteljauBezier::generate_curve_obj() {
    return template_generate_curve_obj<BBDeCasteljauBezier>();
}
