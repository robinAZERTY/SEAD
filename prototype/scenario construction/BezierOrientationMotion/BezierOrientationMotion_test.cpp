#include "BezierOrientationMotion.cpp"

#include <iostream>
using namespace std;

#define PI 3.14159265359

void visualization_Slerp()
{
    Quaternion q1 = Quaternion(0, 0, 0);
    Quaternion q2 = Quaternion(0, PI / 2, PI / 2);

    Slerp motion;
    motion.update(q1, q2,0);

    Quaternion q = motion.SLERP();
    q.set_description("q");

    const double ds = 1.0e-3; // 1ms
    for (unsigned int i = 0; i <= 1 / ds; i++)
    {
        const double s = i * ds;
        motion.update(q1,q2,s);
        //q = motion.SLERP();
        q=motion.SLERP();
        const double *ypr= q.yaw_pitch_roll();
        cout <<"s:"<<s;
        //cout<<"\tyaw:"<<ypr[0]<<"\tpitch:"<<ypr[1]<<"\troll:"<<ypr[2]<<endl;
        cout<<q.to_str()<<endl;
    }
}

void visualization_BezierOrientationMotion()
{
    Quaternion q4[4];
    q4[0] = Quaternion(0, 0, 0);
    q4[1] = Quaternion(0, 0, 0);
    q4[2] = Quaternion(0, 0, PI / 2);
    q4[3] = Quaternion(0, 0, PI / 2);

    const double duration = 2.5;

    BezierOrientationMotion motion(q4, duration);

    const double dt = 1.0e-2; // 1ms

    for (unsigned int i = 0; i <= duration / dt; i++)
    {
        const double t = i * dt;
        motion.update_state(t);
        Quaternion a = (motion.q_der*motion.q.conjugate())*2*180/PI;
        a.a=0;
        cout<<"t:"<<to_string(t)<<'\t'<<motion.q.to_str()<<'\t'<<motion.q_der.to_str()<<'\t'<<a.to_str()<<endl;
    }
}
int main()
{
    //visualization_Slerp();
    visualization_BezierOrientationMotion();
    return 0;
}