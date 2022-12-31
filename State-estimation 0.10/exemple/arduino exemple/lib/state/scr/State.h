/*
créé par : robinAZERTY
version du : 02/09/2022-1
*/

#ifndef STATE_H
#define STATE_H

#include <Arduino.h>
#include "../../math/scr/quaternion.h"

class PositionState
{
private:
    String description = "default PositionState";

public:
    PositionState();
    PositionState(PositionState &another);

    void set_description(const String &description);
    String to_str() const;

    Vector position;
    Vector velocity;
    Vector acceleration;
};

class OrientationState
{
private:
    String description = "default OrientationState";

public:
    OrientationState();
    OrientationState(OrientationState &another);

    void set_description(const String &description);
    String to_str() const;

    Quaternion q,q_velocity,q_acceleration;
};

class State
{
private:
    String description = "default State";

public:
    State();
    State(State &another);

    PositionState positionState;
    OrientationState orientationState;

    void set_description(const String &description);
    String to_str() const;
};
#include "State.cpp"
#endif