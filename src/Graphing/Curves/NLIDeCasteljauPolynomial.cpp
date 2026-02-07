//
// Created by willb on 2/5/2026.
//

#include "NLIDeCasteljauPolynomial.h"
#include <algorithm>

#include "BBDeCasteljauPolynomial.h"
#include "BBDeCasteljauPolynomial.h"

NLIDeCasteljauPolynomial::~NLIDeCasteljauPolynomial() {
}

void NLIDeCasteljauPolynomial::generate(std::span<glm::vec4> inputs) {
    coef = {inputs.begin(), inputs.end()};

    std::sort(coef.begin(), coef.end(), [&](glm::vec2 const& a, glm::vec2 const& b) { return a.x < b.x; });
}

glm::vec4 NLIDeCasteljauPolynomial::sample(float t) const {
    if (coef.size() == 0)
        return glm::vec4{0};
    if (coef.size() == 1)
        return glm::vec4{coef[0].y};

    std::vector<float> main;
    for (auto& val : coef) main.push_back(val.y);
    std::vector<float> sub;
    float inv = 1 - t;
    while (true) {
        for (int i = 0; i < main.size() - 1; ++i) {
            sub.push_back(main[i] * inv + main[i + 1] * t);
        }
        std::swap(main, sub);
        if (main.size() == 1)
            return glm::vec4{main[0]};
        sub.clear();
    }
}

Curve NLIDeCasteljauPolynomial::generate_curve_obj() {
    return template_generate_curve_obj<NLIDeCasteljauPolynomial>();
}
