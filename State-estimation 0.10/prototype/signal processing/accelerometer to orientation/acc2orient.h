/*
cree par : RobinAZERTY
version du 29/09/2022
*/

/*
documentation :
assuming the system is in a static position, the accelerometer will measure the gravity vector in its referencial.
the gravity vector is the vector that points towards the center of the earth.
We want the orientation of the system.
The orientation is the rotation of the system in the referencial of the earth. (quaternion or euler angles)
*/

#pragma once

#if !true// true if IS_VALIDATED | false if IS_PROTOTYPE
#include "../../..\common\exchange data\state\State.cpp"
#else
#include "..\..\..\../common/exchange data/state/State.cpp"
#endif

class acc2orient
{
public:
    acc2orient();
    ~acc2orient();

    void set_prev_quat(const Quaternion &prev_orientation);
    void set_prev_ypr(const Vector &prev_ypr);

    void compute(const Vector &accData, const double &dt);
    void compute(const Vector &prev_ypr, const Vector &accData, const double &dt);
    void compute(const Quaternion &prev_orientation, const Vector &accData, const double &dt);

    OrientationState get_OrientationState();
    Vector get_ypr();

private:

    void general_compute(const Vector &accData);
    Quaternion prev_quat;
    OrientationState orientationState;
    Vector ypr, prev_ypr;
};