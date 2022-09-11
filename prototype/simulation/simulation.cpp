/*
cree par : RobinAZERTY
version du 11/09/2022
*/

#include "simulation.h"
#include <iostream>

simulation::simulation()
{
    scenario = new Scenario();
}

simulation::~simulation()
{
    for (unsigned int i = 0; i < sensor_number; i++)
    {
        delete sensors[i];
    }
    delete[] sensors;
    delete scenario;
}

void simulation::set_scenario(Scenario *scenario)
{
    this->scenario = scenario;
}

#include <typeinfo>
void simulation::add_sensor(sensor &new_sensor)
{
    const type_info &t = typeid(new_sensor);

    if (t == typeid(accelerometer))
    {
        data_type += separator + "accX" + separator + "accY" + separator + "accZ";
    }
    else if (t == typeid(gyroscope))
    {
        data_type += separator + "gyrX" + separator + "gyrY" + separator + "gyrZ";
    }

    sensor_number++;
    Vector *sensor_data_copy = new Vector[sensor_number];
    for (unsigned int i = 0; i < sensor_number - 1; i++)
    {
        sensor_data_copy[i] = sensor_data[i];
    }

    sensor_data = new Vector[sensor_number];
    for (unsigned int i = 0; i < sensor_number - 1; i++)
    {
        sensor_data[i] = sensor_data_copy[i];
    }
    sensor **new_sensors = new sensor *[sensor_number];
    for (unsigned int i = 0; i < sensor_number - 1; i++)
    {
        new_sensors[i] = sensors[i];
    }

    new_sensors[sensor_number - 1] = &new_sensor;

    for (unsigned int i = 0; i < sensor_number - 1; i++)
    {
        delete sensors[i];
    }

    sensors = new sensor *[sensor_number];
    for (unsigned int i = 0; i < sensor_number; i++)
    {
        sensors[i] = new_sensors[i];
    }

    for (unsigned int i = 0; i < sensor_number - 1; i++)
    {
        delete new_sensors[i];
    }
}

void simulation::set_time_step(const double &time_step)
{
    frame_number = floor(scenario->get_total_duration() / time_step);
    this->time_step = time_step;
}

void simulation::set_write_path(const string &path)
{
    this->write_path = path;
}

void simulation::next_frame()
{
    current_frame++;
    time = current_frame * time_step;
}

string simulation::get_data_type()
{
    return data_type;
}

const string simulation::get_data()
{
    return data;
}

#include <sstream>

void simulation::set_current_frame_data()
{
    for (unsigned int i = 0; i < sensor_number; i++)
    {
        scenario->update_state(time);
        const type_info &t = typeid(*sensors[i]);

        if (t == typeid(accelerometer))
        {
            accelerometer &acc = dynamic_cast<accelerometer &>(*sensors[i]);
            sensor_data[i] = acc.measure(scenario->get_state());
        }
        else if (t == typeid(gyroscope))
        {
            gyroscope *gyro = dynamic_cast<gyroscope *>(sensors[i]);
            sensor_data[i] = gyro->measure(scenario->get_state());
        }
    }

    ostringstream oss;
    // we need a addaptable precision digits number
    unsigned int expo=0;
    double tmp=time_step;
    while(tmp!=int(tmp)){
        tmp*=10;
        expo++;
    }
    const unsigned short precision = floor(log10(time))+expo;
    oss.precision(precision);
    oss << scientific << time;
    string data = oss.str();
    for (unsigned int i = 0; i < sensor_number; i++)
    {
        for (unsigned int j = 0; j < sensor_data[i].get_nb_rows(); j++)
        {
            ostringstream ss;
            // convert sensor_data[i](j) to scientific notation, in string
            ss << scientific << sensor_data[i](j);
            data += separator + ss.str();
            ;
        }
    }
    current_frame_data = data;
}

const string simulation::get_current_frame_data()
{

    return current_frame_data;
}

const unsigned int simulation::get_frame_number()
{
    return frame_number;
}
const unsigned int simulation::get_current_frame()
{
    return current_frame;
}

void simulation::save_frame_data()
{
    data += get_current_frame_data() + "\n";
}

#include <fstream>

void simulation::write_simulation()
{
    fstream file;
    if (write_path == "")
    {
        // if already exist, we erase the file
        file.open("simulation.csv", ios::out | ios::trunc);
        file.clear();
    }
    else
    {
        file.open(write_path, ios::out | ios::app);
    }

    file << get_data_type();
    file << "\n"
         << data;
}

void simulation::run()
{
    while (current_frame <= frame_number)
    {
        set_current_frame_data();
        save_frame_data();
        next_frame();
    }
}