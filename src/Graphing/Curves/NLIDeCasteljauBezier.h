//
// Created by willb on 2/6/2026.
//

#ifndef GRAPHING_NLIDECASTELJAUBEZIER_H
#define GRAPHING_NLIDECASTELJAUBEZIER_H
#include "BaseFunction.h"
#include "BBDeCasteljauPolynomial.h"
#include "BBDeCasteljauPolynomial.h"


struct NLIDeCasteljauBezier : BaseFunction {
    virtual ~NLIDeCasteljauBezier();
    void generate(std::span<glm::vec4> inputs) override;
    glm::vec4 sample(float t) const override;
    Curve generate_curve_obj() override;

    bool draw_interp_info = true;
    float debug_render_slider_pos = .5f;

    std::vector<glm::vec4> coef;
};

void CurveDrawerFt_NLIDeCasteljauBezier(flecs::entity e, GraphingWindow &window, GraphingWindow::GraphingContext &ctx);

#endif //GRAPHING_NLIDECASTELJAUBEZIER_H