#include "complementary_filter.cpp"

#include <iostream>
using namespace std;

//add simulation code here
#include "../../..\Simulation-dev 0.1\lib_validated\simulation\simulation.cpp"

//set simulation parameters
/*
Scenario my_scenario;

void construct_my_scenario()
{
    //juste a static state
    State default_state;
    my_scenario=Scenario(default_state);//initial state    
    my_scenario.add_BezierPositionMotion(default_state.positionState,200);//duration of the motion is 1 second
}

//set sensors parameters (1 accelerometer and 1 gyroscope)
accelerometer my_acc;
gyroscope my_gyro;

#define acc_noise 0.1
#define gyro_noise 0.01

void set_sensors()
{   
    const double acc_noise_t[]={acc_noise,acc_noise,acc_noise};
    my_acc.set_std_noise(acc_noise_t);
    const double gyro_noise_t[]={gyro_noise,gyro_noise,gyro_noise};
    my_gyro.set_std_noise(gyro_noise_t);
}

#define cut_off_frequency 0.1
#define sampling_frequency 100
//set complementary filter parameters
complementary_filter my_filter;

void set_filter()
{
    my_filter=complementary_filter(cut_off_frequency,sampling_frequency);
}

const double get_acc_angle(const double acc[3])
{
    return atan2(acc[1],acc[2]);
}

//integration of the gyroscope
double gyro_angle=0;
const double integrate_gyro(const double gyro[3],const double dt)
{   
    gyro_angle+=gyro[0]*dt;
    return gyro_angle;
}

const double get_gyro_angle(const double gyro[3])
{
    return integrate_gyro(gyro,1.0/sampling_frequency);
}

void visualize()
{
    //set the simulation parameters
    construct_my_scenario();
    //set the sensors parameters
    set_sensors();
    //set the filter parameters
    set_filter();

    cout<<"cut_off_frequency="<<cut_off_frequency<<endl;
    cout<<"sampling_frequency="<<sampling_frequency<<endl;
    cout<<"acc_noise="<<acc_noise<<endl;
    cout<<"gyro_noise="<<gyro_noise<<endl;

    //simulation


}
*/
int main()
{
    //visualize();
    return 0;
}
