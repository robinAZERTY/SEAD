#include "simulation.cpp"
#include <iostream>
using namespace std;

Scenario buid_scenar()
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
    //s0.orientationState=o0;

    Scenario s(s0);

    s.add_BezierPositionMotion(p1,1);
    s.add_BezierPositionMotion(p2,2);

    s.add_BezierOrientationMotion(o1,5);
    s.add_BezierOrientationMotion(o2,10);
    return s;
}


void visu()
{
    simulation simu;
    Scenario s=buid_scenar();
    simu.set_scenario(s);
    accelerometer acc;
    simu.add_sensor(acc);
    simu.set_time_step(0.01);
    simu.run();
    simu.write_simulation();
}

int main()
{   
    visu();
    return 0;
}