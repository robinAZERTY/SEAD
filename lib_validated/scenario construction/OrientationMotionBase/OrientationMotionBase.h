/*
créé par : RobinAZERTY
version du : 20/08/2022 -1
*/

#pragma once


//#include "..\..\../lib_validated/exchange data/state/State.cpp" //when this header is a prototype
#include "..\../exchange data/state/State.cpp" //when this header is valid
#include <string>

class OrientationMotionBase
{
public:
    OrientationMotionBase(){};
    ~OrientationMotionBase(){};

    virtual void set_parameters();
    virtual string to_str();

    void set_duration(const double &duration);
    const double get_duration(){return duration;}

    virtual OrientationState get_state(const double &t);
    virtual OrientationState get_initial_state();
    virtual OrientationState get_final_state();

protected:

    virtual void update_state(const double &t);
    virtual void init();
    double duration = 0;
    OrientationState initialState, state, finalState; // output
    bool inited = false;
    string description = "default OrientationMotionBase";
};