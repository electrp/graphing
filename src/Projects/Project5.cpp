//
// Created by willb on 2/25/2026.
//

#include "Project5.h"

#include <random>

#include "Graphing/Curves/DeBoorBSpline.h"
#include "Graphing/Curves/PolynomialInterpolation.h"


void Project5::Start(flecs::world w, GraphingWindow &window) {
    m_host = w.entity();

    m_curve = m_host.child()
        .add<DeBoorBSpline>()
        .add<GraphingWindow::GraphingRelation>(window.m_host)
        .set<GraphingWindow::GraphingDrawer>(GraphingWindow::GraphingDrawer {CurveDrawerFx});

    RemakePoints(window);
}

void Project5::Update(flecs::world w, GraphingWindow &window) {
    if (ImGui::InputInt("Point Count", &m_point_count)) {
        RemakePoints(window);
    }

    if (ImGui::InputInt("Degree", &degree)) {
        RemakePoints(window);
    }

}

void Project5::Stop(flecs::world w, GraphingWindow &window) {
    m_host.destruct();
}


void Project5::RemakePoints(GraphingWindow &window) {
    degree = std::clamp(degree, 1, 9999);
    m_point_count = std::clamp(m_point_count, 2, 9999);

    DeBoorBSpline& spline = m_curve.get_mut<DeBoorBSpline>();
    spline.degree = degree;

    while (m_points.size() > 0) {
        m_points.back().destruct();
        m_points.pop_back();
    }

    float start_point = (degree + 1.0f) / 2;
    while (m_points.size() < m_point_count) {
        m_points.push_back(m_host.child()
            .add<GraphingWindow::GraphingRelation>(window.m_host)
            .emplace<InputPoint>(InputPoint::XPos | InputPoint::ZPos | InputPoint::WPos)
            .set<Position>(Position{{start_point + m_points.size(), 1, 0, m_points.size()}})
            .add<CurveControlPointRel>(m_curve));
    }
}
