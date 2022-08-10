#include "StaticMotion.h"

void StaticMotion::set_parameters(const Matrix &position, const Quaternion &orientation)
{
    parameters.position = position;
    parameters.orientation = orientation;
}

void StaticMotion::init()
{
    state.position = Matrix(3, 1);
    state.velocity = Matrix(3, 1);
    state.acceleration = Matrix(3, 1);
    state.orientation = Quaternion();
    state.angular_velocity = Matrix(3, 1);
    state.angular_acceleration = Matrix(3, 1);

    state.position = parameters.position;
    state.velocity.fill(0);
    state.acceleration.fill(0);
    state.orientation = parameters.orientation;
    state.angular_velocity.fill(0);
    state.angular_acceleration.fill(0);
    inited = true;
}