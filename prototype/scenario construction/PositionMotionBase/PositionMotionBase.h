/*
créé par : RobinAZERTY
version du : 19/08/2022
*/

#pragma once

//#include "State.h"//when this header is valid
#include "..\..\../lib_validated/exchange data/state/State.cpp" //when this header is a prototype

class PositionMotionBase
{
public:
    PositionMotionBase(){};
    ~PositionMotionBase(){};
    
    virtual void set_parameters();

    void set_duration(const double &duration);

    PositionState get_state(const double &t);
    PositionState get_initial_state();
    PositionState get_final_state();

protected:

    virtual void update_state(const double &t);
    virtual void init();
    double duration = 0;
    PositionState initialState, state, finalState; // output
    bool inited = false;
};