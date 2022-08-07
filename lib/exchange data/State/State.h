#pragma once


#include "matrix.cpp"
#include "quaternion.cpp"

#include <string>
using namespace std;

class State
{
    public :
        State();
        ~State();
        
        Matrix position;
        Matrix velocity;
        Matrix acceleration;

        Quaternion orientation;
        Matrix angular_velocity;
        Matrix angular_acceleration;

        void set_description(string description);
        string to_str();       

    private:
        string description;

};