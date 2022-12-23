/*
créé par : robinAZERTY
version du : 20/08/2022
*/

#pragma once
#if false // true when this header is a validated
#include "../PositionMotionBase/PositionMotionBase.cpp" //when this header is a valid
#else
#include "..\..\../lib_validated/scenario construction/PositionMotionBase/PositionMotionBase.cpp" //when this header is a prototype
#endif

class StaticPositionMotion : public PositionMotionBase
{
public:
    StaticPositionMotion();
    StaticPositionMotion(const PositionState &positionState);
};