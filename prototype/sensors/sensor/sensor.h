/*
cree par : RobinAZERTY
version du 15/09/2022
*/

#pragma once

#define IS_VALIDATED false

// inlude all motions

#if !IS_VALIDATED
#include "..\..\../lib_validated/exchange data/state/State.cpp"

#else
#include "..\../exchange data/state/State.cpp"
#endif

class sensor
{
public:
    sensor(){};
    sensor(Vector position, Quaternion orientation);
    virtual ~sensor(){}; // polymorphic destructor

    void set_nonOrthogonality(const Matrix &nonOrthogonality) { this->nonOrthogonality = nonOrthogonality; }
    void set_offsets(const Vector &offsets) { this->offsets = offsets; }
    void set_position(const Vector &position) { this->internal_posisiton = position; }
    void set_orientation(const Quaternion &orientation) { this->internal_orientation = orientation; }
    void set_std_noise(const double std_noise[3]);
    void set_resolution(const double resolution[3]);
    void set_description(const string &description) { this->description = description; }
    string description = "sensor";

    const Vector get_samples() const { return samples; }
    

protected:
    const Vector get_samples(const Vector &physical_quantities_3Dvalue);
    Vector samples;

    Vector internal_posisiton = Vector(3); // position in the robot reference
    Quaternion internal_orientation;       // orientation in the robot reference

    // features of the sensor

    Matrix nonOrthogonality = IdentityMatrix(3);
    Vector offsets = Vector(3);

    double std_noise[3] = {0, 0, 0};
    double resolution[3] = {0, 0, 0};
};