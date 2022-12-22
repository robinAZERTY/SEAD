/*
créé par : robinAZERTY
version du : 20/08/2022
*/

#pragma once

#include "..\..\../lib_validated/scenario construction/PositionMotionBase/PositionMotionBase.cpp" //when this header is a prototype
//#include "../PositionMotionBase/PositionMotionBase.cpp" //when this header is a valid

class StaticPositionMotion : public PositionMotionBase
{
public:
    StaticPositionMotion();
    StaticPositionMotion(const PositionState &positionState);
};