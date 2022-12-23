#include "OrientationMotionBase.cpp"

#include <iostream>
using namespace std;

void visualisation()
{   cout<<"visualisation"<<endl;
    OrientationMotionBase motion;
    cout<<motion.to_str()<<endl;
}

int main()
{   
    visualisation();
    return 0;
}