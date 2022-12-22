/*
cree par : RobinAZERTY
version du 22/12/2022
*/
#include "mag2orient.h"


mag2orient::mag2orient()
{
}

mag2orient::~mag2orient()
{
}

void mag2orient::set_prev_quat(const Quaternion &prev_orientation)
{
    prev_quat = prev_orientation;
}

void mag2orient::set_prev_ypr(const Vector &prev_ypr)
{
    this->prev_ypr = prev_ypr;
}

void  mag2orient::compute(const Quaternion &current_orientation, const Vector &magData, const double &dt)
{
    // projection of the magnetic field vector on the horizontal plane knowing the rool and pitch angle
    // buid the quaternion containing the magnetic field vector
    Quaternion q_mag = Quaternion(0, magData(0), magData(1), magData(2));
    // rotate the magnetic field vector to the current frame
    q_mag = current_orientation * q_mag * current_orientation.conjugate();
    // get the yaw angle difference and buid the quaternion that rotate by yaw_diff
    Quaternion q_yaw_diff = Quaternion(atan2(q_mag.c, q_mag.b), 0, 0);
    // rotate the current orientation by the yaw difference
    this->orientationState.q = current_orientation * q_yaw_diff;
}
