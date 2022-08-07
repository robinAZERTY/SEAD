#pragma once
#include "State.h"



class StaticMotionBase
{
public:
	StaticMotionBase(){};
	~StaticMotionBase(){};

	void set_parameters(const Matrix &position, const Quaternion &orientation);
	State get_state(const double &t);

private:

	struct INPUT
	{
		Matrix position = Matrix(3, 1);
		Quaternion orientation = Quaternion();
	};

	struct INPUT parameters; // input

	void update_state(const double &t);
	void init();
	
	State state; // output

	bool inited = false;
};
