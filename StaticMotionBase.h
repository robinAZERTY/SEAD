#pragma once
#include "State.h"

class StaticMotionBase
{
public :
	StaticMotionBase(){};
	~StaticMotionBase(){};

	void set_parameters(const float parameters[6]);

	void set_state(const float& t);

	State get_state(const float& t);


private:
	State state;

	bool inited = false;
	void init();

	float parameters[6];
	

};

