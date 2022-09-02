/*
créé par : robinAZERTY
version du : 27/08/2022 - 0
*/

#include "StaticOrientationMotion.h"
StaticOrientationMotion::StaticOrientationMotion() : OrientationMotionBase()
{
    this->description = "default StaticOrientationMotion";
}

StaticOrientationMotion::StaticOrientationMotion(const OrientationState &OrientationState) : OrientationMotionBase()
{
    // verifier si les membres velocity et acceleration sont bien nuls
    if (OrientationState.q_velocity.norm() != 0 || OrientationState.q_acceleration.norm() != 0)
        throw "velocity and acceleration vectors must be null";

    this->state = OrientationState;
    this->initialState = state;
    this->finalState = state;
    this->description = "StaticOrientationMotion with Orientation input";
}

StaticOrientationMotion::StaticOrientationMotion(const Quaternion &q) : OrientationMotionBase()
{
    state.q=q;
    initialState = state;
    finalState = state;
    this->description = "StaticOrientationMotion with Quaternion input";
}