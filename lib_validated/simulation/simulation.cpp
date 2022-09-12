/*
cree par : RobinAZERTY
version du 11/09/2022
*/
#pragma once
#include "simulation.h"
#include <iostream>

simulation::simulation()
{
    scenario = new Scenario();
    //state data_type
    string state_data_type[]={"posX","posY","posZ","velX","velY","velZ","accX","accY","accZ","q0","q1","q2","q3","q_vel0","q_vel1","q_vel2","q_vel3","q_acc0","q_acc1","q_acc2","q_acc3"};
    for (int i = 0; i < 19; i++)
    {
        data_type += separator + state_data_type[i];
    }
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
        data_type += separator + new_sensor.description + "X" + separator + new_sensor.description + "Y" + separator + new_sensor.description + "Z";
    }
    else if (t == typeid(gyroscope))
    {
        data_type += separator + new_sensor.description + "X" + separator + new_sensor.description + "Y" + separator + new_sensor.description + "Z";
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


    stringstream oss;
    oss<<time;
    string data = oss.str();

    //add state data into a double reference list
    double *state_data=new double[21];
    state_data[0] = scenario->get_state().positionState.position(0);
    state_data[1] = scenario->get_state().positionState.position(1);
    state_data[2] = scenario->get_state().positionState.position(2);
    state_data[3] = scenario->get_state().positionState.velocity(0);
    state_data[4] = scenario->get_state().positionState.velocity(1);
    state_data[5] = scenario->get_state().positionState.velocity(2);
    state_data[6] = scenario->get_state().positionState.acceleration(0);
    state_data[7] = scenario->get_state().positionState.acceleration(1);
    state_data[8] = scenario->get_state().positionState.acceleration(2);
    state_data[9] = scenario->get_state().orientationState.q.a;
    state_data[10] = scenario->get_state().orientationState.q.b;
    state_data[11] = scenario->get_state().orientationState.q.c;
    state_data[12] = scenario->get_state().orientationState.q.d;
    state_data[13] = scenario->get_state().orientationState.q_velocity.a;
    state_data[14] = scenario->get_state().orientationState.q_velocity.b;
    state_data[15] = scenario->get_state().orientationState.q_velocity.c;
    state_data[16] = scenario->get_state().orientationState.q_velocity.d;
    state_data[17] = scenario->get_state().orientationState.q_acceleration.a;
    state_data[18] = scenario->get_state().orientationState.q_acceleration.b;
    state_data[19] = scenario->get_state().orientationState.q_acceleration.c;
    state_data[20] = scenario->get_state().orientationState.q_acceleration.d;

    oss.precision(6);
    for(unsigned int i = 0; i < 21; i++)
    {   
        ostringstream ss;
        ss<< state_data[i];
        data += separator + ss.str();
    }

    delete[] state_data;

    for (unsigned int i = 0; i < sensor_number; i++)
    {
        for (unsigned int j = 0; j < sensor_data[i].get_nb_rows(); j++)
        {
            ostringstream ss;
            ss<<sensor_data[i](j);
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
    data += "\n" + get_current_frame_data();
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
        file.open(write_path, ios::out | ios::trunc);
        file.clear();
    }

    file << get_data_type();
    file << data;
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