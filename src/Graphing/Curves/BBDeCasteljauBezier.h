//
// Created by willb on 2/7/2026.
//

#ifndef GRAPHING_BBDECASTELIJAUBEZIER_H
#define GRAPHING_BBDECASTELIJAUBEZIER_H
#include "BaseFunction.h"


struct BBDeCasteljauBezier : BaseFunction{
    virtual ~BBDeCasteljauBezier();
    void generate(std::span<glm::vec4> inputs) override;
    glm::vec4 sample(float t) const override;
    Curve generate_curve_obj() override;

    std::vector<glm::vec4> coef;
};


#endif //GRAPHING_BBDECASTELIJAUBEZIER_H