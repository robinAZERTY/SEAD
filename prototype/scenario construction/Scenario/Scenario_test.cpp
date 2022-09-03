



#include <iostream>
using namespace std;
#define PI 3.14

#include "Scenario.cpp"
void visualization(){
    
    BezierPositionMotion b;
    PositionState p0,p1, p2;
    p0.position.set(0,-0.2);
    p1.position.set(0,1);
    p1.velocity.set(2,0.2);
    p2.position.set(0,-2);
    
    

    OrientationState o0,o1,o2;
    o0.q=Quaternion(0,0,0);
    o1.q=Quaternion(0,0,PI/2);
    o1.q_velocity=Quaternion(0,0,o1.q.yaw_pitch_roll()[2])-Quaternion(0,0,o1.q.yaw_pitch_roll()[2]+0.1);

    State s0;
    s0.positionState=p0;
    s0.orientationState=o0;

    Scenario s(s0);

    s.add_BezierPositionMotion(p1,1);
    s.add_BezierPositionMotion(p2,2);

    s.add_BezierOrientationMotion(o1,10);

    const double simulation_duration=s.get_total_duration();

    const double dt = 1.0/100;

    for (unsigned int i = 0; i <= simulation_duration / dt; i++)
    {
        const double t = i * dt;
        State current_state;
        current_state=s.get_state(t);
        

        cout<<"t ="<<to_string(t);
        cout<<"\t"<<"pos : ("<<to_string(current_state.positionState.position(0))<<","<<to_string(current_state.positionState.position(1))<<","<<to_string(current_state.positionState.position(2))<<")";
        cout<<"\t"<<"vel : ("<<to_string(current_state.positionState.velocity(0))<<","<<to_string(current_state.positionState.velocity(1))<<","<<to_string(current_state.positionState.velocity(2))<<")";
        cout<<"\t"<<"acc : ("<<to_string(current_state.positionState.acceleration(0))<<","<<to_string(current_state.positionState.acceleration(1))<<","<<to_string(current_state.positionState.acceleration(2))<<")";
        cout<<"\t"<<current_state.orientationState.q.to_str()<<"\t"<<current_state.orientationState.q_velocity.to_str();
        cout<<endl;      
    }
}
int main()
{   
    visualization();

    return 0;
}