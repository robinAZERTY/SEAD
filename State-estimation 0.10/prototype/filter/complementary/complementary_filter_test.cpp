#include "complementary_filter.cpp"
#include <math.h>
#include <iostream>
using namespace std;

const double gaussien_noise(const double &tau)
{   
    if (tau == 0)
        return 0;
    int rand1 = rand() % 1001;
    int rand2 = rand() % 1001;
    double noise = tau * sqrt(-2 * log(rand1 / 1000.0)) * cos(2 * 3.14159 * rand2 / 1000.0);
    if (rand1==0)
    {
        return gaussien_noise(tau);
    }
    else
    {
        return noise;
    }
}



const double acc_angle()
{   
    double accx = 0.0 + gaussien_noise(0.1);
    double accz = -9.81 + gaussien_noise(0.1);
    return atan2(accx, -accz) * 180 / 3.14159;
}

double angle = 0.0;
double gyr_angle = 0.0;
const double bias= 0.1;
const double gyro_angle(const double &dt)
{   
    double gyro = bias + gaussien_noise(0.1);
    gyr_angle += gyro * dt;
    return gyr_angle;
}

complementary_filter myF;

void visualize()
{
    //set the filter
    const double fs = 100.0;
    const double dt = 1.0 / fs;
    const double fc = 0.1;
    myF.compute_coefficients(fc,fs);


    double t = 0.0;
    while (t < 1.0)
    {
        double acc = acc_angle();
        double gyro = gyro_angle(dt);
        myF.compute(gyr_angle, acc);
        angle = myF.get_output();
        cout << t << " " << acc << " " << gyr_angle << " " << angle << endl;
        t += dt;
    }
}

int main()
{   
    cout<<"time acc gyro angle"<<endl;
    visualize();
    cout<<"end"<<endl;
    return 0;
}
