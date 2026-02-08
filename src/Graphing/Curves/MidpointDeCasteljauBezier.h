//
// Created by willb on 2/7/2026.
//

#ifndef GRAPHING_MIDPOINTCASTELJAUBEZIER_H
#define GRAPHING_MIDPOINTCASTELJAUBEZIER_H
#include "BaseFunction.h"
#include "Graphing/Curve.h"


struct MidpointDeCasteljauBezier : BaseFunction {
    virtual ~MidpointDeCasteljauBezier();
    void generate(std::span<glm::vec4> given_samples) override;
    glm::vec4 sample(float t) const override;
    Curve generate_curve_obj() override;

    void regenerate();

    int subdivision_count = 1;
    std::vector<glm::vec2> given_samples;
    std::vector<glm::vec2> stored_samples;
};

void CurveDrawerFt_MidpointDeCasteljauBezier(flecs::entity e, GraphingWindow &window, GraphingWindow::GraphingContext &ctx);


#endif //GRAPHING_MIDPOINTCASTELJAUBEZIER_H