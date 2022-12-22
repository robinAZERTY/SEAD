#include "magnetometer.cpp"

#include <iostream>
using namespace std;

int main()
{
    magnetometer s;
    cout << "Hello World!" << endl;
    const double std_noise[3] = {0.09, 0.1, 0.1};
    s.set_std_noise(std_noise);
    s.set_resolution(std_noise);
    State v;
    int n = 10*30;
    for (float time = 0; time < 1; time+=0.01)
    {   
        double new_value = s.measure(v,time)(0);

        cout << new_value << endl;
    }


    return 0;
}