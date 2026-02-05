//
// Created by willb on 2/4/2026.
//

#ifndef GRAPHING_WINDOW_H
#define GRAPHING_WINDOW_H
#include <functional>

#include "flecs.h"


class Window {
    std::function<void(flecs::entity e)> draw;
};


#endif //GRAPHING_WINDOW_H