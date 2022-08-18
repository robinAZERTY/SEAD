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
	state=Matrix();
	initialState = state;
	finalState = state;
	inited = true;
}

void MotionBase::update_state(const double &dt)
{
}

Matrix MotionBase::get_state(const double &t)
{
	if (!inited)
	{
		init();
	}
	update_state(t);
	return this->state;
}

Matrix MotionBase::get_initial_state()
{
	return this->initialState;
}

Matrix MotionBase::get_final_state()
{
	return this->finalState;
}