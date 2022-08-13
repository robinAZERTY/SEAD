#include "StaticMotion.h"

StaticMotion::StaticMotion(const Matrix Position)
{
    set_parameters(Position);
}

void StaticMotion::set_parameters(const Matrix &position)
{   
    //verification de la validité des paramètres
    if(position.get_nb_rows() != 3 || position.get_nb_cols() != 1)
    {
        throw "position must be a 3x1 matrix";
    }

    this->position = position;
    inited = false;
}

void StaticMotion::init()
{
    state.position = Matrix();
    state.velocity = Matrix();
    state.acceleration = Matrix();
    state.orientation = Quaternion();
    state.angular_velocity = Matrix();
    state.angular_acceleration = Matrix();

    state.position = this->position;
    state.orientation = Quaternion();
    
    initialState = state;
    finalState = state;
    inited = true;
}