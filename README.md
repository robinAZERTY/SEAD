Simulation-development


This branche contains the code for the simulation of your sensor unit (a system with sensors).

There are two main parts of the simulation:
- Scenario generation : 
    - A scenario idicates STATE (position and orientation) of the sensor unit in the world at given time. To define the position, Beziers curves are used (linear interpolation between points). To define the orientation, the same method is used, but we do a spherical interpolation between quaternions. In other wolrd, you can define the trajectory by giving a list of STATE and its derivate at given time.
- Sensor simulation : 
    - Each sensor have features (physical quantities to measure, noise, refresh delay, etc..). The first step is to define those parameters. Then, the sensor simulation is done by sampling the scenario at the right time, computing the physical quantities, and adding errors in measures.

You choose the movments of the system and your sensors. The code will simulate the sensor readings.
Now, usables sensors are:
accelerometer and gyroscope.
But i hope to add more sensors in the future like magnetometer, barometer, gps, etc.