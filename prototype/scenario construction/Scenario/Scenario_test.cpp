



#include <iostream>
using namespace std;

#include "Scenario.cpp"
void visualization(){
    
    BezierPositionMotion b;
    PositionState p0,p1, p2;
    p0.position.set(0,0.1);
    p1.position.set(0,1);
    p2.position.set(0,0.);
    Scenario s(p0);
    s.add_BezierPositionMotion(p2,1);
    s.add_BezierPositionMotion(p2,2);
    s.get_state(0.5);
    //cout<<s.get_state(0.5).positionState.to_str();
}
int main()
{   
    visualization();
    return 0;
}