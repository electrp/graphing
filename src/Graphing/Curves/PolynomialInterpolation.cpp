//
// Created by willb on 2/25/2026.
//

#include "PolynomialInterpolation.h"

PolynomialInterpolation::~PolynomialInterpolation() {
}

void PolynomialInterpolation::generate(std::span<glm::vec4> inputs) {
    diff.resize(inputs.size());
    for (int i = 0; i < inputs.size(); ++i) {
        if (diff.get_value(i) != inputs[i]) {
            diff.push_internal_index_to_back(i);
            diff.set(inputs[i], i);
        }
    }
}

glm::vec4 PolynomialInterpolation::sample(float t) {
    return diff.sample_polynomial(t);
}

Curve PolynomialInterpolation::generate_curve_obj() {
    return template_generate_curve_obj<PolynomialInterpolation>();
}

