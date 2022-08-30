/*
créé par : robinAZERTY
version du : 27/08/2022 - 0 obsolete par rapport à documents\Motions.pdf
source : http://roboop.sourceforge.net/htmldoc/robotse9.html
*/

#include "..\..\../lib_validated/scenario construction/OrientationMotionBase/OrientationMotionBase.cpp" //when this header is a prototype
//#include "../OrientationMotionBase/OrientationMotionBase.cpp" //when this header is a valid

#include "..\..\../lib_validated/math/quaternion/quaternion.cpp" //when this header is a prototype
//#include "..\../math/quaternion/quaternion.cpp" //when this header is a valid

class Slerp
{
public:
    Slerp();
    Slerp(const Quaternion &q1, const Quaternion &q2);
    Quaternion q1, q2, q1_prime, q2_prime;
    double s, dp, ohm, s0, s1, s2, c0, c1, c2;
    void update_s(const double &s);
    void update_q(const Quaternion &q1, const Quaternion &q2);
    void update_q_primes(const Quaternion &q1_prime, const Quaternion &q2_prime);

    const Quaternion SLERP() const;
    const Quaternion PRIME() const;
};

class BezierOrientationMotion : public OrientationMotionBase
{
public:
    BezierOrientationMotion();
    BezierOrientationMotion(const Quaternion q[4], const double &duration);

private:
    Quaternion qA, qB, qC, qD;
    Slerp Slerp_1, Slerp_2, Slerp_3;
    Slerp Slerp_4, Slerp_5;
    Slerp Slerp_6;
};

