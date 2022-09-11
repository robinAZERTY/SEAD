#pragma once

#define IS_VALIDATED false


#if !IS_VALIDATED
#include "../sensor/sensor.cpp"

#else
#include "../sensor/sensor.cpp"
#endif

class accelerometer : public sensor
{
    public:
        accelerometer(){};
        accelerometer(Vector position, Quaternion orientation);
        virtual ~accelerometer(){};
        const Vector measure(const State &robot_state);
};