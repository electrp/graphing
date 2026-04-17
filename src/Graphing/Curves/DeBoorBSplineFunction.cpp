//
// Created by willb on 4/6/2026.
//

#include "DeBoorBSplineFunction.h"

DeBoorBSplineFunction::~DeBoorBSplineFunction() {
}

void DeBoorBSplineFunction::generate(std::span<glm::vec4> inputs) {
    coefs.clear();
    for (auto& input : inputs) {
        coefs.push_back(input[1]);
    }
}

glm::vec4 DeBoorBSplineFunction::sample(float t) {
    int J = floor(t); // Segment index

    std::vector<float> vs; // Value list
    for (int i = 0; i <= degree; ++i) {
        int idx = i + J - degree;

        if (idx < 0 || idx >= coefs.size())
            vs.push_back(0);
        else
            vs.push_back(coefs[idx]);
    }

    for (int p = 1; p <= degree; ++p) {
        for (int i = degree; i >= p; i--) {
            float a =
                (t - J - i + degree) / (1 - p + degree);
            vs[i] = (1 - a) * vs[i - 1] + a * vs[i];
        }
    }

    return {vs[degree], vs[degree], 0, 1};
}

Curve DeBoorBSplineFunction::generate_curve_obj() {
    return template_generate_curve_obj<DeBoorBSplineFunction>();
}
