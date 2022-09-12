#include "../..\lib_validated\simulation\simulation.cpp"

Scenario my_scenario;

void construct_my_scenario()
{
    //juste a simple translation
    cout<<"constructing scenario"<<endl;
    State default_state;
    //my_scenario=Scenario(default_state);//initial state
    cout<<"scenario constructed"<<endl;
    
    PositionState next_state;
    Vector next_position(3);
    next_position.set(0,1);
    next_position.set(1,0);
    next_position.set(2,0);
    next_state.position=next_position;//speed and acceleration are null

    my_scenario.add_BezierPositionMotion(next_state,1);//duration of the motion is 1 second
}