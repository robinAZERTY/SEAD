#include "gyroscope.cpp"
//include scenario in validated version
#include "..\..\../lib_validated/scenario construction/Scenario/scenario.cpp"

#include <iostream>
using namespace std;

const Scenario buid_scenar()
{
//construct a scenario
    BezierPositionMotion b;
    PositionState p0,p1, p2;
    p0.position.set(0,-0.2);
    p1.position.set(0,1);
    p1.velocity.set(2,0.2);
    p2.position.set(0,-2);
    
    

    OrientationState o0,o1,o2;
    o0.q=Quaternion(0,0,0);
    o1.q=Quaternion(0,0,PI/2);
    //o1.q_velocity=Quaternion(0,0,o1.q.yaw_pitch_roll()[2])-Quaternion(0,0,o1.q.yaw_pitch_roll()[2]+0.1);

    State s0;
    s0.positionState=p0;
    s0.orientationState=o0;

    Scenario s(s0);

    s.add_BezierPositionMotion(p1,1);
    s.add_BezierPositionMotion(p2,2);

    s.add_BezierOrientationMotion(o1,5);
    s.add_BezierOrientationMotion(o2,1);
    return s;
}

void test_gyroscope()
{
    gyroscope gyro;
    
      

}

int main()
{
    gyroscope s;
    cout << "Hello World!" << endl;
    const double std_noise[3] = {0.09, 0.1, 0.1};
    const double resolution[3] = {0, 0, 0};
    s.set_std_noise(std_noise);
    s.set_resolution(resolution);
    
    int n = 10 * 30;
    for (int i = 0; i < n; i++)
    {
        double new_value = s.get_samples(v)(0);

        cout << new_value << endl;
    }

    return 0;
}