#include "State.h"

PositionState::PositionState(PositionState &another)
{
    this->position = another.position;
    this->velocity = another.velocity;
    this->acceleration = another.acceleration;
    this->description = "PositionState by another";
}

void PositionState::set_description(const string &description)
{
    this->description = description;
}

string PositionState::to_str() const
{
    return this->description + " : " + "\n" + "position : " + this->position.to_str() + "\n" + "velocity : " + this->velocity.to_str() + "\n" + "acceleration : " + this->acceleration.to_str();
}

OrientationState::OrientationState(OrientationState &another)
{
    this->E_x = another.E_x;
    this->E_y = another.E_y;
    this->E_z = another.E_z;

    this->dE_x = another.dE_x;
    this->dE_y = another.dE_y;
    this->dE_z = another.dE_z;

    this->description = "OrientationState by another";
}

void OrientationState::set_description(const string &description)
{
    this->description = description;
}

string OrientationState::to_str() const
{
    return this->description + " : " + "\n" + "E_x : " + this->E_x.to_str() + "\n" + "E_y : " + this->E_y.to_str() + "\n" + "E_z : " + this->E_z.to_str() + "\n" + "dE_x : " + this->dE_x.to_str() + "\n" + "dE_y : " + this->dE_y.to_str() + "\n" + "dE_z : " + this->dE_z.to_str();
}

State::State(State &another)
{
    this->positionState = another.positionState;
    this->orientationState = another.orientationState;
    this->description = "State by another";
}

void State::set_description(const string &description)
{
    this->description = description;
}

string State::to_str() const
{
    return this->description + " : " + "\n" + this->positionState.to_str() + "\n" + this->orientationState.to_str();
}