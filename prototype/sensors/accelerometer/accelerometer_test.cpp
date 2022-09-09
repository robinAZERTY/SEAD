#include "accelerometer.cpp"

#include <iostream>
using namespace std;

int main()
{
    accelerometer s;
    cout << "Hello World!" << endl;
    const double std_noise[3] = {0.09, 0.1, 0.1};
    s.set_std_noise(std_noise);
    s.set_resolution(std_noise);
    Vector v(3);
    v.fill(12.364);
    double average = 0;
    double standard_deviation = 0;
    int n = 10*30;
    for (int i = 0; i < n; i++)
    {   
        double new_value = s.get_samples(v)(0);
        average += new_value;
        standard_deviation += pow(new_value, 2);
        cout << new_value << endl;
    }
    standard_deviation = sqrt(standard_deviation / n - pow(average / n, 2));
    average /= n;
    cout << "average: " << average << endl;
    cout << "standard_deviation: " << standard_deviation << endl;

    return 0;
}