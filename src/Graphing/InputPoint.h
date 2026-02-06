//
// Created by willb on 2/4/2026.
//

#ifndef GRAPHING_INPUTPOINT_H
#define GRAPHING_INPUTPOINT_H
#include "flecs.h"
#include "imgui.h"


struct InputPoint {
    enum Constraints {
        XPos = (1 << 0),
        YPos = (1 << 1),
        ZPos = (1 << 2),
        WPos = (1 << 3)
    };
    typedef int Constraint_t;

    Constraint_t constraints = XPos | ZPos | WPos;
    ImColor color = ImColor(255, 255, 255, 64);
};
void SetupInputPoint(flecs::world w);


#endif //GRAPHING_INPUTPOINT_H