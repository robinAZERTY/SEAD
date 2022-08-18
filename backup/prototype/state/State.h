#pragma once


#include "Vector.cpp"
#include "quaternion.cpp"

#include <string>
using namespace std;

class State
{
    public :
        State();
        ~State();
        
        Vector position;
        Vector velocity;
        Vector acceleration;

        Quaternion orientation;
        Vector angular_velocity;
        Vector angular_acceleration;

        void set_description(string description);
        string to_str();       

    private:
        string description;

};