#include "gyroscope.cpp"
//include scenario in validated version
#include "..\..\../lib_validated/scenario construction/Scenario/scenario.cpp"

#include <iostream>
using namespace std;

int main()
{
    gyroscope s;
    cout << "Hello World!" << endl;
    const double std_noise[3] = {0.09, 0.1, 0.1};
    const double resolution[3] = {0, 0, 0};
    s.set_std_noise(std_noise);
    s.set_resolution(resolution);
    Vector v(3);
    v.fill(0);
    
    int n = 10 * 30;
    for (float time = 0; time<1; time+=0.01)
    {
        double new_value = s.get_samples(v,time)(0);

        cout << new_value << endl;
    }

    return 0;
}