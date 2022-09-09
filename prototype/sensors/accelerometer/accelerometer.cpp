#include "accelerometer.h"

accelerometer::accelerometer():sensor()
{
}
accelerometer::accelerometer(Vector position, Quaternion orientation):sensor(Vector position, Quaternion orientation)
{ 
}

const Vector accelerometer::measure(const State &robot_state)
{
    
}