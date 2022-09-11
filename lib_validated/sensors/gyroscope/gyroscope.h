#pragma once

#define IS_VALIDATED true


#include "../sensor/sensor.cpp"


class gyroscope : public sensor
{
    public:
        gyroscope(){};
        gyroscope(Vector position, Quaternion orientation);
        const Vector measure(const State &robot_state);
};