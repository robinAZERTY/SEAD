#include "StaticMotion.h"

StaticMotion::StaticMotion(const Matrix Position)
{
    set_parameters(Position);
}

void StaticMotion::set_parameters(const Matrix &position)
{
    this->position = position;
    inited = false;
}

void StaticMotion::init()
{
    state = this->position;
    initialState = state;
    finalState = state;
    inited = true;
}