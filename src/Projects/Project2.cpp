//
// Created by willb on 2/7/2026.
//

#include "Project2.h"

#include "Graphing/Curves/BBDeCasteljauBezier.h"
#include "Graphing/Curves/MidpointDeCasteljauBezier.h"
#include "Graphing/Curves/NLIDeCasteljauBezier.h"

void Project2::Start(flecs::world w, GraphingWindow &window) {
    m_host = w.entity();

    m_nli_curve = m_host.child()
        .add<NLIDeCasteljauBezier>()
        .add<GraphingWindow::GraphingRelation>(window.m_host)
        .set<GraphingWindow::GraphingDrawer>(GraphingWindow::GraphingDrawer {CurveDrawerFt_NLIDeCasteljauBezier, true});

    m_bb_curve = m_host.child()
        .add<BBDeCasteljauBezier>()
        .add<GraphingWindow::GraphingRelation>(window.m_host)
        .set<GraphingWindow::GraphingDrawer>(GraphingWindow::GraphingDrawer {CurveDrawerFt, false});

    m_midpoint_curve = m_host.child()
        .add<MidpointDeCasteljauBezier>()
        .add<GraphingWindow::GraphingRelation>(window.m_host)
        .set<GraphingWindow::GraphingDrawer>(GraphingWindow::GraphingDrawer {CurveDrawerFt_MidpointDeCasteljauBezier, false});

    RemakePoints(window);
}

void Project2::Update(flecs::world w, GraphingWindow &window) {
    if (ImGui::InputInt("Point Count", &m_point_count)) {
        RemakePoints(window);
    }

    ImGui::Checkbox("NLI Bezier", &m_nli_curve.get_mut<GraphingWindow::GraphingDrawer>().enabled);
    ImGui::Checkbox("BB Bezier", &m_bb_curve.get_mut<GraphingWindow::GraphingDrawer>().enabled);
    ImGui::Checkbox("Midpoint Bezier", &m_midpoint_curve.get_mut<GraphingWindow::GraphingDrawer>().enabled);

    ImGui::Separator();
    ImGui::Checkbox("NLI Draw Shells", &m_nli_curve.get_mut<NLIDeCasteljauBezier>().draw_interp_info);
    ImGui::SliderFloat("Shell t value", &m_nli_curve.get_mut<NLIDeCasteljauBezier>().debug_render_slider_pos, 0, 1);
    ImGui::InputInt("Midpoint Subdivisions", &m_midpoint_curve.get_mut<MidpointDeCasteljauBezier>().subdivision_count);
}

void Project2::Stop(flecs::world w, GraphingWindow &window) {
    m_host.destruct();
}

void Project2::RemakePoints(GraphingWindow& window) {
    while (m_points.size() < m_point_count) {
        m_points.push_back(m_host.child()
            .add<GraphingWindow::GraphingRelation>(window.m_host)
            .emplace<InputPoint>(InputPoint::ZPos | InputPoint::WPos)
            .set<Position>(Position{{m_points.size() / 3.0f, 1, 0, m_points.size()}})
            .add<CurveControlPointRel>(m_nli_curve)
            .add<CurveControlPointRel>(m_bb_curve)
            .add<CurveControlPointRel>(m_midpoint_curve));
    }

    while (m_points.size() > m_point_count) {
        m_points.back().destruct();
        m_points.pop_back();
    }

}

