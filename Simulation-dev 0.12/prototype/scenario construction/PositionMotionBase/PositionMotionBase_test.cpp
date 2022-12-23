#include "PositionMotionBase.cpp"

#include <iostream>
using namespace std;

void visualisation()
{   cout<<"visualisation"<<endl;
    PositionMotionBase motion;
    cout<<motion.to_str()<<endl;
}

int main()
{   
    visualisation();
    return 0;
}