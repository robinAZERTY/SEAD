#include "BezierOrientationMotion.cpp"

#include <iostream>
using namespace std;

#define PI 3.14159265359

void visualization()
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

    const double ds = 1.0e-3;//1ms
    for (unsigned int i = 0; i <= 1 / ds; i++)
    {
        const double s = i * ds;
        motion.update_s(s);
        q = motion.SLERP();
        q_dif = (q - q_prev) / ds;
        q_prev = q;
        q_der = motion.PRIME2();
        q_err = q_der - q_dif;
        cout << q.to_str() << '\t';
        cout << q_der.to_str() << '\t';
        cout << q_dif.to_str() << '\t';
        cout << "error : " << q_err.norm() << endl;

        motion.PRIME2();
    }
}

int main()
{
    visualization();
    return 0;
}