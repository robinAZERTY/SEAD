/*
créé par : robinAZERTY
version du : 20/08/2022 - 0
*/
#include "BezierPositionMotion.cpp"

#include <iostream>
using namespace std;

void visualisation()
{   
    cout << "BezierPositionMotion Visualisation" << endl;

    Vector points_3D[4] = {Vector(3),Vector(3),Vector(3),Vector(3)};
    points_3D[0].fill(0);
    points_3D[1].fill(1);
    points_3D[2].fill(2);
    points_3D[3].fill(3);
    BezierPositionMotion bezierPositionMotion(points_3D,1);
    
    //cout << bezierPositionMotion.to_str(); // it is like a constant speed motion lol

    PositionState initialState;
    initialState.position.fill(1);
    initialState.velocity.set(0,0);

    PositionState finalState;
    finalState.position.fill(2);

    BezierPositionMotion bezierPositionMotion2(initialState,finalState,1);
    cout << bezierPositionMotion2.get_state(0).acceleration(0)<<endl;
    cout << bezierPositionMotion2.get_state(0.01).acceleration(0)<<endl;

}

int main()
{
    visualisation();
    return 0;
}