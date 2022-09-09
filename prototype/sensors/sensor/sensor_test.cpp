#include "sensor.cpp"

#include <iostream>
using namespace std;

int main()
{
    sensor s;
    cout << "Hello World!" << endl;
    const double std_noise[3] = {2, 0.1, 0.1};
    s.set_std_noise(std_noise);
    Vector v(3);
    v.fill(125.364);
    double average = 0;
    double standard_deviation = 0;
    int n = 10000;
    for (int i = 0; i < n; i++)
    {   
        average += s.get_samples(v)(0);
        standard_deviation += pow(s.get_samples(v)(0), 2);
        cout << s.get_samples(v)(0) << endl;
    }
    standard_deviation = sqrt(standard_deviation / n - pow(average / n, 2));
    average /= n;
    cout << "average: " << average << endl;
    cout << "standard_deviation: " << standard_deviation << endl;

    return 0;
}