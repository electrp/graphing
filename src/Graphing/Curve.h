//
// Created by willb on 2/5/2026.
//

#ifndef GRAPHING_CURVE_H
#define GRAPHING_CURVE_H
#include <functional>
#include <span>

#include "flecs.h"
#include "GraphingWindow.h"

struct Curve {
    std::function<void(flecs::entity, std::span<glm::vec4> inputs)> generate;
    std::function<glm::vec4(flecs::entity, float)> sample;
};

struct CurveMarker {};
// Relates curves to their control points
struct CurveControlPointRel {};

void SetupCurve(flecs::world w);
void CurveDrawerFx(flecs::entity e, GraphingWindow& window, GraphingWindow::GraphingContext& ctx);
void CurveDrawerFt(flecs::entity e, GraphingWindow& window, GraphingWindow::GraphingContext& ctx);

template<int SAMPLES>
void CurveDrawerFtSamples(flecs::entity e, GraphingWindow& window, GraphingWindow::GraphingContext& ctx) {
    Curve const& curve = e.get<Curve>();
    auto* dl = ImGui::GetWindowDrawList();
    float step = 1.0f / SAMPLES;

    glm::vec2 last = curve.sample(e, 0);
    for (float it = step; it < 1; it += step) {
        glm::vec2 curr = curve.sample(e, it);
        dl->AddLine
            (ctx.world_to_screen(last) + ctx.canvas_p0, ctx.world_to_screen(curr) + ctx.canvas_p0, ImColor(155, 255, 255, 128));
        last = curr;
    }
}

#endif //GRAPHING_CURVE_H
