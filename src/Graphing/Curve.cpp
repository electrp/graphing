//
// Created by willb on 2/5/2026.
//

#include "Curve.h"

#include <iostream>

#include "Curves/BaseFunction.h"
#include "Curves/BBDeCasteljauBezier.h"
#include "Curves/BBDeCasteljauPolynomial.h"
#include "Curves/MidpointDeCasteljauBezier.h"
#include "Curves/NLIDeCasteljauBezier.h"
#include "Curves/NLIDeCasteljauPolynomial.h"
#include "Entity/Transform.h"

void SetupCurve(flecs::world w) {
    // Curve marked component
    w.observer<CurveMarker>()
        .event(flecs::OnAdd)
        .each([](flecs::entity component, CurveMarker) {
            // For each entity with a component marked CurveMarker
            component.world().observer()
                .with(component)
                .event(flecs::OnAdd)
                .each([=](flecs::entity e) {
                    if (e.has<Curve>()) {
                        e.remove<BaseFunction>();
                        return;
                    }
                    BaseFunction* bf = static_cast<BaseFunction*>(e.get_mut(component));
                    e.emplace<Curve>(bf->generate_curve_obj());
                });
            component.world().observer()
                .with(component)
                .event(flecs::OnRemove)
                .each([](flecs::entity e) {
                    e.remove<Curve>();
                });
        });

    w.component<CurveControlPointRel>()
        .add(flecs::Symmetric)
        .add(flecs::OrderedChildren)
        .add(flecs::Sparse);

    auto update_curve_control_fn = [](flecs::entity curve_e) {
        auto& curve = curve_e.get_mut<Curve>();
        std::vector<glm::vec4> ctrl;
        curve_e.children(
            curve_e.world().component<CurveControlPointRel>(),
            [&](flecs::entity e) {
                auto const& pos = e.get<Position>();
                ctrl.push_back(pos);
            });
        curve.generate(curve_e, std::span{ctrl});
    };

    // Update curves with attached points
    w.observer<>()
        .with<Position>()
        .with<CurveControlPointRel>("$wild")
        .with<Curve>().src("$wild")
        .event(flecs::OnAdd)
        .event(flecs::OnRemove)
        .event(flecs::OnSet)
        .each([&](flecs::entity e) {
            e.children<CurveControlPointRel>([=](flecs::entity curve_e) {
                if (curve_e.has<Curve>())
                    update_curve_control_fn(curve_e);
                else
                    std::cerr << "CurveControlPointRel was applied to something that it shouldn't have been" << std::endl;
            });
        });
    // It's symmetric counterpart
    w.observer<>()
        .with<Curve>()
        .with<CurveControlPointRel>("$wild")
        .with<Position>().src("$wild")
        .event(flecs::OnAdd)
        .event(flecs::OnRemove)
        .event(flecs::OnSet)
        .each(update_curve_control_fn);

    w.component<NLIDeCasteljauPolynomial>()
        .add<CurveMarker>();
    w.component<BBDeCasteljauPolynomial>()
        .add<CurveMarker>();
    w.component<NLIDeCasteljauBezier>()
        .add<CurveMarker>();
    w.component<BBDeCasteljauBezier>()
        .add<CurveMarker>();
    w.component<MidpointDeCasteljauBezier>()
        .add<CurveMarker>();
}

// For f(x)
void CurveDrawerFx(flecs::entity e, GraphingWindow &window, GraphingWindow::GraphingContext& ctx) {
    float last_x = ctx.screen_to_world({0, 0}).x;
    float last = e.get<Curve>().sample(e, last_x).x;
    for (int i = 1; i < ctx.canvas_sz.x; i += 2) {
        float x = ctx.screen_to_world({i, 0}).x;
        float y = e.get<Curve>().sample(e, x).x;

        glm::vec2 s1 = ctx.world_to_screen({last_x, last}) + ctx.canvas_p0;
        glm::vec2 s2 = ctx.world_to_screen({x, y}) + ctx.canvas_p0;

        auto* draw = ImGui::GetWindowDrawList();
        draw->AddLine(s1, s2, IM_COL32(155, 255, 255, 128));

        last_x = x;
        last = y;
    }
}

// For f(t)
// I liked this idea but it did not work well :(
// void CurveDrawerFt(flecs::entity e, GraphingWindow &window, GraphingWindow::GraphingContext &ctx) {
//     struct Pair {
//         float input;
//         glm::ivec2 output;
//     };
//
//     // in screen space
//     std::vector<Pair> point_stack;
//     Curve const& curve = e.get<Curve>();
//     auto* dl = ImGui::GetWindowDrawList();
//
//     float bottom = 0;
//     point_stack.push_back({1, ctx.world_to_screen(curve.sample(e, 1))});
//     while (point_stack.size() > 0) {
//         Pair last = point_stack.back();
//         float mid = (last.input + bottom) / 2;
//         glm::ivec2 next = ctx.world_to_screen(curve.sample(e, mid));
//         if (next == last.output) {
//             bottom = last.input;
//             point_stack.pop_back();
//             dl->AddRectFilled
//                 (glm::vec2{next} + ctx.canvas_p0, glm::vec2{next} + ctx.canvas_p0 + glm::vec2{1, 1}, ImColor(155, 255, 255, 128));
//         } else {
//             point_stack.push_back(Pair{mid, next});
//         }
//     }
// }

void CurveDrawerFt(flecs::entity e, GraphingWindow &window, GraphingWindow::GraphingContext &ctx) {
    CurveDrawerFtSamples<200>(e, window, ctx);
}

