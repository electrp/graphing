//
// Created by willb on 2/4/2026.
//

#include "GraphingWindow.h"

#include "imgui.h"

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

    m_host.child()
        .emplace<Position>(glm::vec4{1, 1, 0, 1})
        .emplace<InputPoint>();
}

void GraphingWindow::draw(bool &open) {
    // Setup window
    ImGui::Begin("Graphing");
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));      // Disable padding
    ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(50, 50, 50, 255));  // Set a background color
    ImGui::BeginChild("canvas", ImVec2(0.0f, 0.0f), ImGuiChildFlags_Borders, ImGuiWindowFlags_NoMove);
    ImGui::PopStyleColor();
    ImGui::PopStyleVar();

    // Get sizes
    glm::vec2 canvas_p0 = ImGui::GetCursorScreenPos();
    glm::vec2 canvas_sz = ImGui::GetContentRegionAvail();
    glm::vec2 half_size = canvas_sz / 2.f;

    ImGuiIO& io = ImGui::GetIO();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    glm::vec2 min_extent = m_position - half_size;
    glm::vec2 max_extent = m_position + half_size;
    glm::vec2 button_size = glm::vec2(m_pointRadius, m_pointRadius);

    auto world_to_screen = [&](glm::vec2 pos) -> glm::vec2 {
        pos.y = -pos.y;
        return pos * m_zoom - half_size;
    };
    auto screen_to_world = [&](glm::vec2 pos) -> glm::vec2 {
        glm::vec2 out = (pos + half_size) / m_zoom;
        out.y = -out.y;
        return out;
    };

    if (m_currentlyDragging.is_alive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
        Position& pos = m_currentlyDragging.get_mut<Position>();
        InputPoint const& point = m_currentlyDragging.get<InputPoint>();

        glm::vec2 mouse_world = screen_to_world(glm::vec2(ImGui::GetMousePos()) - canvas_p0);

        if (!(point.constraints & InputPoint::Constraints::XPos))
            pos.x = mouse_world.x;

        if (!(point.constraints & InputPoint::Constraints::YPos))
            pos.y = mouse_world.y;
    } else {
        m_currentlyDragging = {};
    }

    m_pointQuery.each([&](flecs::entity e, Position& p, InputPoint& ip) {
        ImGui::PushID(e);
        glm::vec2 screen = world_to_screen(p);
        glm::vec2 begin_pos = screen - button_size;
        draw_list->AddCircleFilled(begin_pos, m_pointRadius, 0);
        ImGui::SetCursorPos(begin_pos);
        ImGui::InvisibleButton("Draggable", button_size);
        if (!m_currentlyDragging.is_alive() && ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
            m_currentlyDragging = e;
        }

        ImGui::PopID();
    });

    // Finish window
    ImGui::EndChild();
    ImGui::End();
};
