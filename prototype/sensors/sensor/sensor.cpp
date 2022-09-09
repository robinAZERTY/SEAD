#include "sensor.h"
#define M_PI 3.14159265359

sensor::sensor(Vector position, Quaternion orientation)
{
    internal_posisiton=position;
    internal_orientation=orientation;
}

const double gaussien_noise(const double &tau)
{   
    if (tau == 0)
        return 0;
    int rand1 = rand() % 1001;
    int rand2 = rand() % 1001;
    double noise = tau * sqrt(-2 * log(rand1 / 1000.0)) * cos(2 * M_PI * rand2 / 1000.0);
    if (isnan(noise) || isinf(noise))
    {
        return gaussien_noise(tau);
    }
    else
    {
        return noise;
    }
}

const Vector noise_v(const double std_noise[3])
{
    Vector ret(3);
    ret.set(0, gaussien_noise(std_noise[0]));
    ret.set(1, gaussien_noise(std_noise[1]));
    ret.set(2, gaussien_noise(std_noise[2]));
    return ret;
}

const double step_function(const double &x, const double &step)
{
    if (step == 0)
        return x;
    return step * round(x / step);
}

const Vector step_function_v(const Vector X, const double resolution[3])
{
    Vector ret(3);
    ret.set(0, step_function(X(0), resolution[0]));
    ret.set(1, step_function(X(1), resolution[1]));
    ret.set(2, step_function(X(2), resolution[2]));
    return ret;
}

const Vector sensor::get_samples(const Vector &physical_quantities_3Dvalue)
{
    // passband and lag delay application
    Vector ret = nonOrthogonality * physical_quantities_3Dvalue + offsets;
    ret += noise_v(std_noise);
    ret = step_function_v(ret, resolution);
    return ret;
}