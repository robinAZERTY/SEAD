#include "sensor_unit.h"
#include "scenario.h"
#include<iostream>


int main()
{   
    simulation my_simu;
    construct_my_scenario();
    init_sensor_unit();
    my_simu.add_sensor(my_acc);
    my_simu.add_sensor(my_acc);
    my_simu.add_sensor(my_gyro);
    my_simu.set_scenario(my_scenario);
    
    //each step, we update the sensors to do some computation with.
    double time=0;
    double dt=0.01;

    while(time<my_scenario.get_total_duration())
    {
        dt=0.01;
        my_simu.set_time_step(dt);
        my_simu.next_frame();
    }
    return 0;
}