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

StaticOrientationMotion::StaticOrientationMotion(const Quaternion &q) : OrientationMotionBase()
{
    Quaternion qx, qy, qz;
    qx.a = 0;
    qy.a = 0;
    qz.a = 0;

    qx.b = 1;
    qy.c = 1;
    qz.d = 1;

    Quaternion qE_x = q.rotate(qx);
    Quaternion qE_y = q.rotate(qy);
    Quaternion qE_z = q.rotate(qz);

    state.E_x.set(0, qE_x.b);
    state.E_x.set(1, qE_x.c);
    state.E_x.set(2, qE_x.d);

    state.E_y.set(0, qE_y.b);
    state.E_y.set(1, qE_y.c);
    state.E_y.set(2, qE_y.d);

    state.E_z.set(0, qE_z.b);
    state.E_z.set(1, qE_z.c);
    state.E_z.set(2, qE_z.d);

    initialState = state;
    finalState = state;
    this->description = "StaticOrientationMotion with Quaternion input";
}