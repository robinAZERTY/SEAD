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
    state.position = Matrix(3, 1);
    state.velocity = Matrix(3, 1);
    state.acceleration = Matrix(3, 1);
    state.orientation = Quaternion();
    state.angular_velocity = Matrix(3, 1);
    state.angular_acceleration = Matrix(3, 1);

    state.position = this->position;
    state.velocity.fill(0);
    state.acceleration.fill(0);
    state.orientation = Quaternion();
    state.angular_velocity.fill(0);
    state.angular_acceleration.fill(0);
    initialState = state;
    finalState = state;
    inited = true;
}