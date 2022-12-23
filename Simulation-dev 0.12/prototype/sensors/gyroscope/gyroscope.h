/*
cree par : RobinAZERTY
version du 11/09/2022
*/

#pragma once

#include "../sensor/sensor.cpp"


class gyroscope : public sensor
{
    public:
        gyroscope(){description="gyro";};
        gyroscope(Vector position, Quaternion orientation);
        const Vector measure(const State &robot_state, const double &time);
};