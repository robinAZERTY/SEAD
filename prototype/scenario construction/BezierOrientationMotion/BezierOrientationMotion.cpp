/*
créé par : robinAZERTY
version du : 27/08/2022 - 0
*/

#include "BezierOrientationMotion.h"

BezierOrientationMotion::BezierOrientationMotion(const Quaternion q[4], const double &duration)
{
    qA = q[0];
    qB = q[1];
    qC = q[2];
    qD = q[3];
    this->duration = duration;
}

Slerp::Slerp(const Quaternion &q1, const Quaternion &q2)
{
    this->theta = acos(q1.b * q2.b + q1.c * q2.c + q1.d * q2.d + q1.a * q2.a);
    this->sn = sin(theta);
    this->q1 = q1;
    this->q2 = q2;
}
const Quaternion Slerp::SLERP(const double &t) const
{
    Quaternion r;
    double t_ = 1 - t;
    double Wa, Wb;
    Wa = sin(t_ * theta) / sn;
    Wb = sin(t * theta) / sn;
    r.b = Wa * q1.b + Wb * q2.b;
    r.c = Wa * q1.c + Wb * q2.c;
    r.d = Wa * q1.d + Wb * q2.d;
    r.a = Wa * q1.a + Wb * q2.a;
    r = r.normalize();
    return r;
}