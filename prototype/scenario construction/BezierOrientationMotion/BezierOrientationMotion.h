/*
créé par : robinAZERTY
version du : 02/09/2022 - 0 obsolete par rapport à documents\Motions.pdf
source : http://roboop.sourceforge.net/htmldoc/robotse9.html
*/

#include "..\..\../lib_validated/scenario construction/OrientationMotionBase/OrientationMotionBase.cpp" //when this header is a prototype
//#include "../OrientationMotionBase/OrientationMotionBase.cpp" //when this header is a valid

#include "..\..\../lib_validated/math/quaternion/quaternion.cpp" //when this header is a prototype
//#include "..\../math/quaternion/quaternion.cpp" //when this header is a valid


//#include "..\..\../lib_validated/math/derivation/derivation.h"////when this header is a prototype

class Slerp
{
public:
    Slerp();
    Quaternion q1, q2;
    double s;
    
    void update(const Quaternion &q1, const Quaternion &q2, const double &s);
    const Quaternion SLERP();
};

class BezierOrientationMotion : public OrientationMotionBase
{
public:
    BezierOrientationMotion();
    BezierOrientationMotion(const Quaternion q[4], const double &duration);
    Quaternion q,q_der;

//private:
    const Quaternion SQUAD(const double &s);
    const Quaternion PRIME(const double &s);
    virtual void update_state(const double &t);
    virtual void init();
    Quaternion qA, qB, qC, qD;
    Slerp Slerp_1, Slerp_2, Slerp_3;
    Slerp Slerp_4, Slerp_5;
    Slerp Slerp_6;
    double alpha;
};

