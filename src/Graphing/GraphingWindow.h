//
// Created by willb on 2/4/2026.
//

#ifndef GRAPHING_GRAPHINGWINDOW_H
#define GRAPHING_GRAPHINGWINDOW_H
#include <string>

#include "flecs.h"
#include "InputPoint.h"
#include "glm/vec2.hpp"
#include "Entity/Transform.h"


struct GraphingWindow {
    // Relates graph to things it displays
    class GraphingRelation {};
    struct GraphingContext {
        glm::vec2 canvas_p0;
        glm::vec2 canvas_sz;
        glm::vec2 canvas_p1;
        std::function<glm::vec2(glm::vec2 pos)>& world_to_screen;
        std::function<glm::vec2(glm::vec2 pos)>& screen_to_world;
    };
    struct GraphingDrawer {
        std::function<void(flecs::entity e, GraphingWindow& window, GraphingContext& ctx)> draw;

        bool enabled = true;
    };

    explicit GraphingWindow(flecs::world w);
    ImGuiID Draw(bool& open);

    // Members
    bool m_dragging_screen = false;
    flecs::entity m_host;
    flecs::entity m_currentlyDragging;
    float m_zoom = 200; // Pixels per unit
    float m_pointRadius = 10;
    glm::vec2 m_position = {.5f, .5f};
    flecs::query<Position, InputPoint> m_pointQuery;
    flecs::query<GraphingDrawer> m_graphingObjects;
};


#endif //GRAPHING_GRAPHINGWINDOW_H