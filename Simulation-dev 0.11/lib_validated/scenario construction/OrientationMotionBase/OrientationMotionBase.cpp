/*
créé par : robinAZERTY
version du : 20/08/2022 - 1
*/
#pragma once
#include "OrientationMotionBase.h"

void OrientationMotionBase::set_parameters()
{
    inited = false;
}
void OrientationMotionBase::set_duration(const double &duration)
{
    this->duration = duration;
}
void OrientationMotionBase::init()
{
    state = OrientationState();
    initialState = state;
    finalState = state;
    inited = true;
}
void OrientationMotionBase::update_state(const double &dt)
{
}
OrientationState OrientationMotionBase::get_state(const double &t)
{
    if (!inited)
    {
        init();
    }
    update_state(t);
    return this->state;
}
OrientationState OrientationMotionBase::get_initial_state()
{
    return this->initialState;
}
OrientationState OrientationMotionBase::get_final_state()
{
    return this->finalState;
}

string OrientationMotionBase::to_str()
{
    string ret;
    ret += description+" :\n";
    ret += "duration : "+to_string(duration)+"\n";
    ret += "initial state : "+initialState.to_str()+"\n";
    ret += "final state : "+finalState.to_str()+"\n";
    ret += "state : "+state.to_str()+"\n";
    return ret;
}