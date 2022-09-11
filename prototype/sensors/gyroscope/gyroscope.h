/*
cree par : RobinAZERTY
version du 11/09/2022
*/

#pragma once

#define IS_VALIDATED false

#if !IS_VALIDATED
#include "../sensor/sensor.cpp"

#else
#include "../sensor/sensor.cpp"
#endif

class gyroscope : public sensor
{
    public:
        gyroscope(){description="gyro";};
        gyroscope(Vector position, Quaternion orientation);
        const Vector measure(const State &robot_state);
};