/*
créé par : robinAZERTY
version du : 02/09/2022-1
*/

#pragma once

#include <string>
#include "..\..\math/vector/Vector.cpp" // when this header is valid
//#include "..\..\../lib_validated/math/vector/Vector.cpp" // when this header is a prototype

//#include "..\..\../lib_validated/math/quaternion/quaternion.cpp"// when this header is a prototype
#include "..\../math/quaternion/quaternion.cpp"// when this header is valid

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
    OrientationState();
    OrientationState(OrientationState &another);

    void set_description(const string &description);
    string to_str() const;

    Quaternion q,q_velocity,q_acceleration;
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