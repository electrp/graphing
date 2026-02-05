//
// Created by willb on 1/28/2026.
//

#ifndef GRAPHING_ENTITYMANIPULATOR_H
#define GRAPHING_ENTITYMANIPULATOR_H

#include <functional>

#include "flecs.h"

class EntityManipulator {
public:
    void draw(bool* enabled);

private:
    flecs::world world;

    struct ManipNode {
        flecs::entity target_entity;
        flecs::entity node_entity;
    };

    flecs::query<> entity_query;
    std::vector<ManipNode> nodes;
};

class ComponentDrawFn {
    std::function<void(void* obj, flecs::entity entity, flecs::entity draw_node)> setup;
    std::function<void(void* obj, flecs::entity entity, flecs::entity draw_node)> update;
    std::function<void(void* obj, flecs::entity entity, flecs::entity draw_node)> destroy;
};

#endif //GRAPHING_ENTITYMANIPULATOR_H