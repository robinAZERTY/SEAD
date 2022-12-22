/*
cree par : RobinAZERTY
version du 11/09/2022
*/

#include "accelerometer.h"
// gravity in Lille (France) at 50° 38' 0.0000" N, 3° 3' 0.0000" E
#define GRAVITY 9.80665

accelerometer::accelerometer(Vector position, Quaternion orientation) : sensor(position, orientation)
{
    description="accel";
}


const Vector accelerometer::measure(const State &robot_state)
{   
    // get the linear acceleration in the sensor reference
    // so we need to rotate the linear acceleration in the robot reference
    // R0 : inertial world reference
    // R1 : robot reference (can move in R0) it is robot_state
    // R2 : sensor reference (can't move in R1)
    // beetwen 2 references, we have 2 transformations : rotation(quaternion) and translation(vector)

    // if the sensor is not centered in the robot origin, the sensor will measure additionnal acceleration if the robot is rotating
    // compute angular velocity in R0 (can change in time)
    Vector w_0 = quaternion_to_vector(robot_state.orientationState.q_velocity * robot_state.orientationState.q.conjugate());
    // the sensor position in R0 (can change)
    Vector P_0 = robot_state.positionState.position + quaternion_to_vector(robot_state.orientationState.q * vector_to_quaternion(internal_posisiton) * robot_state.orientationState.q.conjugate());
    // the sensor velocity in R0 (can change)
    Vector V_0 = robot_state.positionState.velocity + cross(w_0, P_0);
    // the angular acceleration in R0 (can change)
    Vector wa_0 = quaternion_to_vector(robot_state.orientationState.q_acceleration * robot_state.orientationState.q.conjugate());
    // the sensor acceleration in R0 (can change)
    Vector A_0 = robot_state.positionState.acceleration + cross(wa_0, P_0) + cross(w_0, V_0);
    // add gravity in R0
    A_0.set(2, A_0(2) - GRAVITY);

    // get the linear acceleration in R1
    Vector A_1 = quaternion_to_vector(robot_state.orientationState.q.conjugate() * vector_to_quaternion(A_0) * robot_state.orientationState.q);
    // get the linear acceleration in R2
    Vector A_2 = quaternion_to_vector(internal_orientation.conjugate() * vector_to_quaternion(A_1) * internal_orientation);
    
    return get_samples(A_2);
    }