/*
cree par : RobinAZERTY
version du 15/09/2022
*/

#pragma once
#include "sensor.h"

sensor::sensor(Vector position, Quaternion orientation)
{
    internal_posisiton=position;
    internal_orientation=orientation;
    this->samples=Vector(3);
}

const double gaussien_noise(const double &tau)
{   
    if (tau == 0)
        return 0;
    int rand1 = rand() % 1001;
    int rand2 = rand() % 1001;
    double noise = tau * sqrt(-2 * log(rand1 / 1000.0)) * cos(2 * PI * rand2 / 1000.0);
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
    double ret=round(x / step) * step;
    if(abs(ret)==0)
        return 0;
    return ret;
}

const Vector step_function_v(const Vector X, const double resolution[3])
{
    Vector ret(3);
    ret.set(0, step_function(X(0), resolution[0]));
    ret.set(1, step_function(X(1), resolution[1]));
    ret.set(2, step_function(X(2), resolution[2]));
    return ret;
}

void sensor::set_std_noise(const double std_noise[3])
{
    this->std_noise[0] = std_noise[0];
    this->std_noise[1] = std_noise[1];
    this->std_noise[2] = std_noise[2];
}

void sensor::set_resolution(const double resolution[3])
{
    this->resolution[0] = resolution[0];
    this->resolution[1] = resolution[1];
    this->resolution[2] = resolution[2];
}
void sensor::set_refresh_delay(const double refresh_delay[3])
{
    this->refresh_delay[0] = refresh_delay[0];
    this->refresh_delay[1] = refresh_delay[1];
    this->refresh_delay[2] = refresh_delay[2];
}

const Vector sensor::get_samples(const Vector &physical_quantities_3Dvalue, const double &time)
{
    // passband and lag delay application
    Vector ret = nonOrthogonality * physical_quantities_3Dvalue + offsets;
    ret += noise_v(std_noise);
    ret = step_function_v(ret, resolution);

    for (int i = 0; i < 3; i++)
    {   
        const double new_count = double(time/refresh_delay[i]);
        if (new_count>=count[i])
        {
            this->samples.set(i,ret(i));
            count[i]++;
        }
    }
    return this->samples;
}

// convert a pure quaternion to a vector
Vector quaternion_to_vector(const Quaternion &q)
{
    Vector ret(3);
    ret.set(0, q.b);
    ret.set(1, q.c);
    ret.set(2, q.d);
    return ret;
}

// convert a vector to a pure quaternion
Quaternion vector_to_quaternion(const Vector &v)
{
    Quaternion ret;
    ret.a = 0;
    ret.b = v(0);
    ret.c = v(1);
    ret.d = v(2);
    return ret;
}