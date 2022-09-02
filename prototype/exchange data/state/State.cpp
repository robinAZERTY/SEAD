/*
créé par : robinAZERTY
version du : 02/09/2022-1
*/

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

OrientationState::OrientationState()
{
    q_velocity*=0;
    q_acceleration=q_velocity;
}

OrientationState::OrientationState(OrientationState &another)
{
    this->q=another.q;
    this->q_velocity=another.q_velocity;
    this->q_acceleration=another.q_acceleration;

    this->description = "OrientationState by another";
}

void OrientationState::set_description(const string &description)
{
    this->description = description;
}

string OrientationState::to_str() const
{
    return this->description + " : " + "\n" + "q : " + this->q.to_str() + "\n" + "q_derivate : " + this->q_velocity.to_str()+ "\n" + "q_acceleration : " + this->q_acceleration.to_str();
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