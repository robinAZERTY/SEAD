
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

    void set_nonOrthogonality(const Matrix &nonOrthogonality) { this->nonOrthogonality = nonOrthogonality; }
    void set_offsets(const Vector &offsets) { this->offsets = offsets; }
    void set_std_noise(const double std_noise[3])
    {
        this->std_noise[0] = std_noise[0];
        this->std_noise[1] = std_noise[1];
        this->std_noise[2] = std_noise[2];
    }
    void set_resolution(const double resolution[3])
    {
        this->resolution[0] = resolution[0];
        this->resolution[1] = resolution[1];
        this->resolution[2] = resolution[2];
    }
    
    const Vector get_samples(const Vector &physical_quantities_3Dvalue);

private:
    Vector internal_posisiton;       // position in the robot reference
    Quaternion internal_orientation; // orientation in the robot reference

    // features of the sensor

    Matrix nonOrthogonality = IdentityMatrix(3);
    Vector offsets = Vector(3);

    double std_noise[3] = {0, 0, 0};
    double resolution[3] = {0, 0, 0};
};