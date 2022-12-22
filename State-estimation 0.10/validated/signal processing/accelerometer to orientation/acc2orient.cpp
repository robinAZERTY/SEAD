/*
cree par : RobinAZERTY
version du 29/09/2022
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

OrientationState acc2orient::get_OrientationState()
{
    return this->orientationState;
}

Vector acc2orient::get_ypr()
{
    return ypr;
}

void acc2orient::set_prev_quat(const Quaternion &prev_quat)
{
    this->prev_quat = prev_quat;
    this->prev_ypr = prev_quat.yaw_pitch_roll();
}

void acc2orient::set_prev_ypr(const Vector &prev_ypr)
{
    this->prev_ypr = prev_ypr;
    this->prev_quat = Quaternion(prev_ypr(0), prev_ypr(1), prev_ypr(2));
}

void acc2orient::compute(const Vector &accData, const double &dt)
{
    this->compute(this->prev_quat, accData, dt);
}

void acc2orient::compute(const Vector &prev_ypr, const Vector &accData, const double &dt)
{
    general_compute(accData);
    orientationState.q_velocity = (this->orientationState.q - Quaternion(prev_ypr(0), prev_ypr(1), prev_ypr(2))) / dt;
    this->prev_quat = this->orientationState.q;
}

void acc2orient::compute(const Quaternion &prev_quat, const Vector &accData, const double &dt)
{
    this->general_compute(accData);
    //compute angular velocity
    this->orientationState.q_velocity = (this->orientationState.q - prev_quat) / dt;
    this->prev_quat = this->orientationState.q;
    //convert quaternion to ypr
    this->prev_ypr = this->prev_quat.yaw_pitch_roll();
}
void acc2orient::general_compute(const Vector &accData)//juste compute angle with acc data
{
    const double pitch = atan2(-accData(0), sqrt(accData(2) * accData(2) + accData(1) * accData(1)));
    const double roll = atan2(-accData(1), -accData(2));

    this->ypr.set(0, 0);
    this->ypr.set(1, pitch);
    this->ypr.set(2, roll);

    this->orientationState.q = Quaternion(ypr(0), ypr(1), ypr(2));//convert ypr to quaternion
    
    //change its sign if necessary (make sure the quaternion is the closest to the previous one)

    const double sign = this->orientationState.q.a+this->orientationState.q.b+this->orientationState.q.c+this->orientationState.q.d;
    if (sign< 0)
    {   
        this->orientationState.q = this->orientationState.q*-1;
    }
}