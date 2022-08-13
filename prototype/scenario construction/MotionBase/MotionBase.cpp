#include "MotionBase.h"

void MotionBase::set_parameters()
{
	inited = false;
}

void MotionBase::set_duration(const double &duration)
{
	this->duration = duration;
}

void MotionBase::init()
{
	state.position = Matrix();
	state.velocity = Matrix();
	state.acceleration = Matrix();
	state.orientation = Quaternion();
	state.angular_velocity = Matrix();
	state.angular_acceleration = Matrix();

	initialState = state;
	finalState = state;
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

State MotionBase::get_initial_state()
{
	return this->initialState;
}

State MotionBase::get_final_state()
{
	return this->finalState;
}