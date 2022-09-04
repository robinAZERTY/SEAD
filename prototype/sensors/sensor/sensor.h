
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
    const double get_sample(const double &physical_quantities_value(const double &time),const double &time);

private:
    PositionState internal_posisiton;      // position in the robot reference
    OrientationState internal_orientation; // orientation in the robot reference
    
    //features of the sensor
    string physical_quantities_observed;//acceleration,magnetisme,angular_velocity... in 1 direction !
    double linearity(const double physical_quantities_value);//depend also of temperature range and operating_range
    double std_noise;
    double resolution;
    double lag_delay;
    double update_freq;
    double passband_freq[2]={0,9.9e100};
    double operating_range[2]={-9.9e-100,9.9e100};
    double temperature_operating_range[2]={-9.9e-100,9.9e100};
};