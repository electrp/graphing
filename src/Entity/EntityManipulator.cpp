//
// Created by willb on 1/28/2026.
//

#include "EntityManipulator.h"

#include "imgui.h"
#include "Transform.h"
#include "Graphing/GraphingWindow.h"

EntityManipulator::EntityManipulator(flecs::query<> query) : entity_query(query) {

}

void EntityManipulator::draw(bool *enabled, flecs::world w) {
    ImGui::Begin("Entities");

    entity_query.each([&](flecs::entity e) {
        ImGui::PushID(e.id());
        ImGui::SeparatorText(e.name().c_str());
        if (auto* ptr = e.try_get_mut<Position>(); ptr != nullptr) {
            ImGui::InputFloat4("Position", &ptr->x);
        }

        if (auto* ptr = e.try_get_mut<GraphingWindow::GraphingDrawer>(); ptr != nullptr) {
            ImGui::Checkbox("Draw", &ptr->enabled);
        }
        ImGui::PopID();
    });

    ImGui::End();
}
