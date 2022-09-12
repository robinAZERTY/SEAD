#include "sensor_unit.h"
#include "scenario.h"

int main()
{   
    simulation my_simu;
    cout << "Hello World!" << endl;
    construct_my_scenario();
    cout<<"scenario constructed"<<endl;
    init_sensor_unit();
    cout << "sensor_unit initialized" << endl;
    my_simu.add_sensor(my_acc);
    cout << "my_acc added" << endl;
    //my_simu.add_sensor(my_gyro);
    //cout << "my_gyro added" << endl;
    my_simu.set_scenario(&my_scenario);
    cout << "scenario added" << endl;
    my_simu.set_time_step(0.01);//time step of the simulation is 0.01 second
    cout << "time step set" << endl;
    //generate data in this path
    my_simu.set_write_path("D://documents//github//SEAD//exemples//simple_translation//data.csv");
    cout<<"running simulation"<<endl;
    my_simu.run();
    cout<<"simulation finished, writting..."<<endl;
    my_simu.write_simulation();
    return 0;
}