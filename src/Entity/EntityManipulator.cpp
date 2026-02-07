//
// Created by willb on 1/28/2026.
//

#include "EntityManipulator.h"

#include <string>

#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include "Transform.h"
#include "Graphing/GraphingWindow.h"

EntityManipulator::EntityManipulator(flecs::query<> query) : entity_query(query) {

}

void EntityManipulator::draw(bool *enabled, flecs::world w) {
    ImGui::Begin("Entities");

    std::string name;
    w.defer_begin();
    entity_query.each([&](flecs::entity e) {
        ImGui::PushID(e.id());
        ImGui::SeparatorText(e.name().c_str());
        if (ImGui::TreeNode("Edit")) {
            name = e.name().c_str();
            if (ImGui::InputText("Name", &name))
                e.set_name(name.c_str());

            if (ImGui::Button("Delete")) {
                e.destruct();
            }

            ImGui::TreePop();
        }

        if (auto* ptr = e.try_get_mut<Position>(); ptr != nullptr) {
            ImGui::InputFloat4("Position", &ptr->x);
        }

        if (auto* ptr = e.try_get_mut<GraphingWindow::GraphingDrawer>(); ptr != nullptr) {
            ImGui::Checkbox("Draw", &ptr->enabled);
        }

        if (auto* ptr = e.try_get_mut<InputPoint>(); ptr != nullptr) {
            if (ImGui::BeginCombo("Constraints", "...")) {
                auto toggle = [&](const char* name, int bit) {
                    bool is_set = (ptr->constraints & bit);
                    if (ImGui::Checkbox(name, &is_set)) {
                        if (is_set)
                            ptr->constraints |= bit;
                        else
                            ptr->constraints &= ~bit;
                    }
                };
                toggle("XPos", InputPoint::XPos);
                toggle("YPos", InputPoint::YPos);
                toggle("ZPos", InputPoint::ZPos);
                toggle("WPos", InputPoint::WPos);

                ImGui::EndCombo();
            }
        }

        ImGui::PopID();
    });
    w.defer_end();

    ImGui::End();
}
