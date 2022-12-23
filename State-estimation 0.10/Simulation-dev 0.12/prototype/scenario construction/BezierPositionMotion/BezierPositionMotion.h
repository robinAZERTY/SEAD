/*
créé par : robinAZERTY
version du : 20/08/2022 - 0
*/

#pragma once
#if false // true when this header is a validated
#include "../PositionMotionBase/PositionMotionBase.cpp" //when this header is a valid
#else
#include "..\..\../lib_validated/scenario construction/PositionMotionBase/PositionMotionBase.cpp" //when this header is a prototype
#endif

class BezierPositionMotion : public PositionMotionBase
{
public:
    BezierPositionMotion();
    BezierPositionMotion(const Vector Points_3D[4], const double &duration);
    BezierPositionMotion(const PositionState &initialState, const Vector Points_3D[2], const double &duration);
    BezierPositionMotion(const PositionState &initialState, const PositionState &finalState, const double &duration);
    virtual PositionState get_state(const double &t);
    virtual PositionState get_initial_state();
    virtual PositionState get_final_state();
    
private:
    Vector A = Vector(3),B=1,C=A,D=A;
    virtual void update_state(const double &t);
    virtual void init();
    double alpha;
};  