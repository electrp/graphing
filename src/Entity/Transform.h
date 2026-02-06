//
// Created by willb on 2/4/2026.
//

#ifndef GRAPHING_TRANSFORM_H
#define GRAPHING_TRANSFORM_H

#include "flecs.h"
#include "glm/glm.hpp"

struct Position : public glm::vec4 {
};

void SetupTransform(flecs::world w);


#endif //GRAPHING_TRANSFORM_H