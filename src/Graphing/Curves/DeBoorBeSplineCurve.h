//
// Created by willb on 4/13/2026.
//

#ifndef GRAPHING_DEBOORBESPLINECURVE_H
#define GRAPHING_DEBOORBESPLINECURVE_H
#include "BaseFunction.h"


class DeBoorBSplineCurve : BaseFunction{
public:
    virtual ~DeBoorBSplineCurve();
    void generate(std::span<glm::vec4> inputs) override;
    glm::vec4 sample(float t) override;
    Curve generate_curve_obj() override;
    void extra_draw();

    int degree = 3;
    bool draw_shells = true;
    float shell_pos = .5f;
    std::vector<glm::vec4> points;
    std::vector<float> knot_sequence;
};

void CurveDrawerFt_DeBoorBSplineCurve(flecs::entity e, GraphingWindow &window, GraphingWindow::GraphingContext &ctx);

#endif //GRAPHING_DEBOORBESPLINECURVE_H