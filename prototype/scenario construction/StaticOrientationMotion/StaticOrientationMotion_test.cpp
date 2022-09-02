/*
créé par : robinAZERTY
version du : 20/08/2022
*/

#include "StaticOrientationMotion.cpp"
#include <iostream>
using namespace std;

void visualisation()
{
    OrientationState state;
    state.E_x.set(0, 0.5);
    state.E_x.set(1, -0.56);
    state.E_x.set(2, -1);

    //state.dE_x.set(0,1);// throw "velocity and acceleration vectors must be null";

    StaticOrientationMotion staticMotion(state);
    cout << staticMotion.to_str();

    //heritage

    OrientationMotionBase *motions;
    motions = new OrientationMotionBase[3];
    motions[0]=staticMotion;
    motions[1]=OrientationMotionBase();
    motions[2]=StaticOrientationMotion();

    for(int i=0;i<3;i++)
    {
        cout << motions[i].to_str();
    }

    delete [] motions;

    Quaternion q;
    q.a=0.6;
    q.b=0.8;
    q=q.normalize();

    staticMotion=StaticOrientationMotion(q);
    cout << staticMotion.to_str();
}

int main()
{
    visualisation();
    return 0;
}