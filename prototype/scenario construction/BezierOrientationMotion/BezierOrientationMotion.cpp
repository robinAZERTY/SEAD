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
    r = q1 * (sin((1 - t) * theta) / sn) + q2 * (sin(t * theta) / sn);
    r = r.normalize();
    return r;
} 

const double quat_dot(const Quaternion &q1, const Quaternion &q2)
{
    return q1.b * q2.b + q1.c * q2.c + q1.d * q2.d + q1.a * q2.a;
}

const Quaternion Slerp::PRIME(const Quaternion &q1_prime, const Quaternion &q2_prime, const double &t) const
{
    const double dp = quat_dot(q1,q2);
    const double ohm = acos(dp);
    const double a = ohm * t;
    const double b = ohm - a;
    const double s0 = sin(ohm);
    const double s1 = sin(a);
    const double s2 = sin(b);
    const double c0 = cos(ohm);
    const double c1 = cos(a);
    const double c2 = cos(b);

    const Quaternion cst1 = (q2 * c1 - q1 * c2) * 2 * ohm;
    const Quaternion cst2 = q1_prime * s2 + q2_prime * s1;
    const Quaternion cst3 = (q1 * ((1 - t) * c2 * s0 -s2 * c0) + q2 * (t * c1 * s0 -s1 * c0)) / s0;
    const double cst4 = -1 / sqrt(1 - dp * dp) * (quat_dot(q1_prime,q2)+ quat_dot(q2_prime,q1));

    return (cst1 + cst2 + cst3 * cst4) / s0;
} 