State Estimation - dev
==============================

We have now a working version of Sensor Simulation !
So we can start working on State Estimation.
The goal is to give many options to the user, he can choose the best method for his application.
So he can choose between many tools and connect them as he wants.

Here is the list of the tasks to do :

    -   Identify wich tools we can use to do the state estimation
    -   put those in different categories

Tools must be independent from the simulation and any library. They must be able to work from any computer, processor or microprocessor (like arduino).
tools ideas :
    
    filter :
        -   Kalman Filter
        -   Particle Filter
        -   Extended Kalman Filter
        -   complementary filter
        -   low pass filter
        -   high pass filter
    
    calibration :
        -   ellipsoid fitting (least square)
        -   noise estimation (high pass filter)
    
    computation :
        -   orientation gyro integration (quaternion)
        -   orientation acc
    