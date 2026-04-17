//
// Created by willb on 2/25/2026.
//

#include "Project7.h"

#include <random>

#include "Graphing/Curves/DeBoorBeSplineCurve.h"
#include "Graphing/Curves/DeBoorBSplineFunction.h"
#include "Graphing/Curves/PolynomialInterpolation.h"


void Project7::Start(flecs::world w, GraphingWindow &window) {
    m_host = w.entity();

    m_curve = m_host.child()
        .add<DeBoorBSplineCurve>()
        .add<GraphingWindow::GraphingRelation>(window.m_host)
        .set<GraphingWindow::GraphingDrawer>(GraphingWindow::GraphingDrawer {CurveDrawerFt_DeBoorBSplineCurve});

    RemakePoints(window);
}

void Project7::Update(flecs::world w, GraphingWindow &window) {
    if (ImGui::InputInt("Point Count", &m_point_count)) {
        RemakePoints(window, true);
    }

    if (ImGui::InputInt("Degree", &degree)) {
        RemakePoints(window, true);
    }

    if (ImGui::Button("Randomize"))
        RandomizePoints(w, window);

    DeBoorBSplineCurve& spline = m_curve.get_mut<DeBoorBSplineCurve>();

    ImGui::Checkbox("Draw Shells", &spline.draw_shells);

    ImGui::SliderFloat("Shell T value", &spline.shell_pos, 0, 1);

    for (int i = 0; i < spline.knot_sequence.size(); ++i) {
        ImGui::PushID(i);
        ImGui::Text("Knot %d", i);
        float v = spline.knot_sequence[i];
        if (ImGui::InputFloat("k", &v)) {
            spline.knot_sequence[i] = v;
            RemakePoints(window);
        }
        ImGui::PopID();
    }
}

void Project7::Stop(flecs::world w, GraphingWindow &window) {
    m_host.destruct();
}

void Project7::RandomizePoints(flecs::world w, GraphingWindow &window) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> distrib_a(m_random_begin.x, m_random_end.x);
    std::uniform_real_distribution<> distrib_b(m_random_begin.y, m_random_end.y);

    for (int i = 0; i < m_points.size(); ++i) {
        m_points[i].set<Position>({glm::vec4{distrib_a(gen), distrib_b(gen), 0, i}});
    }
}

void Project7::RemakePoints(GraphingWindow &window, bool reset_knots) {
    DeBoorBSplineCurve& spline = m_curve.get_mut<DeBoorBSplineCurve>();

    degree = std::clamp(degree, 1, m_point_count - 1);
    spline.degree = degree;
    m_point_count = std::clamp(m_point_count, degree, 9999);

    if (reset_knots)
        spline.knot_sequence.clear();

    while (m_points.size() > m_point_count) {
        m_points.back().destruct();
        m_points.pop_back();
    }

    float start_point = (degree + 1.0f) / 2;
    while (m_points.size() < m_point_count) {
        m_points.push_back(m_host.child()
            .add<GraphingWindow::GraphingRelation>(window.m_host)
            .emplace<InputPoint>(InputPoint::ZPos | InputPoint::WPos)
            .set<Position>(Position{{start_point + m_points.size(), 1, 0, m_points.size()}})
            .add<CurveControlPointRel>(m_curve));
    }
}
