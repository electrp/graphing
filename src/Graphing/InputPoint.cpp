//
// Created by willb on 2/4/2026.
//

#include "InputPoint.h"

#include "flecs.h"

void SetupInputPoint(flecs::world w) {
    w.component<ImColor>()
        .member("x", &ImVec4::x)
        .member("y", &ImVec4::y)
        .member("z", &ImVec4::z)
        .member("w", &ImVec4::w);

    w.component<InputPoint>()
        .member("constraints", &InputPoint::constraints)
        .member("color", &InputPoint::color);
}
