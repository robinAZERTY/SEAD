#include "../..\lib_validated\simulation\simulation.cpp"

accelerometer my_acc;
gyroscope my_gyro;
magnetometer my_mag;

#define acc_noise 0.1
#define gyro_noise 0.01
#define mag_noise 0.5

#define acc_resolution 0.012529
#define gyro_resolution 0.006
#define mag_resolution 0.2

#define mag_refresh_delay 0.2



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

    Matrix acc_nonOrtho(3,3);//3 vectors with litlle non orthogonality
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

    Vector gain(3);
    gain.set(0,1.05);
    gain.set(1,0.94);
    gain.set(2,1.01);

    //the average of the 3 gains must be 1
    gain/=(gain(0)+gain(1)+gain(2))/3;
    acc_nonOrtho1*=gain(0);
    acc_nonOrtho2*=gain(1);
    acc_nonOrtho3*=gain(2);

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

    Matrix gyro_nonOrtho(3,3);//3 vectors with litlle non orthogonality
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

    my_mag.set_description("mag1");
    const double mag_noise_t[]={mag_noise,mag_noise,mag_noise};
    my_mag.set_std_noise(mag_noise_t);
    Vector mag_bias(3);
    mag_bias.set(0,5);
    mag_bias.set(1,10);
    mag_bias.set(2,15);

    my_mag.set_offsets(mag_bias);

    Matrix mag_nonOrtho(3,3);//3 vectors with litlle non orthogonality
    Vector mag_nonOrtho1(3);
    mag_nonOrtho1.set(0,1);
    mag_nonOrtho1.set(1,0.051);
    mag_nonOrtho1.set(2,-0.102);
    mag_nonOrtho1=mag_nonOrtho1.normalize();//the new direction of x axis
    Vector mag_nonOrtho2(3);
    mag_nonOrtho2.set(0,-0.001);
    mag_nonOrtho2.set(1,1);
    mag_nonOrtho2.set(2,0.053); 
    mag_nonOrtho2=mag_nonOrtho2.normalize();//the new direction of y axis
    Vector mag_nonOrtho3(3);
    mag_nonOrtho3.set(0,0.202);
    mag_nonOrtho3.set(1,-0.003);
    mag_nonOrtho3.set(2,1);
    mag_nonOrtho3=mag_nonOrtho3.normalize();//the new direction of z axis
    mag_nonOrtho.set(0,0,mag_nonOrtho1(0));
    mag_nonOrtho.set(0,1,mag_nonOrtho1(1));
    mag_nonOrtho.set(0,2,mag_nonOrtho1(2));
    mag_nonOrtho.set(1,0,mag_nonOrtho2(0));
    mag_nonOrtho.set(1,1,mag_nonOrtho2(1));
    mag_nonOrtho.set(1,2,mag_nonOrtho2(2));
    mag_nonOrtho.set(2,0,mag_nonOrtho3(0));
    mag_nonOrtho.set(2,1,mag_nonOrtho3(1));
    mag_nonOrtho.set(2,2,mag_nonOrtho3(2));
    my_mag.set_nonOrthogonality(mag_nonOrtho);

    const double mag_resolution_t[]={mag_resolution,mag_resolution,mag_resolution};
    my_mag.set_resolution(mag_resolution_t);
    const double mag_refresh_delay_t[]={mag_refresh_delay,mag_refresh_delay,mag_refresh_delay};
    my_mag.set_refresh_delay(mag_refresh_delay_t);

    
}