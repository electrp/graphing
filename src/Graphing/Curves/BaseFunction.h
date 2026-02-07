//
// Created by willb on 2/5/2026.
//

#ifndef GRAPHING_BASEFUNCTION_H
#define GRAPHING_BASEFUNCTION_H

#include <span>

#include "Graphing/Curve.h"

struct BaseFunction {
    virtual ~BaseFunction();
    virtual void generate(std::span<glm::vec4> inputs) = 0;
    virtual glm::vec4 sample(float t) const = 0;
    virtual Curve generate_curve_obj() = 0;


    template<typename T>
    static Curve template_generate_curve_obj();
};

template<typename T>
Curve BaseFunction::template_generate_curve_obj() {
    return Curve {
        [](flecs::entity e, std::span<glm::vec4> inputs) {
            e.get_mut<T>().generate(inputs);
        },
        [](flecs::entity e, float t) -> glm::vec4 {
            return e.get<T>().sample(t);
        }
    };
}


#endif //GRAPHING_BASEFUNCTION_H
