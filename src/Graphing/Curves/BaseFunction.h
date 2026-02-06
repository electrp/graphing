//
// Created by willb on 2/5/2026.
//

#ifndef GRAPHING_BASEFUNCTION_H
#define GRAPHING_BASEFUNCTION_H

#include <span>

#include "Graphing/Curve.h"

struct BaseFunction {
    virtual void generate(std::span<float> inputs) = 0;
    virtual float sample(float t) const = 0;
    virtual Curve generate_curve_obj() = 0;


    template<typename T>
    static Curve template_generate_curve_obj();
};

template<typename T>
Curve BaseFunction::template_generate_curve_obj() {
    return Curve {
        [](flecs::entity e, std::span<float> inputs) {
            e.get_mut<T>().generate(inputs);
        },
        [](flecs::entity e, float t) -> float {
            return e.get<T>().sample(t);
        }
    };
}


#endif //GRAPHING_BASEFUNCTION_H
