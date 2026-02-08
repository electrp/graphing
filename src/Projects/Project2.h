//
// Created by willb on 2/7/2026.
//

#ifndef GRAPHING_PROJECT2_H
#define GRAPHING_PROJECT2_H
#include "ProjectBase.h"


struct Project2 : ProjectBase {
    void Start(flecs::world w, GraphingWindow &window) override;
    void Update(flecs::world w, GraphingWindow &window) override;
    void Stop(flecs::world w, GraphingWindow &window) override;

    void RemakePoints(GraphingWindow &window);


    flecs::entity m_nli_curve;
    flecs::entity m_bb_curve;
    flecs::entity m_midpoint_curve;
    flecs::entity m_host;
    std::vector<flecs::entity> m_points;

    int m_point_count = 5;
};


#endif //GRAPHING_PROJECT2_H