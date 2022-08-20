/*
créé par : robinAZERTY
version du : 20/08/2022 - 0
*/
#include "BezierPositionMotion.cpp"

#include <iostream>
using namespace std;

void visualisation()
{
    Vector points_3D[4] = {Vector(3),Vector(3),Vector(3),Vector(3)};
    points_3D[0].fill(0);
    points_3D[1].fill(1);
    points_3D[2].fill(2);
    points_3D[3].fill(3);
    BezierPositionMotion bezierPositionMotion(points_3D,1);
    bezierPositionMotion.get_final_state(0.5);

}