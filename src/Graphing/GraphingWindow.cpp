//
// Created by willb on 2/4/2026.
//

#include "GraphingWindow.h"

#include "imgui.h"
#include <iostream>
#include <string>

#include "Curves/BBDeCasteljauBezier.h"
#include "Curves/BBDeCasteljauPolynomial.h"
#include "Curves/MidpointDeCasteljauBezier.h"
#include "Curves/NLIDeCasteljauBezier.h"
#include "Curves/NLIDeCasteljauPolynomial.h"

GraphingWindow::GraphingWindow(flecs::world w) {
    m_host = w.entity();

    m_pointQuery = w.query_builder<Position, InputPoint>()
        .with(w.component<GraphingRelation>(), m_host)
        .cached()
        .build();

    m_graphingObjects = w.query_builder<GraphingDrawer>()
        .with(w.component<GraphingRelation>(), m_host)
        .cached()
        .build();
}

ImGuiID GraphingWindow::Draw(bool &open) {
    auto w = m_host.world();

    // Setup window
    ImGui::Begin("Graphing");

    // if (ImGui::Button("New Entity")) {
    //     m_host.child<GraphingRelation>();
    // }
    //
    // ImGui::SameLine();
    // if (ImGui::Button("Serialize Test")) {
    //     std::vector<std::string> units;
    //     w.defer_begin();
    //     m_pointQuery.each([&](flecs::entity e, Position&, InputPoint&) {
    //         units.push_back(e.to_json().c_str());
    //         e.destruct();
    //     });
    //     w.defer_end();
    //     for (auto& unit : units) {
    //         w.entity().from_json(unit.c_str());
    //     }
    // }

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));      // Disable padding
    ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(50, 50, 50, 255));  // Set a background color
    ImGui::BeginChild("canvas", ImVec2(0.0f, 0.0f), ImGuiChildFlags_Borders, ImGuiWindowFlags_NoMove);
    ImGui::PopStyleColor();
    ImGui::PopStyleVar();

    m_zoom += ImGui::GetIO().MouseWheel * 5;

    // Get sizes
    glm::vec2 canvas_p0 = ImGui::GetCursorScreenPos();
    glm::vec2 canvas_sz = ImGui::GetContentRegionAvail();
    glm::vec2 canvas_p1 = canvas_p0 + canvas_sz;
    glm::vec2 half_size = canvas_sz / 2.f;

    ImGuiIO& io = ImGui::GetIO();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    glm::vec2 button_size = glm::vec2(m_pointRadius * 2, m_pointRadius * 2);

    std::function world_to_screen = [&](glm::vec2 pos) -> glm::vec2 {
        glm::vec2 out = (pos - m_position) * m_zoom + half_size;
        out.y = canvas_sz.y - out.y;
        return out;
    };

    std::function screen_to_world = [&](glm::vec2 pos) -> glm::vec2 {
        pos.y = canvas_sz.y - pos.y;
        glm::vec2 out = (pos - half_size) / m_zoom;
        return out + m_position;
    };

    glm::vec2 world_extent_screen_min = screen_to_world({0, 0});
    glm::vec2 world_extent_screen_max = screen_to_world(canvas_sz);

    glm::vec2 screen_origin = world_to_screen({0, 0});
    draw_list->AddLine(
        glm::vec2{screen_origin.x, 0} + canvas_p0,
        glm::vec2{screen_origin.x, canvas_sz.y} + canvas_p0,
        IM_COL32(0, 255, 0, 128));

    draw_list->AddLine(
        glm::vec2{0, screen_origin.y} + canvas_p0,
        glm::vec2{canvas_sz.x, screen_origin.y} + canvas_p0,
        IM_COL32(255, 0, 0, 128));

    m_pointQuery.each([&](flecs::entity e, Position& p, InputPoint& ip) {
        ImGui::PushID(e);
        glm::vec2 screen = world_to_screen(p);
        glm::vec2 begin_pos = screen - button_size / 2.0f;
        ImGui::SetCursorPos(begin_pos);
        ImGui::InvisibleButton("Draggable", button_size);
        draw_list->AddCircleFilled(
            screen + canvas_p0,
            m_pointRadius,
            ImGui::IsItemHovered() ? ip.color_hover : ip.color);
        if (!m_currentlyDragging.is_alive() && ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
            m_currentlyDragging = e;
        }

        ImGui::PopID();
    });

    // Special cases to handle drags off screen without triggering offscreen
    if (!m_currentlyDragging.is_alive()
        && ImGui::IsMouseClicked(0)
        && m_dragging_screen == false
        && ImGui::GetMousePos().x >= canvas_p0.x
        && ImGui::GetMousePos().y >= canvas_p0.y
        && ImGui::GetMousePos().x < canvas_p1.x
        && ImGui::GetMousePos().y < canvas_p1.y) {
        m_dragging_screen = true;
    }
    if (!ImGui::IsMouseDown(0)) {
        m_dragging_screen = false;
    }


    if (ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
        if (m_currentlyDragging.is_alive()) {
            Position& pos = m_currentlyDragging.get_mut<Position>();
            InputPoint const& point = m_currentlyDragging.get<InputPoint>();

            glm::vec2 mouse_world = screen_to_world(glm::vec2(ImGui::GetMousePos()) - canvas_p0);

            if (!(point.constraints & InputPoint::Constraints::XPos))
                pos.x = mouse_world.x;

            if (!(point.constraints & InputPoint::Constraints::YPos))
                pos.y = mouse_world.y;

            m_currentlyDragging.modified<Position>();
        } else if (m_dragging_screen) {
            glm::vec2 delta = ImGui::GetIO().MouseDelta;
            delta.y = -delta.y;
            m_position -= delta / m_zoom;
        }
    } else {
        m_currentlyDragging = {};
    }

    GraphingContext c {
        canvas_p0,
        canvas_sz,
        canvas_p1,
        world_to_screen,
        screen_to_world
    };

    m_graphingObjects.each([&](flecs::entity e, GraphingDrawer& drawer) {
        if (drawer.enabled)
            drawer.draw(e, *this, c);
    });

    // Finish window
    ImGui::EndChild();
    ImGui::End();

    return ImGui::GetID("Graphing");
}
