#include "sensor.cpp"
//#include "../accelerometer/accelerometer.cpp"
//#include "../gyroscope/gyroscope.cpp"

#include <iostream>
using namespace std;

void simu()
{
    
    sensor s;
    cout << "Hello World!" << endl;
    const double std_noise[3] = {2, 0.1, 0.1};
    const double resolution[3] = {0.01, 0.01, 0.01};
    const double refresh_delay[3] = {0.03, 0.02, 0.01};
    s.set_std_noise(std_noise);
    s.set_resolution(resolution);
    s.set_refresh_delay(refresh_delay);
    Vector v(3);
    v.fill(125.364);
    double average = 0;
    double standard_deviation = 0;
    int n = 10000;
    for (float time = 0; time < 1; time+=0.01)
    {
        const Vector& new_value = s.get_samples(v,time);
        cout << "time: " << time << " x: " << new_value(0) << " y: " << new_value(1) << " z: " << new_value(2) << endl;
    }
    
}

#include <typeinfo>
/*
void dynamic_type()
{
    
    accelerometer acc;

    sensor* s = &acc;

    sensor* s2 = new sensor[2];
    s2[0]=acc;

    const type_info& ti1 = typeid(acc);
    const type_info& ti2 = typeid(*s);
    const type_info& ti3 = typeid(s2[0]);
    //convert s2[0] to accelerometer
    accelerometer* acc2 = dynamic_cast<accelerometer*>(&s2[0]);
    //we can use acc2 as an accelerometer and use its methods
    const type_info& ti4 = typeid(*acc2);


    cout<<ti1.name()<<endl;
    cout<<ti2.name()<<endl;
    cout<<ti3.name()<<endl;
    cout<<ti4.name()<<endl;
    delete s,acc;
    delete[] s2;
}

void dynamic_type_bis()
{
    accelerometer acc;
    gyroscope gyro;
    sensor ssensor;


    sensor** s = new sensor*[3];
    s[0]=&acc;
    s[1]=&gyro;
    s[2]=&ssensor;

    for (int i = 0; i < 3; i++)
    {   cout<<i<<'\t';
        const type_info& t = typeid(*s[i]);
        if(t==typeid(accelerometer))
        {
            accelerometer* this_senror = dynamic_cast<accelerometer*>(s[i]);
            cout<<"this is an accelerometer"<<endl;
        }
        else if(t==typeid(gyroscope))
        {
            gyroscope* this_senror = dynamic_cast<gyroscope*>(s[i]);
            cout<<"this is a gyroscope"<<endl;
        }
        else if(t==typeid(sensor))
        {
            sensor* this_senror = dynamic_cast<sensor*>(s[i]);
            cout<<"this is a sensor"<<endl;
        }
    }
    for (int i = 0; i < 3; i++)
    {
        delete s[i];
    }
}
*/
int main()
{
    //dynamic_type_bis();
    simu();
    return 0;
}