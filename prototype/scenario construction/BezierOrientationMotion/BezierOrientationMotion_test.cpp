#include "BezierOrientationMotion.cpp"

#include <iostream>
using namespace std;

#define PI 3.14159265359

void visualization_Slerp()
{
    Quaternion q1 = Quaternion(0, 0, 0);
    Quaternion q2 = Quaternion(0, PI / 2, PI / 2);

    Slerp motion;
    motion.update_q(q1, q2);

    Quaternion q = motion.SLERP(), q_prev = q, q_dif, q_der, q_err;
    q.set_description("q");
    q_der.set_description("q_der");
    q_dif.set_description("q_dif");
    q_err.set_description("q_err");

    const double ds = 1.0e-3; // 1ms
    for (unsigned int i = 0; i <= 1 / ds; i++)
    {
        const double s = i * ds;
        motion.update_s(s);
        q = motion.SLERP();
        q_dif = (q - q_prev) / ds;
        q_prev = q;
        q_der = motion.PRIME();
        q_err = q_der - q_dif;
        cout << q.to_str() << '\t';
        cout << q_der.to_str() << '\t';
        cout << q_dif.to_str() << '\t';
        cout << "error : " << q_err.norm() << endl;
    }
}

void visualization_BezierOrientationMotion()
{
    Quaternion q4[4];
    q4[0] = Quaternion(0, 0, 0);
    q4[1] = Quaternion(0, 0, 0);
    q4[2] = Quaternion(0, 0, PI / 2);
    q4[3] = Quaternion(0, 0, PI / 2);

    BezierOrientationMotion motion(q4, 1);
    Quaternion q = motion.q, q_prev = q, q_dif, q_der, q_err;
    q.set_description("q");
    q_der.set_description("q_der");
    q_dif.set_description("q_dif");
    q_err.set_description("q_err");

    const double ds = 1.0e-2; // 1ms
    for (unsigned int i = 0; i <= 1 / ds; i++)
    {
        const double s = i * ds;

        Quaternion q_m, q_p;
        if (s > 0 && s < 1)
            motion.get_state(s - ds / 2);
        if (s > 0 && s < 1)
            q_m = motion.q;
        if (s > 0 && s < 1)
            motion.get_state(s + ds / 2);
        if (s > 0 && s < 1)
            q_p = motion.q;
        if (s > 0 && s < 1)
            q_dif = (q_p * q_m.inverse());
        if (s > 0 && s < 1)
            q_dif = q_dif;

        motion.get_state(s);

        q = motion.q;
        q_der = motion.q_der;
        q_err = q_der - q_dif;
        cout << q.to_str() << '\t';
        cout << (q.conjugate() * q_der * 2).to_str() << '\t';//omega
        cout << q_der.to_str() << '\t';
        if (s > 0 && s < 1)
            cout << q_dif.to_str() << '\t';
        if (s > 0 && s < 1)
            cout << "error : " << q_err.norm() << '\t';
        cout << "t : " << s << endl;
    }
}

int main()
{
    // visualization_Slerp();
    visualization_BezierOrientationMotion();
    return 0;
}