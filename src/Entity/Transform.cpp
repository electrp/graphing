//
// Created by willb on 2/4/2026.
//

#include "Transform.h"

void SetupTransform(flecs::world w) {
    w.component<Position>()
        .member("x", &Position::x)
        .member("y", &Position::y)
        .member("z", &Position::z)
        .member("w", &Position::w);
}
