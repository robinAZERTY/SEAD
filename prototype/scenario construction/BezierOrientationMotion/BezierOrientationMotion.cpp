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

const double quat_dot(const Quaternion &q1, const Quaternion &q2)
{
    return q1.b * q2.b + q1.c * q2.c + q1.d * q2.d + q1.a * q2.a;
}
Slerp::Slerp()
{
    this->q1_prime.a = 0;
    this->q2_prime.a = 0;
}
Slerp::Slerp(const Quaternion &q1, const Quaternion &q2)
{
    this->update_q(q1, q2);
    this->q1_prime.a = 0;
    this->q2_prime.a = 0;
}
void Slerp::update_s(const double &s)
{
    const double a = ohm * s;
    const double b = ohm - a;
    s1 = sin(a);
    s2 = sin(b);
    c1 = cos(a);
    c2 = cos(b);
}
void Slerp::update_q(const Quaternion &q1, const Quaternion &q2)
{
    this->q1 = q1;
    this->q2 = q2;

    this->dp = quat_dot(q1, q2);
    this->ohm = acos(dp);
    this->s0 = sin(ohm);
    this->c0 = dp; // cos(ohm)
}

void Slerp::update_q_primes(const Quaternion &q1_prime, const Quaternion &q2_prime)
{
    this->q1_prime = q1_prime;
    this->q2_prime = q2_prime;
}

const Quaternion Slerp::SLERP() const
{
    Quaternion r;
    r = q1 * (s2 / s0) + q2 * (s1 / s0);
    r = r.normalize();
    return r;
}

const Quaternion Slerp::PRIME() const
{
    const Quaternion cst1 = (q2 * c1 - q1 * c2) * 2 * ohm;
    const Quaternion cst2 = q1_prime * s2 + q2_prime * s1;
    const Quaternion cst3 = (q1 * ((1 - s) * c2 * s0 - s2 * c0) + q2 * (s * c1 * s0 - s1 * c0)) / s0;
    const double cst4 = -1 / sqrt(1 - dp * dp) * (quat_dot(q1_prime, q2) + quat_dot(q2_prime, q1));

    return (cst1 + cst2 + cst3 * cst4) / s0;
}

const Quaternion Slerp::PRIME2() const
{
    const bool q_not_moving = (q1_prime.norm() == 0 && q1_prime.norm() == 0);
    const bool ohm_lim = (1 - abs(dp) < 0.0001); // ohm ->0

    if (ohm_lim)
    {
        if (q_not_moving)
        {
            Quaternion ret = q2_prime - q1_prime;
            return ret;
        }

        const Quaternion cst1 = q2_prime - q1_prime;
        const Quaternion csta = q2 * 1 / 3 * (s * s * s - s);
        const Quaternion cstb = q1 * (-1) / 3 * s * (2 - 3 * s + s * s);
        const double cstc = quat_dot(q1_prime, q2) + quat_dot(q2_prime, q1);
        const Quaternion cst2 = (csta + cstb) * cstc;
        const Quaternion cst3 = q1_prime * (1 - s);
        const Quaternion cst4 = q2_prime * s;
        Quaternion ret = cst1 + cst2 + cst3 + cst4;
        return ret;
    }

    const Quaternion cst1 = (q2 * c1 - q1 * c2) * ohm;

    if (q_not_moving)
    {
        Quaternion ret = cst1 / s0;
        return ret;
    }

    const Quaternion csta = q1 * (c2 * (1 - s) - c0 * s2 / s0);
    const Quaternion cstb = q2 * (c1 * s - c0 * s1 / s0);
    const double cstc = -1 / sqrt(1 - dp * dp) * (quat_dot(q1_prime, q2) + quat_dot(q2_prime, q1));
    const Quaternion cst2 = (csta + cstb) * cstc;
    const Quaternion cst3 = q1_prime * s2;
    const Quaternion cst4 = q2_prime * s1;

    Quaternion ret = (cst1 + cst2 + cst3 + cst4) / s0;
    return ret;
}