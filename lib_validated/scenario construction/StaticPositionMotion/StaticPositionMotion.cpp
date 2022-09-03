/*
créé par : robinAZERTY
version du : 20/08/2022
*/
#pragma once
#include "StaticPositionMotion.h"
StaticPositionMotion::StaticPositionMotion() : PositionMotionBase()
{
    this->description = "default StaticPositionMotion";
}

StaticPositionMotion::StaticPositionMotion(const PositionState &positionState):PositionMotionBase()
{   
    //verifier si les membres velocity et acceleration sont bien des vecteurs nuls
    if(positionState.velocity!=Vector(3) || positionState.acceleration!=Vector(3)) throw "velocity and acceleration vectors must be null";

    this->state = positionState;
    this->initialState = state;
    this->finalState = state;
    this->description = "StaticPositionMotion with position input";
}