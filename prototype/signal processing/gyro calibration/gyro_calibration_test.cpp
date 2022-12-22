/*
cree par : robinAZERTY
version du : 13/10/2022
*/

#include "gyro calibration.cpp"


void visu()
{
    GyroCalibration gyroCalibration;

    Vector trueBias = Vector(3);
    trueBias.set(0,0.1);
    trueBias.set(1,0.2);
    trueBias.set(2,0.3);
    const double noise = 0.1;
    const unsigned int nbSamples = 100;
    Vector samples = Vector(3);
    for(unsigned int i=0;i<nbSamples;i++)
    {
        samples.set(0,noise*rand()/RAND_MAX-0.5*noise);
        samples.set(1,noise*rand()/RAND_MAX-0.5*noise);
        samples.set(2,noise*rand()/RAND_MAX-0.5*noise);
        samples = samples+trueBias;
        gyroCalibration.process(samples);
    }
    std::cout<<"true bias : "<<trueBias.to_str()<<std::endl;
    std::cout<<"estimated bias : "<<gyroCalibration.bias.to_str()<<std::endl;
    std::cout<<"estimated standard deviation : "<<gyroCalibration.stdDev.to_str()<<std::endl;

}
int main()
{
    visu();
    return 0;
}