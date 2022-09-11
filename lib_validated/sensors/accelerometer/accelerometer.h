#pragma once

#define IS_VALIDATED true

#include "../sensor/sensor.cpp"

class accelerometer : public sensor
{
    public:
        accelerometer(){};
        accelerometer(Vector position, Quaternion orientation);
        const Vector measure(const State &robot_state);
};