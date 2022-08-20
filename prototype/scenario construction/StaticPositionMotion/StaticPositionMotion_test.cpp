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
}

int main()
{
    visualisation();
    return 0;
}