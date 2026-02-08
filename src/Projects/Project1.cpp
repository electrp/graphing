//
// Created by willb on 2/7/2026.
//

#include "Project1.h"

#include "Graphing/Curves/BBDeCasteljauPolynomial.h"
#include "Graphing/Curves/NLIDeCasteljauPolynomial.h"

void Project1::Start(flecs::world w, GraphingWindow& window) {
    m_host = w.entity();

    m_nli_curve = m_host.child()
        .add<NLIDeCasteljauPolynomial>()
        .add<GraphingWindow::GraphingRelation>(window.m_host)
        .set<GraphingWindow::GraphingDrawer>(GraphingWindow::GraphingDrawer {CurveDrawerFx, true});

    m_bb_curve = m_host.child()
        .add<BBDeCasteljauPolynomial>()
        .add<GraphingWindow::GraphingRelation>(window.m_host)
        .set<GraphingWindow::GraphingDrawer>(GraphingWindow::GraphingDrawer {CurveDrawerFx, false});

    RemakePoints(window);
}

void Project1::Update(flecs::world w, GraphingWindow &window) {
    if (ImGui::InputInt("Point Count", &m_point_count)) {
        RemakePoints(window);
    }
    ImGui::Checkbox("NLI Curve", &m_nli_curve.get_mut<GraphingWindow::GraphingDrawer>().enabled);
    ImGui::Checkbox("BB Curve", &m_bb_curve.get_mut<GraphingWindow::GraphingDrawer>().enabled);
}

void Project1::Stop(flecs::world w, GraphingWindow &window) {
    m_host.destruct();
}

void Project1::RemakePoints(GraphingWindow& window) {
    while (m_points.size() < m_point_count) {
        m_points.push_back(m_host.child()
            .add<GraphingWindow::GraphingRelation>(window.m_host)
            .emplace<InputPoint>(InputPoint::XPos | InputPoint::ZPos | InputPoint::WPos)
            .set<Position>(Position{{0, 1, 0, 0}})
            .add<CurveControlPointRel>(m_nli_curve)
            .add<CurveControlPointRel>(m_bb_curve));
    }

    while (m_points.size() > m_point_count) {
        m_points.back().destruct();
        m_points.pop_back();
    }

    if (m_point_count < 2)
        return;

    float separation = 1.0f / (m_point_count - 1);
    for (int i = 0; i < m_point_count; ++i) {
        glm::vec4 p = m_points[i].get<Position>();
        m_points[i].set<Position>( Position{{
            separation * i, p.y, 0, 0
        }});
    }
}


