//
// Created by willb on 2/6/2026.
//

#include "NLIDeCasteljauBezier.h"

#include "BBDeCasteljauPolynomial.h"
#include <algorithm>

NLIDeCasteljauBezier::~NLIDeCasteljauBezier() {
}

void NLIDeCasteljauBezier::generate(std::span<glm::vec4> inputs) {
    coef = {inputs.begin(), inputs.end()};
    // w is the order
    std::sort(coef.begin(), coef.end(), [&](glm::vec4 a, glm::vec4 b) {
        return a.w < b.w;
    });
}

glm::vec4 NLIDeCasteljauBezier::sample(float t) const {
    if (coef.size() == 0)
        return glm::vec4{0};
    if (coef.size() == 1)
        return glm::vec4{coef[0].x, coef[0].y, 0, 0};

    std::vector<glm::vec2> main;
    for (auto& val : coef) main.push_back({val.x, val.y});
    std::vector<glm::vec2> sub;
    float inv = 1 - t;
    while (true) {
        for (int i = 0; i < main.size() - 1; ++i) {
            sub.push_back(inv * main[i] + t * main[i + 1]);
        }
        std::swap(main, sub);
        if (main.size() == 1)
            return glm::vec4{main[0].x, main[0].y, 0, 0};
        sub.clear();
    }
}

Curve NLIDeCasteljauBezier::generate_curve_obj() {
    return template_generate_curve_obj<NLIDeCasteljauBezier>();
}

void CurveDrawerFt_NLIDeCasteljauBezier(flecs::entity e, GraphingWindow &window, GraphingWindow::GraphingContext &ctx) {
    // Regular drawing
    CurveDrawerFt(e, window, ctx);

    auto const* curve = e.try_get_mut<NLIDeCasteljauBezier>();
    if (!curve || !curve->draw_interp_info)
        return;

    auto* dl = ImGui::GetWindowDrawList();

    std::vector<glm::vec2> main;
    for (auto& val : curve->coef) main.push_back({val.x, val.y});
    std::vector<glm::vec2> sub;
    float t = curve->debug_render_slider_pos;
    float inv = 1 - t;
    while (true) {
        for (int i = 0; i < main.size() - 1; ++i) {
            sub.push_back(inv * main[i] + t * main[i + 1]);
            dl->AddLine(ctx.world_to_screen(main[i]) + ctx.canvas_p0, ctx.world_to_screen(main[i + 1]) + ctx.canvas_p0, IM_COL32(255, 0, 0, 128));
        }
        std::swap(main, sub);
        if (main.size() == 1) {
            dl->AddCircleFilled(ctx.world_to_screen(main[0]) + ctx.canvas_p0, 5, IM_COL32(255, 0, 0, 128));
            break;
        }
        sub.clear();
    }
}
