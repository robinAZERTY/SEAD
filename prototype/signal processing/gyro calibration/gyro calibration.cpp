/*
cree par : robinAZERTY
version du 13/10/2022
*/

#ifndef false//if it is validated
#include "../../..\validated\tools\ellipsoidFitting\ellipsoidFitting.cpp"
#else
#include "..\tools\ellipsoidFitting\ellipsoidFitting.cpp"
#endif

//the goal of this program is to find the best bias from a set of gyroscope MEMS data
class GyroCalibration
{
    public:
        //constructor
        GyroCalibration();
        ~GyroCalibration();
    Vector bias;

    void process(const Vector &newSamples);//add new samples to the calibration
    void reset();//reset the calibration
    private:
    unsigned int count;
};


GyroCalibration::GyroCalibration()
{
    bias = Vector(3);
    count = 0;
}

GyroCalibration::~GyroCalibration()
{
}

void GyroCalibration::process(const Vector &newSamples)
{
    bias = (bias*count+newSamples)/(count+1);//update the bias
    count++;
}

void GyroCalibration::reset()
{
    bias = Vector(3);
    count = 0;
}