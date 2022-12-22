/*
cree par : RobinAZERTY
version du 11/09/2022
*/

#pragma once


#include "../sensor/sensor.cpp"

class magnetometer : public sensor
{
    public:
        magnetometer();
        magnetometer(Vector position, Quaternion orientation);//position relative au robot
        virtual ~magnetometer(){};
        const Vector measure(const State &robot_state, const double &time);

    private:
        Vector mag_field;
};