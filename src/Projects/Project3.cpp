//
// Created by willb on 2/25/2026.
//

#include "Project3.h"

#include <random>

#include "Graphing/Curves/PolynomialInterpolation.h"


void Project3::Start(flecs::world w, GraphingWindow &window) {
    m_host = w.entity();

    m_polynomial_curve = m_host.child()
        .add<PolynomialInterpolation>()
        .add<GraphingWindow::GraphingRelation>(window.m_host)
        .set<GraphingWindow::GraphingDrawer>(GraphingWindow::GraphingDrawer {CurveDrawerFtSamples<1000>, true});

    RemakePoints(window);
}

void Project3::Update(flecs::world w, GraphingWindow &window) {
    if (ImGui::InputInt("Point Count", &m_point_count)) {
        RemakePoints(window);
    }

    if (ImGui::Button("Print test"))
        m_polynomial_curve.get_mut<PolynomialInterpolation>().diff.print();

    if (ImGui::Button("Randomize"))
        RandomizePoints(w, window);
}

void Project3::Stop(flecs::world w, GraphingWindow &window) {
    m_host.destruct();
}

void Project3::RandomizePoints(flecs::world w, GraphingWindow &window) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> distrib_a(m_random_begin.x, m_random_end.x);
    std::uniform_real_distribution<> distrib_b(m_random_begin.y, m_random_end.y);

    for (int i = 0; i < m_points.size(); ++i) {
        m_points[i].set<Position>({glm::vec4{distrib_a(gen), distrib_b(gen), 0, i}});
    }
}

void Project3::RemakePoints(GraphingWindow &window) {
    while (m_points.size() < m_point_count) {
        m_points.push_back(m_host.child()
            .add<GraphingWindow::GraphingRelation>(window.m_host)
            .emplace<InputPoint>(InputPoint::ZPos | InputPoint::WPos)
            .set<Position>(Position{{m_points.size() / 3.0f, 1, 0, m_points.size()}})
            .add<CurveControlPointRel>(m_polynomial_curve));
    }

    while (m_points.size() > m_point_count) {
        m_points.back().destruct();
        m_points.pop_back();
    }
}
