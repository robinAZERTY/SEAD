/*
créé par : robinAZERTY
version du : 21/08/2022-1
*/

#pragma once

#include <string>
#include "..\..\math/vector/Vector.cpp" // when this header is valid
//#include "..\..\..\lib_validated\math\vector\Vector.cpp" // when this header is a prototype

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

    Vector E_x =Vector(3);
    Vector E_y = Vector(3);
    Vector E_z = Vector(3);

    Vector dE_x = Vector(3);
    Vector dE_y = Vector(3);
    Vector dE_z = Vector(3);
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