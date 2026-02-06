//
// Created by willb on 2/5/2026.
//

#include "Curve.h"

#include <iostream>

#include "Curves/BaseFunction.h"
#include "Curves/BBDeCasteljauPolynomial.h"
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

    w.component<CurveControlPointRel>().add(flecs::Symmetric);

    auto update_curve_control_fn = [](flecs::entity curve_e) {
        auto& curve = curve_e.get_mut<Curve>();
        std::vector<float> ctrl;
        curve_e.children(
            curve_e.world().component<CurveControlPointRel>(),
            [&](flecs::entity e) {
                auto const& pos = e.get<Position>();
                ctrl.push_back(pos[curve.param_index]);
            });
        curve.generate(curve_e, std::span<float>{ctrl});
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
}

void BasicCurveDrawer(flecs::entity e, GraphingWindow &window, GraphingWindow::GraphingContext& ctx) {
    for (int i = 0; i < ctx.canvas_sz.x; i += 2) {
        float x1 = ctx.screen_to_world({i, 0}).x;
        float y1 = e.get<Curve>().sample(e, x1);
        float x2 = ctx.screen_to_world({i + 1, 0}).x;
        float y2 = e.get<Curve>().sample(e, x2);

        glm::vec2 s1 = ctx.world_to_screen({x1, y1}) + ctx.canvas_p0;
        glm::vec2 s2 = ctx.world_to_screen({x2, y2}) + ctx.canvas_p0;

        auto* draw = ImGui::GetWindowDrawList();
        draw->AddLine(s1, s2, IM_COL32(155, 255, 255, 128));
    }
}
