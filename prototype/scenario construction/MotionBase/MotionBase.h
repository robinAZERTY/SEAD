#pragma once
#include "State.h"

class MotionBase
{
public:
	MotionBase(){};
	~MotionBase(){};
	virtual void set_parameters();
	
	void set_duration(const double &duration);
	Matrix get_state(const double &t);
	Matrix get_initial_state();
	Matrix get_final_state();


protected:

	double duration = 0;

	virtual void update_state(const double &t);
	virtual void init();

	Matrix initialState,state,finalState; // output

	bool inited = false;
};
