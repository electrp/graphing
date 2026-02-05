//
// Created by willb on 2/4/2026.
//

#ifndef GRAPHING_INPUTPOINT_H
#define GRAPHING_INPUTPOINT_H



struct InputPoint {
    enum Constraints {
        XPos = (1 << 0),
        YPos = (1 << 1),
        ZPos = (1 << 2),
        WPos = (1 << 3)
    };
    typedef int Constraint_t;

    Constraint_t constraints = XPos | ZPos | WPos;
};


#endif //GRAPHING_INPUTPOINT_H