/*
créé par : robinAZERTY
version du : 19/08/2022
*/

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
    {
        init();
    }
    update_state(t);
    return this->state;
}
PositionState PositionMotionBase::get_initial_state()
{
    return this->initialState;
}
PositionState PositionMotionBase::get_final_state()
{
    return this->finalState;
}