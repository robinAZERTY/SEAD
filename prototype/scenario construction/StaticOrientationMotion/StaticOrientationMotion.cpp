/*
créé par : robinAZERTY
version du : 20/08/2022
*/

#include "StaticOrientationMotion.h"
StaticOrientationMotion::StaticOrientationMotion() : OrientationMotionBase()
{
    this->description = "default StaticOrientationMotion";
}

StaticOrientationMotion::StaticOrientationMotion(const OrientationState &OrientationState) : OrientationMotionBase()
{
    // verifier si les membres velocity et acceleration sont bien des vecteurs nuls
    if (OrientationState.dE_x != Vector(3) ||
        OrientationState.dE_y != Vector(3) ||
        OrientationState.dE_z != Vector(3))
        throw "velocity and acceleration vectors must be null";

    this->state = OrientationState;
    this->initialState = state;
    this->finalState = state;
    this->description = "StaticOrientationMotion with Orientation input";
}