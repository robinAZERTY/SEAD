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
    double dt=0.001;

    while(time<=my_scenario.get_total_duration())
    {
        my_simu.set_time(time);
        my_simu.set_current_frame_data();
        //now we can use the fresh sensor data to do some computation
        cout<<"time: "<<time<<"\t"<<"accX : "<<my_acc.get_samples()(0)<<endl;

        //dt=0.01;//we can change the time step here to simulate the real time
        time+=dt;
    }
    cout<<"simulation done"<<endl;
    return 0;
}