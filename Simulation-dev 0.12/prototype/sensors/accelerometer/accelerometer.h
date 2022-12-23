/*
cree par : RobinAZERTY
version du 11/09/2022
*/

#pragma once

#include "../sensor/sensor.cpp"


class accelerometer : public sensor
{
    public:
        accelerometer(){description="accel";};
        accelerometer(Vector position, Quaternion orientation);
        virtual ~accelerometer(){};
        const Vector measure(const State &robot_state, const double &time);
};