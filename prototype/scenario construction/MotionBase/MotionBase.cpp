#include "MotionBase.h"

void MotionBase::set_parameters()
{
	inited = false;
}

void MotionBase::init()
{
	state.position = Matrix(3, 1);
	state.velocity = Matrix(3, 1);
	state.acceleration = Matrix(3, 1);
	state.orientation = Quaternion();
	state.angular_velocity = Matrix(3, 1);
	state.angular_acceleration = Matrix(3, 1);

	state.position.fill(0);
	state.velocity.fill(0);
	state.acceleration.fill(0);
	state.angular_velocity.fill(0);
	state.angular_acceleration.fill(0);
	inited = true;
}

void MotionBase::update_state(const double &dt)
{
}

State MotionBase::get_state(const double &t)
{
	if (!inited)
	{
		init();
	}
	update_state(t);
	return this->state;
}