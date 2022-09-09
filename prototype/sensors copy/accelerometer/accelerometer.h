#pragma once

#include "../sensor/sensor.h"

class accelerometer : sensor
{
    public:
        accelerometer();
        accelerometer(Vector position, Quaternion orientation);
        const Vector measure(const State &robot_state);
};