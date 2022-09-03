/*
créé par : robinAZERTY
version du : 19/08/2022 - 2
*/
#pragma once
#include "PositionMotionBase.h"

void PositionMotionBase::set_parameters()
{
    inited = false;
}
void PositionMotionBase::set_duration(const double &duration)
{
    this->duration = duration;
}
void PositionMotionBase::init()
{
    state = PositionState();
    initialState = state;
    finalState = state;
    inited = true;
}
void PositionMotionBase::update_state(const double &dt)
{
}
PositionState PositionMotionBase::get_state(const double &t)
{
    if (!inited)
        init();

    update_state(t);
    return this->state;
}
PositionState PositionMotionBase::get_initial_state()
{
    if (!inited)
        init();

    return this->initialState;
}
PositionState PositionMotionBase::get_final_state()
{
    if (!inited)
        init();

    return this->finalState;
}

string PositionMotionBase::to_str()
{
    string ret;
    ret += description + " :\n";
    ret += "duration : " + to_string(duration) + "\n";
    ret += "initial state : " + get_initial_state().to_str() + "\n";
    ret += "final state : " + get_final_state().to_str() + "\n";
    ret += "state : " + state.to_str() + "\n";
    return ret;
}