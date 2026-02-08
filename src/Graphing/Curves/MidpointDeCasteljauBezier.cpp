//
// Created by willb on 2/7/2026.
//

#include "MidpointDeCasteljauBezier.h"
#include <algorithm>

MidpointDeCasteljauBezier::~MidpointDeCasteljauBezier() {
}

void MidpointDeCasteljauBezier::generate(std::span<glm::vec4> inputs) {
    std::vector<glm::vec4> coef = {inputs.begin(), inputs.end()};

    std::sort(coef.begin(), coef.end(), [&](glm::vec4 a, glm::vec4 b) {
        return a.w < b.w;
    });

    if (coef.size() == 0) {
        stored_samples = {{0, 0}};
        return;
    }
    if (coef.size() == 1) {
        stored_samples = {{coef[0].x, coef[0].y}};
        return;
    }

    given_samples.clear();
    for (int i = 0; i < coef.size(); ++i) {
        given_samples.emplace_back(coef[i].x, coef[i].y);
    }

    regenerate();
}

glm::vec4 MidpointDeCasteljauBezier::sample(float t) const {
    auto v = stored_samples[t * stored_samples.size()];
    return {v.x, v.y, 0, 0};
}

Curve MidpointDeCasteljauBezier::generate_curve_obj() {
    return template_generate_curve_obj<MidpointDeCasteljauBezier>();
}

void MidpointDeCasteljauBezier::regenerate() {
    if (given_samples.size() < 2)
        return;

    std::vector<glm::vec2> subdivisions = given_samples;
    std::vector<glm::vec2> sub;

    if (subdivisions.size() == 0) {
        stored_samples = {{0, 0}};
        return;
    }
    if (subdivisions.size() == 0) {
        stored_samples = {{0, 0}};
        return;
    }

    for (int i = 0; i < subdivision_count; ++i) {
        int last_sub_size = subdivisions.size();
        stored_samples.resize(last_sub_size * 2 - 1);

        for (int j = 0; j < last_sub_size - 1; ++j) {
            sub.clear();
            stored_samples[j] = subdivisions.front();
            stored_samples[stored_samples.size() - j - 1] = subdivisions.back();
            for (int k = 0; k < subdivisions.size() - 1; ++k) {
                sub.push_back(subdivisions[k] * .5f + subdivisions[k + 1] * .5f);
            }
            std::swap(sub, subdivisions);
        }
        stored_samples[last_sub_size - 1] = subdivisions.front();

        if (i < subdivision_count - 1) {
            std::swap(subdivisions, stored_samples);
            stored_samples.clear();
        }
    }
}

void CurveDrawerFt_MidpointDeCasteljauBezier(flecs::entity e, GraphingWindow &window, GraphingWindow::GraphingContext &ctx) {
    MidpointDeCasteljauBezier const& curve = e.get<MidpointDeCasteljauBezier>();
    auto* dl = ImGui::GetWindowDrawList();

    if (curve.stored_samples.size() < 2)
        return;

    for (int i = 0; i < curve.stored_samples.size() - 1; ++i) {
        dl->AddLine(
            ctx.world_to_screen(curve.stored_samples[i]) + ctx.canvas_p0,
            ctx.world_to_screen(curve.stored_samples[i + 1]) + ctx.canvas_p0,
            ImColor(155, 255, 255, 128)
        );
    }
}
