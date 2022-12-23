/*
créé par : robinAZERTY
version du : 27/08/2022 - 0
*/

#pragma once

#if false // true when this header is a validated
#include "../OrientationMotionBase/OrientationMotionBase.cpp" //when this header is a valid
#else
#include "..\..\../lib_validated/scenario construction/OrientationMotionBase/OrientationMotionBase.cpp" //when this header is a prototype
#endif



class StaticOrientationMotion : public OrientationMotionBase
{
public:
    StaticOrientationMotion();
    StaticOrientationMotion(const OrientationState &OrientationState);
    StaticOrientationMotion(const Quaternion &q);
};