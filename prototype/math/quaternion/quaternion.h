/*
créé par : robinAZERTY
version du : 31/08/2022 - 1
*/

#pragma once

#include <string>
#include <math.h>

//#include "../vector/Vector.cpp" // when this header is valid
#include "..\..\../lib_validated\math\vector\Vector.cpp" // when this header is a prototype

using namespace std;
class Quaternion
{
    public :

        double a, b, c, d;

        Quaternion();
        Quaternion(const Quaternion& q);
        Quaternion(const double& yaw, const double& pitch, const double& roll);
        Quaternion(const double& angle, const double& x, const double& y, const double& z);
        ~Quaternion();

        const Quaternion& operator=(const Quaternion& q);
        const Quaternion operator+(const Quaternion& q) const;
        const Quaternion operator+=(const Quaternion& q);
        const Quaternion operator-(const Quaternion& q) const;
        const Quaternion operator-=(const Quaternion& q);
        const Quaternion operator*(const Quaternion& q) const;
        const Quaternion operator*=(const Quaternion& q);
        const Quaternion operator*(const double& d) const;
        const Quaternion operator*=(const double& d);
        const Quaternion operator/(const double& d) const;
        const Quaternion operator/=(const double& d);
        const bool operator==(const Quaternion& q) const;
        const bool operator!=(const Quaternion& q) const;
        
        const Quaternion conjugate() const;
        const Quaternion inverse() const;
        const double norm() const;
        const Quaternion normalize() const;
        const Quaternion rotate(const Quaternion& q) const;

        const double theta()const;
        const Vector v()const;
        const Quaternion operator^(const double &d)const;
        

        const double* yaw_pitch_roll() const;//return Body 3-2-1 angles (RZ,RY,RX) https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles

        void set_description(const string& description);
        const string to_str() const;

        private :
            string description = "Quaternion";
};