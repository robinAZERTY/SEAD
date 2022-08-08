#pragma once
#include "State.h"



class MotionBase
{
public:
	MotionBase(){};
	~MotionBase(){};

	void set_parameters();
	State get_state(const double &t);

private:

	struct INPUT
	{
	};

	struct INPUT parameters; // input

	void update_state(const double &t);
	void init();
	
	State state; // output

	bool inited = false;
};
