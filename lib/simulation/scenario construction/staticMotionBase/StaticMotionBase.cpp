#include "StaticMotionBase.h"

void StaticMotionBase::set_parameters(const float parameters[6])
{
	for (int i = 0; i < 6; i++)
	{
		this->parameters[i] = parameters[i];
	}
}

State StaticMotionBase::get_state(const float& t)
{
	if (!inited)
	{
		init();
	}

	set_state(t);

	return this->state;
}

void StaticMotionBase::init()
{
	
}
void StaticMotionBase::set_state(const float& t)
{
	//nothing
}