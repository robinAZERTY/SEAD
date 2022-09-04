#include "sensor.h"

const double noise(const bool &std_noise)
{
    return 0;
};

const double passband(const double &f(const double &time),const double &time,const const double &low_f,const double &high_f)
{
    return f(time);
}

const double step_function(const double &x, const double &step)
{
    return x;
}

const double sensor::get_sample(const double &physical_quantities_value(const double &time),const double &time)
{
    //passband and lag delay application
    double ret=passband(physical_quantities_value,time-lag_delay,passband_freq[0],passband_freq[1]);

    //linerarity application
    ret = linearity(ret);

    //noise application
    ret=ret+noise(std_noise);

    //resolution application
    ret=step_function(ret,resolution);

    return ret;
}