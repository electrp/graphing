//
// Created by willb on 2/25/2026.
//

#ifndef GRAPHING_PROJECT6_H
#define GRAPHING_PROJECT6_H
#include "ProjectBase.h"

struct Project7 : ProjectBase {
    void Start(flecs::world w, GraphingWindow &window) override;
    void Update(flecs::world w, GraphingWindow &window) override;
    void Stop(flecs::world w, GraphingWindow &window) override;
    void RandomizePoints(flecs::world w, GraphingWindow& window);
    void RemakePoints(GraphingWindow &window, bool reset_knots = false);

    flecs::entity m_curve;
    flecs::entity m_host;
    std::vector<flecs::entity> m_points;
    int m_point_count = 5;
    int degree = 3;
    glm::vec2 m_random_begin = {-2, -2};
    glm::vec2 m_random_end = {2, 2};
};



#endif //GRAPHING_PROJECT6_H
