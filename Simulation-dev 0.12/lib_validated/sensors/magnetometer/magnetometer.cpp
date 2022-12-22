/*
cree par : RobinAZERTY
version du 11/09/2022
*/

#include "magnetometer.h"


/*
by WMM2020 : https://www.ngdc.noaa.gov/geomag/WMM/wmm_wdownload.shtml

Results For

Latitude        50.63N
Longitude       3.05E
Altitude:       0.10 Kilometers above the WGS-84 ellipsoid
Date:           2023.0

                Main Field                      Secular Change
F       =         48883.5 +/- 145.0 nT           Fdot =  45.6   nT/yr
H       =         19928.6 +/- 128.0 nT           Hdot =   9.4   nT/yr
X       =         19919.7 +/- 131.0 nT           Xdot =   7.5   nT/yr
Y       =           593.4 +/-  94.0 nT           Ydot =  64.7   nT/yr
Z       =         44636.9 +/- 157.0 nT           Zdot =  45.7   nT/yr
Decl    =       1 Deg  42 Min  (EAST) +/- 23 Min Ddot = 11.1    Min/yr
Incl    =      65 Deg  56 Min  (DOWN) +/- 13 Min Idot = 0.7     Min/yr

*/

#define X 19.9197
#define Y 0.5934
#define Z 44.6369//in uT

magnetometer::magnetometer() : sensor()
{
    description="mag";
    this->mag_field = Vector(3);
    this->mag_field.set(0, X);
    this->mag_field.set(1, Y);
    this->mag_field.set(2, Z);
}

magnetometer::magnetometer(Vector position, Quaternion orientation) : sensor(position, orientation)
{
    description="mag";
    this->mag_field = Vector(3);
    this->mag_field.set(0, X);
    this->mag_field.set(1, Y);
    this->mag_field.set(2, Z);
}


const Vector magnetometer::measure(const State &robot_state, const double &time)
{   
    // R0 : inertial world reference
    // R1 : robot reference (can move in R0) it is robot_state
    // R2 : sensor reference (can't move in R1)
    /*  
    Vector A_0 = quaternion_to_vector(mag_field);
    // get the mag vector in R1
    Vector A_1 = quaternion_to_vector(robot_state.orientationState.q.conjugate() * vector_to_quaternion(A_0) * robot_state.orientationState.q);
    // get the mag vector in R2
    Vector A_2 = quaternion_to_vector(internal_orientation.conjugate() * vector_to_quaternion(A_1) * internal_orientation);
    */
    //more simple and faster
    Vector A_2 = quaternion_to_vector(internal_orientation.conjugate() * robot_state.orientationState.q.conjugate() * vector_to_quaternion(this->mag_field) * robot_state.orientationState.q * internal_orientation);
    return get_samples(A_2, time);

}