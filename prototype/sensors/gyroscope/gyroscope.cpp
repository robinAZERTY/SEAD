#include "gyroscope.h"

gyroscope::gyroscope(Vector position, Quaternion orientation) : sensor(position, orientation)
{
}

const Vector gyroscope::measure(const State &robot_state)
{
    
    // the gyroscope measures the angular velocity of the robot, in the sensor reference frame
    // angular velocity of the robot in the robot reference frame
    Vector w_1(3);
    w_1 = quaternion_to_vector(robot_state.orientationState.q_velocity*robot_state.orientationState.q)*2;
    // we just have to convert the angular velocity in the robot frame to the sensor reference frame
    Vector w_2(3);
    w_2 = quaternion_to_vector(internal_orientation*vector_to_quaternion(w_1)*internal_orientation.conjugate());
    //et voila
    return get_samples(w_2);
    }