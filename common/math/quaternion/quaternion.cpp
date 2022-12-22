/*
créé par : robinAZERTY
version du : 31/08/2022 - 1
*/

#include "quaternion.h"

Quaternion::Quaternion()
{
    a = 1;
    b = 0;
    c = 0;
    d = 0;
}

Quaternion::Quaternion(const Quaternion &q)
{
    a = q.a;
    b = q.b;
    c = q.c;
    d = q.d;
}

Quaternion::Quaternion(const double &angle, const double &x, const double &y, const double &z)
{
    double n = sqrt(x * x + y * y + z * z);
    if (n == 0)
    {
        a = 1;
        b = 0;
        c = 0;
        d = 0;
    }
    else
    {
        a = cos(angle / 2);
        const double s = sin(angle / 2);
        b = s * x / n;
        c = s * y / n;
        d = s * z / n;
    }
}

Quaternion::Quaternion(const double &yaw, const double &pitch, const double &roll)
{
    double c1 = cos(roll / 2);
    double s1 = sin(roll / 2);
    double c2 = cos(pitch / 2);
    double s2 = sin(pitch / 2);
    double c3 = cos(yaw / 2);
    double s3 = sin(yaw / 2);
    a = c1 * c2 * c3 + s1 * s2 * s3;
    b = s1 * c2 * c3 - c1 * s2 * s3;
    c = c1 * s2 * c3 + s1 * c2 * s3;
    d = c1 * c2 * s3 - s1 * s2 * c3;
}

Quaternion::~Quaternion()
{
}

const Quaternion &Quaternion::operator=(const Quaternion &q)
{
    a = q.a;
    b = q.b;
    c = q.c;
    d = q.d;
    return *this;
}

const Quaternion Quaternion::operator+(const Quaternion &q) const
{
    Quaternion q_sum;
    q_sum.a = a + q.a;
    q_sum.b = b + q.b;
    q_sum.c = c + q.c;
    q_sum.d = d + q.d;
    return q_sum;
}

const Quaternion Quaternion::operator+=(const Quaternion &q)
{
    *this = *this + q;
    return *this;
}

const Quaternion Quaternion::operator*(const double &scalar) const
{
    Quaternion q_scalar = *this;
    q_scalar.a *= scalar;
    q_scalar.b *= scalar;
    q_scalar.c *= scalar;
    q_scalar.d *= scalar;
    return q_scalar;
}

const Quaternion Quaternion::operator*=(const double &scalar)
{
    *this = *this * scalar;
    return *this;
}

const Quaternion Quaternion::operator/(const double &scalar) const
{

    return *this * (1 / scalar);
}

const Quaternion Quaternion::operator/=(const double &scalar)
{
    *this = *this / scalar;
    return *this;
}

const Quaternion Quaternion::operator-(const Quaternion &q) const
{
    Quaternion q_diff = *this + q * (-1);
    return q_diff;
}

const Quaternion Quaternion::operator-=(const Quaternion &q)
{
    *this = *this - q;
    return *this;
}

const Quaternion Quaternion::operator*(const Quaternion &q) const
{
    Quaternion q_product;
    q_product.a = a * q.a - b * q.b - c * q.c - d * q.d;
    q_product.b = a * q.b + b * q.a + c * q.d - d * q.c;
    q_product.c = a * q.c - b * q.d + c * q.a + d * q.b;
    q_product.d = a * q.d + b * q.c - c * q.b + d * q.a;
    return q_product;
}

const Quaternion Quaternion::operator*=(const Quaternion &q)
{
    *this = *this * q;
    return *this;
}

const bool Quaternion::operator==(const Quaternion &q) const
{
    return (a == q.a && b == q.b && c == q.c && d == q.d);
}

const bool Quaternion::operator!=(const Quaternion &q) const
{
    return !(*this == q);
}

const Quaternion Quaternion::conjugate() const
{
    Quaternion q_conj;
    q_conj.a = a;
    q_conj.b = -b;
    q_conj.c = -c;
    q_conj.d = -d;
    return q_conj;
}

const Quaternion Quaternion::inverse() const
{
    Quaternion q_inv;
    const double square = a * a + b * b + c * c + d * d;
    q_inv.a = a / square;
    q_inv.b = -b / square;
    q_inv.c = -c / square;
    q_inv.d = -d / square;
    return q_inv;
}

const double Quaternion::norm() const
{
    return sqrt(a * a + b * b + c * c + d * d);
}

const Quaternion Quaternion::normalize() const
{
    Quaternion q_norm;
    const double n = this->norm();
    q_norm.a = a / n;
    q_norm.b = b / n;
    q_norm.c = c / n;
    q_norm.d = d / n;
    return q_norm;
}

const Quaternion Quaternion::rotate(const Quaternion &q) const
{
    // This will roate the input vector by this normalized rotation quaternion.
    Quaternion q_rot;
    q_rot = (*this * q * this->conjugate());
    return q_rot;
}

const double q_dot(const Quaternion &q1, const Quaternion &q2)
{
    return q1.b * q2.b + q1.c * q2.c + q1.d * q2.d + q1.a * q2.a;
}

const double Quaternion::theta() const
{
    return acos(this->a) * 2.00;
}

const Vector Quaternion::v() const
{
    const double theta = this->theta();
    if (theta == 0)
        return Vector(3);
    const double cst = sin(theta / 2);
    Vector ret(3);
    ret.set(0, this->b / cst);
    ret.set(1, this->c / cst);
    ret.set(2, this->d / cst);
    return ret;
}

const Quaternion Quaternion::operator^(const double &d) const
{
    Quaternion ret;
    ret.a = cos(this->theta() * d / 2.0);
    Vector V = this->v() * sin(this->theta() * d / 2.0);
    ret.b = V(0);
    ret.c = V(1);
    ret.d = V(2);
    return ret;
}

const Quaternion q_exp(const Quaternion &q)
{
    Quaternion ret;
    const Vector V = q.v();
    const double V_norm = V.norm();
    Vector new_V;
    if (V_norm < 1.0e-6)
    {
        new_V = exp(q.a) * V;
    }
    else
        new_V = exp(q.a) * V * sin(V_norm) / V_norm;

    ret.a = exp(q.a) * cos(V_norm);
    ret.b = new_V(0);
    ret.c = new_V(1);
    ret.d = new_V(2);

    return ret;
}

const Quaternion q_log(const Quaternion &q)
{
    Quaternion ret;
    const Vector V = q.v();
    const double V_norm = V.norm();
    Vector new_V;

    new_V = V.normalize() * acos(q.a / q.norm());

    ret.a = log(q.norm());
    ret.b = new_V(0);
    ret.c = new_V(1);
    ret.d = new_V(2);

    return ret;
}

const Matrix Quaternion::rotation_matrix() const
{
    Matrix ret(3, 3);
    ret.set(0, 0, a*a + b*b - c*c - d*d);
    ret.set(0, 1, 2 * (b*c - a*d));
    ret.set(0, 2, 2 * (b*d + a*c));
    ret.set(1, 0, 2 * (b*c + a*d));
    ret.set(1, 1, a*a - b*b + c*c - d*d);
    ret.set(1, 2, 2 * (c*d - a*b));
    ret.set(2, 0, 2 * (b*d - a*c));
    ret.set(2, 1, 2 * (c*d + a*b));
    ret.set(2, 2, a*a - b*b - c*c + d*d);
    
    return ret;
}

double *Quaternion::yaw_pitch_roll() const
{
    const double yaw = atan2(2 * (a * d + b * c), 1 - 2 * (c * c + d * d));

    double pitch;
    const double sinp = 2 * (a * c - d * b);
    if (sinp >= 1)
    {
        pitch = 3.14159265358979323846 / 2; // use 90 degrees if out of range
    }
    else if (sinp <= -1)
    {
        pitch = -3.14159265358979323846 / 2; // use -90 degrees if out of range
    }
    else
    {
        pitch = asin(sinp);
    }

    const double roll = atan2(2 * (a * b + c * d), 1 - 2 * (b * b + c * c));
    static double ret[3] = {yaw, pitch, roll};
    return ret;
}

void Quaternion::set_description(const string &description)
{
    this->description = description;
}

const string Quaternion::to_str() const
{
    string str = this->description + ": ";
    str += "(" + to_string(a) + ", " + to_string(b) + ", " + to_string(c) + ", " + to_string(d) + ")";
    return str;
}