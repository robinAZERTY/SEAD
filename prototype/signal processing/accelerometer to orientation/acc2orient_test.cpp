/*
cree par : RobinAZERTY
version du 28/09/2022
*/

#include "acc2orient.cpp"
#include "../../..\Simulation-dev 0.11/lib_validated\simulation\simulation.cpp"
#include <iostream>
using namespace std;

#define RAD_TO_DEG 57.295779513082320876798154814105
#define DEG_TO_RAD 0.01745329251994329576923690768489

void visualize()
{
    // create a simulation
    simulation my_simulation;
    my_simulation.set_time_step(0.05);

    // create a scenario
    State default_state;
    Scenario my_scenario = Scenario(default_state);
    OrientationState next_state;
    Vector next_ypr(3);
    next_ypr.set(0, 0);
    next_ypr.set(1, 0);
    next_ypr.set(2, 180 * DEG_TO_RAD);
    next_state.q = Quaternion(next_ypr(0), next_ypr(1), next_ypr(2)); // speed and acceleration are null
    my_scenario.add_BezierOrientationMotion(next_state, 5);          // duration of the motion is 1 second
    my_simulation.set_scenario(my_scenario);

    // create an accelerometer
    accelerometer my_acc;
    const double acc_noise = 0.05;
    const double acc_noise_t[] = {acc_noise, acc_noise, acc_noise};
    my_acc.set_std_noise(acc_noise_t);
    my_simulation.add_sensor(my_acc);

    // create a accelerometer to orientation converter
    acc2orient acc2orient;

    while (my_simulation.get_time() < my_scenario.get_total_duration())
    {
        my_simulation.next_frame();

        my_simulation.set_current_frame_data();
        acc2orient.compute(my_acc.get_samples(), my_simulation.get_time_step());
        cout << my_simulation.get_time() << "\t";
        Quaternion q = my_scenario.get_state().orientationState.q;
        Quaternion q2 = acc2orient.get_OrientationState().q;
        Quaternion qd= q.inverse() * q2;
        const double angular_diff = acos(q_dot(q,q2)*q_dot(q,q2)*2-1) * RAD_TO_DEG;
        cout << "angular diff = "<<angular_diff<< "\t";
        cout << "q = " << q.to_str() << "\t";
        cout << "q2 = " << q2.to_str();
        cout<<endl;
    }
}

int main()
{
    cout << "test" << endl;
    visualize();
    cout << "end" << endl;
    return 0;
}