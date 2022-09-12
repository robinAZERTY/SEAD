#include "../..\lib_validated\simulation\simulation.cpp"

accelerometer my_acc;
gyroscope my_gyro;

#define acc_noise 0.1
#define gyro_noise 0.01

#define acc_resolution 0.012529
#define gyro_resolution 0.006



void init_sensor_unit()
{
    my_acc.set_description("acc1");

    const double acc_noise_t[]={acc_noise,acc_noise,acc_noise};
    my_acc.set_std_noise(acc_noise_t);

    Vector acc_bias(3);
    acc_bias.set(0,0.11);
    acc_bias.set(1,0.05);
    acc_bias.set(2,-0.12);

    my_acc.set_offsets(acc_bias);

    Matrix acc_nonOrtho(3,3);//3 unit vectors with litlle non orthogonality but keeping norm 1
    Vector acc_nonOrtho1(3);
    acc_nonOrtho1.set(0,1);
    acc_nonOrtho1.set(1,0.01);
    acc_nonOrtho1.set(2,0.01);
    acc_nonOrtho1=acc_nonOrtho1.normalize();//the new direction of x axis
    Vector acc_nonOrtho2(3);
    acc_nonOrtho2.set(0,-0.01);
    acc_nonOrtho2.set(1,1);
    acc_nonOrtho2.set(2,0.01);
    acc_nonOrtho2=acc_nonOrtho2.normalize();//the new direction of y axis
    Vector acc_nonOrtho3(3);
    acc_nonOrtho3.set(0,-0.01);
    acc_nonOrtho3.set(1,-0.01);
    acc_nonOrtho3.set(2,1);
    acc_nonOrtho3=acc_nonOrtho3.normalize();//the new direction of z axis
    acc_nonOrtho.set(0,0,acc_nonOrtho1(0));
    acc_nonOrtho.set(0,1,acc_nonOrtho1(1));
    acc_nonOrtho.set(0,2,acc_nonOrtho1(2));
    acc_nonOrtho.set(1,0,acc_nonOrtho2(0));
    acc_nonOrtho.set(1,1,acc_nonOrtho2(1));
    acc_nonOrtho.set(1,2,acc_nonOrtho2(2));
    acc_nonOrtho.set(2,0,acc_nonOrtho3(0));
    acc_nonOrtho.set(2,1,acc_nonOrtho3(1));
    acc_nonOrtho.set(2,2,acc_nonOrtho3(2));

    my_acc.set_nonOrthogonality(acc_nonOrtho);

    const double acc_resolution_t[]={acc_resolution,acc_resolution,acc_resolution};
    my_acc.set_resolution(acc_resolution_t);
    
    my_gyro.set_description("gyro1");

    const double gyro_noise_t[]={gyro_noise,gyro_noise,gyro_noise};
    my_gyro.set_std_noise(gyro_noise_t);

    Vector gyro_bias(3);
    gyro_bias.set(0,0.001);
    gyro_bias.set(1,0.002);
    gyro_bias.set(2,-0.003);

    my_gyro.set_offsets(gyro_bias);

    Matrix gyro_nonOrtho(3,3);//3 unit vectors with litlle non orthogonality but keeping norm 1
    Vector gyro_nonOrtho1(3);
    gyro_nonOrtho1.set(0,1);
    gyro_nonOrtho1.set(1,0.001);
    gyro_nonOrtho1.set(2,-0.002);
    gyro_nonOrtho1=gyro_nonOrtho1.normalize();//the new direction of x axis
    Vector gyro_nonOrtho2(3);
    gyro_nonOrtho2.set(0,-0.001);
    gyro_nonOrtho2.set(1,1);
    gyro_nonOrtho2.set(2,0.003);
    gyro_nonOrtho2=gyro_nonOrtho2.normalize();//the new direction of y axis
    Vector gyro_nonOrtho3(3);
    gyro_nonOrtho3.set(0,0.002);
    gyro_nonOrtho3.set(1,-0.003);
    gyro_nonOrtho3.set(2,1);
    gyro_nonOrtho3=gyro_nonOrtho3.normalize();//the new direction of z axis
    gyro_nonOrtho.set(0,0,gyro_nonOrtho1(0));
    gyro_nonOrtho.set(0,1,gyro_nonOrtho1(1));
    gyro_nonOrtho.set(0,2,gyro_nonOrtho1(2));
    gyro_nonOrtho.set(1,0,gyro_nonOrtho2(0));
    gyro_nonOrtho.set(1,1,gyro_nonOrtho2(1));
    gyro_nonOrtho.set(1,2,gyro_nonOrtho2(2));
    gyro_nonOrtho.set(2,0,gyro_nonOrtho3(0));
    gyro_nonOrtho.set(2,1,gyro_nonOrtho3(1));
    gyro_nonOrtho.set(2,2,gyro_nonOrtho3(2));

    my_gyro.set_nonOrthogonality(gyro_nonOrtho);

    const double gyro_resolution_t[]={gyro_resolution,gyro_resolution,gyro_resolution};
    my_gyro.set_resolution(gyro_resolution_t);
}