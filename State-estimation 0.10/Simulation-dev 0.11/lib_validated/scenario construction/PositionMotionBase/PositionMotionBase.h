/*
créé par : RobinAZERTY
version du : 19/08/2022
*/

#pragma once

#include "../../../..\common\exchange data\state\State.cpp"//when this header is valid
//#include "..\..\../lib_validated/exchange data/state/State.cpp" //when this header is a prototype
#include <string>

class PositionMotionBase
{
public:
    PositionMotionBase(){};
    ~PositionMotionBase(){};

    virtual void set_parameters();
    virtual string to_str();

    void set_duration(const double &duration);
    const double get_duration(){return duration;}

    virtual PositionState get_state(const double &t);
    virtual PositionState get_initial_state();
    virtual PositionState get_final_state();

protected:

    virtual void update_state(const double &t);
    virtual void init();
    double duration = 0;
    PositionState initialState, state, finalState; // output
    bool inited = false;
    string description = "default PositionMotionBase";
};