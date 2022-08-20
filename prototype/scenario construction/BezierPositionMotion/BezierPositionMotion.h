/*
créé par : robinAZERTY
version du : 20/08/2022 - 0
*/

#pragma once

#include "..\..\../lib_validated/scenario construction/PositionMotionBase/PositionMotionBase.cpp" //when this header is a prototype
//#include "../PositionMotionBase/PositionMotionBase.cpp" //when this header is a valid

class BezierPositionMotion : public PositionMotionBase
{
public:
    BezierPositionMotion();
    BezierPositionMotion(const Vector Points_3D[4], const double &duration);
    BezierPositionMotion(const PositionState &initialState, const Vector Points_3D[2], const double &duration);
    BezierPositionMotion(const PositionState &initialState, const PositionState &finalState, const double &duration);
private:
    Vector A = Vector(3),B=1,C=A,D=A;
    virtual void update_state(const double &t);
    virtual void init();
    double alpha;
};  