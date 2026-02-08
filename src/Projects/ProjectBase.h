//
// Created by willb on 2/7/2026.
//

#ifndef GRAPHING_PROJECTBASE_H
#define GRAPHING_PROJECTBASE_H
#include "flecs.h"
#include "Graphing/GraphingWindow.h"


struct ProjectBase {
    virtual ~ProjectBase();
    virtual void Start(flecs::world w, GraphingWindow& windo) = 0;
    virtual void Update(flecs::world w, GraphingWindow& window) = 0;
    virtual void Stop(flecs::world w, GraphingWindow& window) = 0;
};


#endif //GRAPHING_PROJECTBASE_H