/*
cree par : RobinAZERTY
version du 28/09/2022
*/

/*
documentation :
The gyroscope measures the angular velocity of the system in its referencial. (rad/s)
It's independent by translations.
This is a better way to measure the orientation of the system. But only during a short period of time.
To compute orientation, we need to integrate the angular velocity.
So error will accumulate.
*/

#pragma once


#if false // true if IS_VALIDATED | false if IS_PROTOTYPE
#include "../../..\validated\exchange data\state\State.cpp"

#else
#include "../..\exchange data\state\State.cpp"
#endif

class gyr2orient
{
public:
    gyr2orient();
    ~gyr2orient();

    void set_prev_quat(const Quaternion &prev_orientation);
    void set_prev_ypr(const Vector &prev_ypr);

    void compute(const Vector &gyrData, const double &dt);
    void compute(const Vector &prev_ypr, const Vector &gyrData, const double &dt);
    void compute(const Quaternion &prev_orientation, const Vector &gyrData, const double &dt);

    OrientationState get_OrientationState();
    Vector get_ypr();

private:
    
        void general_compute(const Vector &gyrData, const double &dt);
        Quaternion prev_quat;
        OrientationState orientationState;
        Vector ypr, prev_ypr;
};

