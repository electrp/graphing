//
// Created by willb on 2/25/2026.
//

#ifndef GRAPHING_PROJECT5_H
#define GRAPHING_PROJECT5_H
#include "ProjectBase.h"

struct Project5 : ProjectBase {
    void Start(flecs::world w, GraphingWindow &window) override;
    void Update(flecs::world w, GraphingWindow &window) override;
    void Stop(flecs::world w, GraphingWindow &window) override;

    void RemakePoints(GraphingWindow &window);

    flecs::entity m_curve;
    flecs::entity m_host;
    std::vector<flecs::entity> m_points;
    int m_point_count = 5;
    int degree = 3;
};



#endif //GRAPHING_PROJECT5_H
