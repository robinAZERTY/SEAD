#include "StaticMotionBase.h"

void StaticMotionBase::set_parameters(const Matrix &position, const Quaternion &orientation)
{
	parameters.position = position;
	parameters.orientation = orientation;
	inited = false;
}

void StaticMotionBase::init()
{
	state.position = parameters.position;
	state.orientation = parameters.orientation;
	state.velocity = Matrix(3, 1);
	state.acceleration = Matrix(3, 1);
	state.angular_velocity = Matrix(3, 1);
	state.angular_acceleration = Matrix(3, 1);
	state.velocity.fill(0);
	state.acceleration.fill(0);
	state.angular_velocity.fill(0);
	state.angular_acceleration.fill(0);
	inited = true;	
}

void StaticMotionBase::update_state(const double &dt)
{

}


State StaticMotionBase::get_state(const double& t)
{
	if (!inited)
	{
		init();
	}
	update_state(t);
	return this->state;
}