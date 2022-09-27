/*
cree par : RobinAZERTY
version du 27/09/2022
*/


#pragma once

#include "acc2orient.h"

acc2orient::acc2orient()
{
    this->orientationState = OrientationState();
    this->ypr = Vector(3);
    this->prev_ypr = ypr;
}

acc2orient::~acc2orient()
{
}

OrientationState acc2orient::get_state()
{
    return this->orientationState;
}

Vector acc2orient::get_ypr()
{
    return ypr;
}

void acc2orient::compute_state(const Vector &accData, const double &dt)
{
    const double pitch = atan2(-accData(2), sqrt(accData(0) * accData(0) + accData(1) * accData(1)));
    const double roll = atan2(accData(1), accData(0));

    ypr.set(0, 0);
    ypr.set(1, pitch);
    ypr.set(2, roll);

    orientationState.q = Quaternion(ypr(0), ypr(1), ypr(2));
    orientationState.q_velocity = (orientationState.q-Quaternion(prev_ypr(0), prev_ypr(1), prev_ypr(2))) / dt;
    prev_orientation=orientationState.q;
}