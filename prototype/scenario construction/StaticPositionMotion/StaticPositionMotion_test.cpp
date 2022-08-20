/*
créé par : robinAZERTY
version du : 20/08/2022
*/

#include "StaticPositionMotion.cpp"
#include <iostream>
using namespace std;

void visualisation()
{
    PositionState state;
    state.position.set(0, 0.5);
    state.position.set(1, -0.56);
    state.position.set(2, -1);

    // state.velocity.set(0,1);// throw "velocity and acceleration vectors must be null";

    StaticPositionMotion staticMotion(state);
    cout << staticMotion.to_str();

    //heritage

    PositionMotionBase *motions;
    motions = new PositionMotionBase[3];
    motions[0]=staticMotion;
    motions[1]=PositionMotionBase();
    motions[2]=StaticPositionMotion();

    for(int i=0;i<3;i++)
    {
        cout << motions[i].to_str();
    }

    delete [] motions;
}

int main()
{
    visualisation();
    return 0;
}