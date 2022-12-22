/*
créé par : robinAZERTY
version du : 02/09/2022 - 0 obsolete par rapport à documents\Motions.pdf
source : http://roboop.sourceforge.net/htmldoc/robotse9.html
*/

#if false // true when this header is a validated
#include "../OrientationMotionBase/OrientationMotionBase.cpp" //when this header is a valid
#else
#include "..\..\../lib_validated/scenario construction/OrientationMotionBase/OrientationMotionBase.cpp" //when this header is a prototype
#endif

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
    BezierOrientationMotion(const OrientationState initialState, const OrientationState finalState, const double &duration);

    virtual OrientationState get_state(const double &t);
    virtual OrientationState get_initial_state();
    virtual OrientationState get_final_state();

    const Quaternion SQUAD(const double &s);
    const Quaternion PRIME(const double &s);
    const Quaternion SECOND_PRIME(const double &s);

private:
    virtual void update_state(const double &t);
    virtual void init();
    Quaternion qA, qB, qC, qD;
    Slerp Slerp_1, Slerp_2, Slerp_3;
    Slerp Slerp_4, Slerp_5;
    Slerp Slerp_6;
    double alpha;
};
