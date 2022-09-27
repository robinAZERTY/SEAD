/*
cree par : RobinAZERTY
version du 27/09/2022
*/

/*
documentation :
assuming that the system is in a static position, the accelerometer will measure the gravity vector in its referencial.
the gravity vector is the vector that points towards the center of the earth.
We want the orientation of the system.
The orientation is the rotation of the system in the referencial of the earth. (quaternion or euler angles)
*/

#pragma once

#define IS_VALIDATED 0

#if IS_VALIDATED
#include "../../..\validated\exchange data\state\State.cpp"
#else
#include "../..\exchange data\state\State.cpp"
#endif

class acc2orient
{
public:
    acc2orient();
    ~acc2orient();

    OrientationState get_state();
    Vector get_ypr();
    void compute_state(const Vector &accData, const double &dt);

private:
    Quaternion prev_orientation;
    OrientationState orientationState;
    Vector ypr,prev_ypr;  
};