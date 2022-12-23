/*
cree par : RobinAZERTY
version du 29/09/2022
*/

#include "mag2orient.cpp"
#include "..\..\..\../Simulation-dev 0.12/lib_validated/simulation/simulation.cpp"
#include <iostream>
using namespace std;

#define RAD_TO_DEG 57.295779513082320876798154814105
#define DEG_TO_RAD 0.01745329251994329576923690768489

// magnetometer object (as sensor) must be added to the simulation

void visualize()
{
    // create a simulation
    simulation my_simulation;
    my_simulation.set_time_step(0.02);

    // create a scenario
    State default_state;
    Scenario my_scenario = Scenario(default_state);
    OrientationState next_state;
    Vector next_ypr(3);
    next_ypr.set(0, -90 * DEG_TO_RAD);
    next_ypr.set(1, 100 * DEG_TO_RAD);
    next_ypr.set(2, 165 * DEG_TO_RAD);
    next_state.q = Quaternion(next_ypr(0), next_ypr(1), next_ypr(2)); // speed and acceleration are null
    my_scenario.add_BezierOrientationMotion(next_state, 5);          // duration of the motion is 1 second
    my_simulation.set_scenario(my_scenario);

    // create an magnetometer
    magnetometer my_mag;
    const double mag_noise = 0.00;
    const double mag_noise_t[] = {mag_noise, mag_noise, mag_noise};
    my_mag.set_std_noise(mag_noise_t);
    my_simulation.add_sensor(my_mag);

    // create an magnetometer to orientation converter
    mag2orient mag2orient;

    while (my_simulation.get_time() < my_scenario.get_total_duration())
    {

        my_simulation.set_current_frame_data();
        mag2orient.compute(my_mag.get_samples(), my_simulation.get_time_step(),1);
        
        Quaternion q = my_scenario.get_state().orientationState.q;
        Quaternion q2 = mag2orient.get_OrientationState().q;
        Quaternion qd= q.inverse() * q2;

        double q_vel_diff_norm = (my_scenario.get_state().orientationState.q_velocity-mag2orient.get_OrientationState().q_velocity).norm();
        const double q_diff_norm = (q-q2).norm();//acos(q_dot(q,q2)*q_dot(q,q2)*2-1) * RAD_TO_DEG;//

        cout << my_simulation.get_time() << "\t";
        //print sensors data
        //cout << my_mag.get_samples()(0) << "\t" << my_mag.get_samples()(1) << "\t" << my_mag.get_samples()(2) << "\t";
        
        cout << "q_diff_norm = "<<q_diff_norm<< "  ";
        cout << "q = " << q.to_str() << "  ";
        cout << "q2 = " << q2.to_str()<< "  ";
        cout << "q_vel = " << q_vel_diff_norm;
        
        cout<<endl;

        my_simulation.next_frame();
    }
}

int main()
{
    cout << "test" << endl;
    visualize();
    cout << "end" << endl;
    return 0;
}
