/*
créé par : robinAZERTY
version du : 18/08/2022
*/

#pragma once

#include <string>
#include "Vector.cpp"

class PositionState
{
private:
    string description = "default PositionState";

public:
    PositionState(){};
    PositionState(PositionState &another);

    void set_description(const string &description);
    string to_str() const;

    Vector position = Vector(3);
    Vector velocity = Vector(3);
    Vector acceleration = Vector(3);
};

class OrientationState
{
private:
    string description = "default OrientationState";

public:
    OrientationState(){};
    OrientationState(OrientationState &another);

    void set_description(const string &description);
    string to_str() const;

    Vector orientation = Vector(4);
    Vector velocity = Vector(4);
    Vector acceleration = Vector(4);
};

class State
{
private:
    string description = "default State";

public:
    State(){};
    State(State &another);

    PositionState positionState;
    OrientationState orientationState;

    void set_description(const string &description);
    string to_str() const;
};