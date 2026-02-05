//
// Created by willb on 2/4/2026.
//

#ifndef GRAPHING_GRAPHINGWINDOW_H
#define GRAPHING_GRAPHINGWINDOW_H
#include "flecs.h"
#include "InputPoint.h"
#include "glm/vec2.hpp"
#include "Entity/Transform.h"


struct GraphingWindow {
    // Relates graph to things it displays
    class GraphingRelation {};
    class GraphingDrawer {
        std::function<void(flecs::entity e, GraphingWindow& state)> draw;
    };

    explicit GraphingWindow(flecs::world w);
    void draw(bool& open);

    // Members
    flecs::entity m_host;
    flecs::entity m_currentlyDragging;
    float m_zoom = 50; // Pixels per unit
    float m_pointRadius = 10;
    glm::vec2 m_position = {0, 0};
    flecs::query<Position, InputPoint> m_pointQuery;
    flecs::query<GraphingDrawer> m_graphingObjects;
};


#endif //GRAPHING_GRAPHINGWINDOW_H