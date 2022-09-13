/*
cree par : RobinAZERTY
version du 11/09/2022
*/

#pragma once

static const bool IS_VALIDATED = true;

#if !IS_VALIDATED

#include "..\../lib_validated/sensors/accelerometer/accelerometer.cpp"
#include "..\../lib_validated/sensors/gyroscope/gyroscope.cpp"
#include "..\../lib_validated/scenario construction/Scenario/Scenario.cpp"

#else
#include "..\sensors\accelerometer\accelerometer.cpp"
#include "..\sensors\gyroscope\gyroscope.cpp"
#include "..\scenario construction/Scenario/Scenario.cpp"
#endif

class simulation
{
public:
    simulation();
    ~simulation();

    void set_scenario(Scenario &scenario);
    void add_sensor(sensor &new_sensor);

    void set_time_step(const double &time_step);
    void set_write_path(const string &path);
    void set_string_separator(const string &separator);

    void run();

    void next_frame();
    string get_data_type();
    void set_current_frame_data();
    const string get_current_frame_data();
    const string get_data();
    const unsigned int get_frame_number();
    const unsigned int get_current_frame();
    void save_frame_data();
    void write_simulation();

private:
    Scenario *scenario;
    sensor **sensors;
    unsigned int sensor_number = 0;

    Vector *sensor_data;
    string write_path;
    string separator="\t";
    string data_type = "time"; //"time,accX,accY,accZ,gyroX,gyroY,gyroZ..."
    string current_frame_data; //"0.1,0.2,0.3,0.4,0.5,0.6..."
    string data;               // all data of simulation

    unsigned int frame_number = 0;
    unsigned int current_frame = 0;
    double time = 0;
    double time_step = 0.01;
};