/*
cree par : RobinAZERTY
version du 28/09/2022
*/

#pragma once

#include "gyr2orient.h"

gyr2orient::gyr2orient()
{
    this->orientationState = OrientationState();
    this->ypr = Vector(3);
    this->prev_ypr = ypr;
}

gyr2orient::~gyr2orient()
{
}

OrientationState gyr2orient::get_OrientationState()
{
    return this->orientationState;
}

Vector gyr2orient::get_ypr()
{
    return ypr;
}

void gyr2orient::set_prev_quat(const Quaternion &prev_quat)
{
    this->prev_orientationState.q = prev_quat;
    this->prev_ypr = prev_quat.yaw_pitch_roll();
}

void gyr2orient::set_prev_ypr(const Vector &prev_ypr)
{
    this->prev_ypr = prev_ypr;
    this->prev_orientationState.q = Quaternion(prev_ypr(0), prev_ypr(1), prev_ypr(2));
}

void gyr2orient::compute(const Vector &gyrData, const double &dt, const unsigned short &mode)
{
    this->compute(this->prev_orientationState.q, gyrData, dt,mode);
}

void gyr2orient::compute(const Vector &prev_ypr, const Vector &gyrData, const double &dt, const unsigned short &mode)
{
    this->prev_orientationState.q = Quaternion(prev_ypr(0), prev_ypr(1), prev_ypr(2));
    if (mode == 0)
        general_compute(gyrData,dt);
    else if (mode == 1)
        general_compute2(gyrData,dt);

    this->prev_orientationState = this->orientationState;
    //convert quaternion to ypr
    this->prev_ypr = this->prev_orientationState.q.yaw_pitch_roll();

}

void gyr2orient::compute(const Quaternion &prev_quat, const Vector &gyrData, const double &dt, const unsigned short &mode)
{   
    this->prev_orientationState.q = prev_quat;
    if (mode == 0)
        general_compute(gyrData,dt);
    else if (mode == 1)
        general_compute2(gyrData,dt);

    this->prev_orientationState = this->orientationState;
    //convert quaternion to ypr
    this->prev_ypr = this->prev_orientationState.q.yaw_pitch_roll();
}

void gyr2orient::general_compute(const Vector &gyrData, const double &dt) //juste compute the new orientation from gyro data
{
    //integration
    //https://lucidar.me/fr/quaternions/quaternion-and-gyroscope/
    Quaternion q_gyr;
    q_gyr.a=0;
    q_gyr.b=gyrData(0);
    q_gyr.c=gyrData(1);
    q_gyr.d=gyrData(2);

    this->orientationState.q_velocity = this->prev_orientationState.q * q_gyr * 0.5;
    this->orientationState.q = this->prev_orientationState.q + this->orientationState.q_velocity*dt;
    this->orientationState.q.normalize(); 
    this->ypr = this->orientationState.q.yaw_pitch_roll();
}

void gyr2orient::general_compute2(const Vector &gyrData, const double &dt) //trapzoidal integration
{
    //integration
    //https://lucidar.me/fr/quaternions/quaternion-and-gyroscope/
    Quaternion q_gyr;
    q_gyr.a=0;
    q_gyr.b=gyrData(0);
    q_gyr.c=gyrData(1);
    q_gyr.d=gyrData(2);

    this->orientationState.q_velocity = this->prev_orientationState.q * q_gyr * 0.5;
    this->orientationState.q = this->prev_orientationState.q + (this->orientationState.q_velocity + this->prev_orientationState.q_velocity)*dt/2;
    this->orientationState.q=this->orientationState.q.normalize(); 
    this->ypr = this->orientationState.q.yaw_pitch_roll();
}
