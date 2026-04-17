//
// Created by willb on 4/13/2026.
//

#include "DeBoorBeSplineCurve.h"
#include <algorithm>

DeBoorBSplineCurve::~DeBoorBSplineCurve() {
}

void DeBoorBSplineCurve::generate(std::span<glm::vec4> inputs) {

    points = {inputs.begin(), inputs.end()};
}

glm::vec4 DeBoorBSplineCurve::sample(float t) {
    // Update knot sequence
    while (knot_sequence.size() < points.size() + degree + 1) {
        float knot = std::max(static_cast<int>(knot_sequence.size()) - degree + 1, 0);
        knot_sequence.push_back(knot);
    }
    while (knot_sequence.size() > points.size() + degree + 1) {
        knot_sequence.pop_back();
    }
    std::sort(knot_sequence.begin(), knot_sequence.end());

    if (points.size() < degree ||
        knot_sequence.front() == knot_sequence.back())
        return {0, 0, 0, 0};

    // Adjust sample range to be between t_d and t_(N-d)
    t = t * (knot_sequence[knot_sequence.size() - degree - 1] - knot_sequence[degree]) + knot_sequence[degree];

    const int s = points.size() - 1;

    // Find the last value less than t
    int J = std::upper_bound(knot_sequence.begin(), knot_sequence.end(), t) - knot_sequence.begin() - 1;
    J = std::clamp(J, 0, static_cast<int>(knot_sequence.size() - 1));

    // If j has reached <= s, the result is out of bounds, so clip
    if (J > s)
        J = s;

    // Input the values from P_J to P_(J+d+1)
    std::vector<glm::vec4> vs; // Value list
    for (int i = 0; i <= degree; ++i)
        vs.push_back(points[std::min(J - degree + i, static_cast<int>(points.size() - 1))]);

    // Recurse b-spline algorithm
    for (int p = 1; p <= degree; ++p) {
        // Renamed from i to it to not confuse formulas
        for (int it = degree; it >= p; it--) {
            int i = std::max(0, J - degree + it);
            float k_f = knot_sequence[i + degree - (p - 1)];
            float k_c = knot_sequence[i];
            float k_diff = k_f - k_c;

            vs[it] =
                ( (t - k_c) / k_diff ) * vs[it] +
                ( (k_f - t) / k_diff ) * vs[it-1];
        }
    }

    return vs[degree];
}

Curve DeBoorBSplineCurve::generate_curve_obj() {
    return template_generate_curve_obj<DeBoorBSplineCurve>();
}

void CurveDrawerFt_DeBoorBSplineCurve(flecs::entity e, GraphingWindow &window, GraphingWindow::GraphingContext &ctx) {
    // Regular drawing
    CurveDrawerFt(e, window, ctx);

    auto const* curve = e.try_get_mut<DeBoorBSplineCurve>();
    if (!curve || !curve->draw_shells)
        return;

    auto* dl = ImGui::GetWindowDrawList();

    auto& knot_sequence = curve->knot_sequence;
    auto& degree = curve->degree;
    auto& points = curve->points;

    // Adjust sample range to be between t_d and t_(N-d)
    float t = curve->shell_pos * (knot_sequence[knot_sequence.size() - degree - 1] - knot_sequence[degree]) + knot_sequence[degree];

    const int s = points.size() - 1;

    // Find the last value less than t
    int J = std::upper_bound(knot_sequence.begin(), knot_sequence.end(), t) - knot_sequence.begin() - 1;
    J = std::clamp(J, 0, static_cast<int>(knot_sequence.size() - 1));

    // If j has reached <= s, the result is out of bounds, so clip
    if (J > s)
        J = s;

    // Input the values from P_J to P_(J+d+1)
    std::vector<glm::vec4> vs; // Value list
    for (int i = 0; i <= degree; ++i)
        vs.push_back(points[std::min(J - degree + i, static_cast<int>(points.size() - 1))]);

    for (int i = 0; i < degree; ++i) {
        dl->AddLine(
            ctx.world_to_screen(vs[i]) + ctx.canvas_p0,
            ctx.world_to_screen(vs[i + 1]) + ctx.canvas_p0,
            IM_COL32(80, 80, 80, 128)
        );
    }

    // Recurse b-spline algorithm
    for (int p = 1; p <= degree; ++p) {
        // Renamed from i to it to not confuse formulas
        for (int it = degree; it >= p; it--) {
            int i = std::max(0, J - degree + it);
            float k_f = knot_sequence[i + degree - (p - 1)];
            float k_c = knot_sequence[i];
            float k_diff = k_f - k_c;

            vs[it] =
                ( (t - k_c) / k_diff ) * vs[it] +
                ( (k_f - t) / k_diff ) * vs[it-1];
        }

        for (int i = p; i < degree; ++i) {
            dl->AddLine(
                ctx.world_to_screen(vs[i]) + ctx.canvas_p0,
                ctx.world_to_screen(vs[i + 1]) + ctx.canvas_p0,
                IM_COL32(255, 0, 0, 128)
            );
        }
    }

    dl->AddCircleFilled(ctx.world_to_screen(vs[degree]) + ctx.canvas_p0, 5, IM_COL32(0, 255, 0, 128));
}
