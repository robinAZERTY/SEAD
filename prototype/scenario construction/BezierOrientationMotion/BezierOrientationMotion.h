/*
créé par : robinAZERTY
version du : 27/08/2022 - 0 obsolete par rapport à documents\Motions.pdf
*/

#include "..\..\../lib_validated/scenario construction/OrientationMotionBase/OrientationMotionBase.cpp" //when this header is a prototype
//#include "../OrientationMotionBase/OrientationMotionBase.cpp" //when this header is a valid

#include "..\..\../lib_validated/math/quaternion/quaternion.cpp"//when this header is a prototype
//#include "..\../math/quaternion/quaternion.cpp" //when this header is a valid


class BezierOrientationMotion : public OrientationMotionBase
{
    public :
        BezierOrientationMotion();
        BezierOrientationMotion(const Quaternion q[4], const double &duration);

    private :
       const Quaternion Slerp(const Quaternion &q1, const Quaternion &q2, const double &t) const;
};