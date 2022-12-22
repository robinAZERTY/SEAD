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
    my_simu.set_time_step(0.01);
    //generate data in this path
    my_simu.set_write_path("D://documents//github//SEAD//SEAD//Simulation-dev 0.11//exemples//simple_translation_simulation//data.csv");
    my_simu.run();
    my_simu.write_simulation();
    std::cout << "simulation done" << std::endl;
    return 0;
}