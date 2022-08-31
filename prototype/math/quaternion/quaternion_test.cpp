#include "quaternion.cpp"

#include <iostream>
using namespace std;

#define PI 3.14159265358979323846

void test_cons_dest()
{   
    cout << "test_cons_dest" << endl;
    Quaternion q0;
    q0.set_description("q0");
    cout << q0.to_str() << endl;

    Quaternion q1(q0);
    q1.set_description("q1");
    cout << q1.to_str() << endl;

    Quaternion q2(PI / 2, 0, 0);
    q2.set_description("q2");
    cout << q2.to_str() << endl;

    Quaternion q3(PI/2, 1, 1, 1);
    q3.set_description("q3");
    cout << q3.to_str() << endl << endl;
}

void test_operator()
{
    cout << "test_operator" << endl;

    Quaternion q0(PI / 2, 0, 0);
    q0.set_description("q0");
    cout << q0.to_str() << endl;

    Quaternion q1(-PI / 2,0,0);
    q1.set_description("q1");
    cout << q1.to_str() << endl;

    Quaternion q=q0+q1;
    q.set_description("q0+q1");
    cout << q.to_str() << endl;

    q=q0-q1;
    q.set_description("q0-q1");
    cout << q.to_str() << endl;

    q=q0*q1;
    q.set_description("q0*q1");
    cout << q.to_str() << endl;

    q=q0*2;
    q.set_description("q0*2");
    cout << q.to_str() << endl;

    q=q0/2;
    q.set_description("q0/2");
    cout << q.to_str() << endl;

    bool b=q0==q1;
    cout << "q0==q1:" << b << endl;
    b=q0!=q1;
    cout << "q0!=q1:" << b << endl;

    b=q0==q0;
    cout << "q0==q0:" << b << endl;
    b=q0!=q0;
    cout << "q0!=q0:" << b << endl << endl;
}

void test_function()
{
    Quaternion q0(-0.5, 1.5, 0.5);
    q0.set_description("q0");
    cout << q0.to_str() << endl;
    cout << "q0.norm():" << q0.norm() << endl;
    cout << "q0.conjugate():" << q0.conjugate().to_str() << endl;
    cout << "q0.inverse():" << q0.inverse().to_str() << endl << endl;


    // yaw pitch roll quaternion conversion.
    const double *ypr;
    ypr=q0.yaw_pitch_roll();
    cout << "q0.yaw_pitch_roll():" <<ypr[0]<<" "<<ypr[1]<<" "<<ypr[2]<<endl;

    q0=Quaternion(1,1,1,1);

    
    //cout<<Vector(3).to_str();
    cout<<q0.v().to_str()<<endl;
    cout<<q0.theta()<<endl;

}

int main()
{   
    test_cons_dest();
    test_operator();
    test_function();
    return 0;
}