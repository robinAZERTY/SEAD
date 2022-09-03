



#include <iostream>
using namespace std;

#include "Scenario.cpp"
void visualization(){
    
    BezierPositionMotion b;
    PositionState p0,p1, p2;
    p0.position.set(0,-0.2);
    p1.position.set(0,1);
    p1.velocity.set(2,0.2);
    p2.position.set(0,0);
    Scenario s(p0);
    s.add_BezierPositionMotion(p1,1);
    s.add_BezierPositionMotion(p2,2);

    const double simulation_duration=s.get_total_duration();

    const double dt = 1.0/30;

    for (unsigned int i = 0; i <= simulation_duration / dt; i++)
    {
        const double t = i * dt;
        PositionState current_state;
        current_state=s.get_state(t).positionState;

        cout<<"t ="<<to_string(t);
        cout<<"\t"<<"pos : ("<<to_string(current_state.position(0))<<","<<to_string(current_state.position(1))<<","<<to_string(current_state.position(2))<<")";
        cout<<"\t"<<"vel : ("<<to_string(current_state.velocity(0))<<","<<to_string(current_state.velocity(1))<<","<<to_string(current_state.velocity(2))<<")";
        cout<<"\t"<<"acc : ("<<to_string(current_state.acceleration(0))<<","<<to_string(current_state.acceleration(1))<<","<<to_string(current_state.acceleration(2))<<")";
        cout<<endl;      
    }
}
int main()
{   
    visualization();

    return 0;
}