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
    std::function<void(flecs::entity, std::span<float> inputs)> generate;
    std::function<float(flecs::entity, float)> sample;

    int param_index = 1; // Y is the input
};

struct CurveMarker {};
// Relates curves to their control points
struct CurveControlPointRel {};

void SetupCurve(flecs::world w);
void BasicCurveDrawer(flecs::entity e, GraphingWindow& window, GraphingWindow::GraphingContext& ctx);

#endif //GRAPHING_CURVE_H
