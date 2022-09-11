#include "sensor.cpp"
#include "../accelerometer/accelerometer.cpp"
#include "../gyroscope/gyroscope.cpp"

#include <iostream>
using namespace std;

void simu()
{
    /*
    sensor s;
    cout << "Hello World!" << endl;
    const double std_noise[3] = {2, 0.1, 0.1};
    s.set_std_noise(std_noise);
    Vector v(3);
    v.fill(125.364);
    double average = 0;
    double standard_deviation = 0;
    int n = 10000;
    for (int i = 0; i < n; i++)
    {
        average += s.get_samples(v)(0);
        standard_deviation += pow(s.get_samples(v)(0), 2);
        cout << s.get_samples(v)(0) << endl;
    }
    standard_deviation = sqrt(standard_deviation / n - pow(average / n, 2));
    average /= n;
    cout << "average: " << average << endl;
    cout << "standard_deviation: " << standard_deviation << endl;
    */
}


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

int main()
{
    dynamic_type_bis();
    return 0;
}