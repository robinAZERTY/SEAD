/*
créé par : robinAZERTY
version du : 27/08/2022 - 0
*/

#include "BezierOrientationMotion.h"

static const double ds = 1.0e-5;

const double quat_dot(const Quaternion &q1, const Quaternion &q2)
{
    return q1.b * q2.b + q1.c * q2.c + q1.d * q2.d + q1.a * q2.a;
}
Slerp::Slerp()
{
}
void Slerp::update(const Quaternion &q1, const Quaternion &q2, const double &s)
{
    this->q1 = q1;
    this->q2 = q2;
    this->s = s;
}

const Quaternion Slerp::SLERP()
{
    if (q1 == q2)
        return q1;
    else
        return ((q2 * q1.inverse())^s) * q1;
}

BezierOrientationMotion::BezierOrientationMotion() : OrientationMotionBase()
{
    this->description = "default BezierOrientationMotion";
}

BezierOrientationMotion::BezierOrientationMotion(const Quaternion q[4], const double &duration)
{
    qA = q[0];
    qB = q[1];
    qC = q[2];
    qD = q[3];
    this->duration = duration;
    this->alpha = 1 / duration;
}


BezierOrientationMotion::BezierOrientationMotion(const OrientationState initialState, const OrientationState finalState, const double &duration)
{
    this->duration = duration;
    this->alpha = 1 / duration;

    qA=initialState.q;
    qD=finalState.q;

    qB = (((qA+initialState.q_velocity*ds).normalize()*qA.inverse())^(1/(alpha*3.0*ds)))*qA;
    qC = (((qD-finalState.q_velocity*ds).normalize()*qD.inverse())^(1/(alpha*3.0*ds)))*qD;
}

const Quaternion BezierOrientationMotion::SQUAD(const double &s)
{
    Slerp_1.update(qA, qB, s);
    Slerp_2.update(qB, qC, s);
    Slerp_3.update(qC, qD, s);
    Slerp_4.update(Slerp_1.SLERP(), Slerp_2.SLERP(), s);
    Slerp_5.update(Slerp_2.SLERP(), Slerp_3.SLERP(), s);
    Slerp_6.update(Slerp_4.SLERP(), Slerp_5.SLERP(), s);
    

    return Slerp_6.SLERP();
}



const Quaternion BezierOrientationMotion::PRIME(const double &s)
{
    return (SQUAD(s + ds / 2) - SQUAD(s - ds / 2)) / ds;
}

const Quaternion BezierOrientationMotion::SECOND_PRIME(const double &s)
{
    return (PRIME(s + ds / 2) - PRIME(s - ds / 2)) / ds;
}

void BezierOrientationMotion::update_state(const double &t)
{
    // verification de la validité des paramètres
    if (t < 0 || t > duration)
        throw "t must be between 0 and duration";

    const double s = t * alpha;

    state.q = SQUAD(s);
    state.q_velocity = PRIME(s) * alpha;
    state.q_acceleration = SECOND_PRIME(s)*alpha;
}

void BezierOrientationMotion::init()
{
    update_state(duration);
    finalState = state;

    update_state(0);
    initialState = state;

    state = initialState;

    inited = true;
}