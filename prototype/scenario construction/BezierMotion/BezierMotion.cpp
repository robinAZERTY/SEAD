#include "BezierMotion.h"

BezierMotion::BezierMotion(const Matrix points[4], const double &duration)
{
    set_parameters(points, duration);
}

BezierMotion::BezierMotion(const Matrix initialPosition, const Matrix initialVelocity, const Matrix new_points[2], const double &duration)
{   

    Matrix new_4points[4];
    new_4points[0]=initialPosition;
    new_4points[1]=initialVelocity*duration/3.0+new_4points[0];
    new_4points[2]=new_points[0];
    new_4points[3]=new_points[1];

    set_parameters(new_4points, duration);
}

void BezierMotion::set_parameters(const Matrix points[4], const double &duration)
{
    // les 4 Matrices de points doivent avoir les mêmes dimensions
    const unsigned int nb_rows = points[0].get_nb_rows();
    const unsigned int nb_cols = points[0].get_nb_cols();

    for(unsigned int i=1; i<4; i++)
    {
        if(points[i].get_nb_rows() != nb_rows || points[i].get_nb_cols() != nb_cols)
        {
            throw "points must have the same dimensions";
        }
    }

    A = points[0];
    B = points[1];
    C = points[2];
    D = points[3];
    state.resize(A.get_nb_rows(),3*A.get_nb_cols());//because it will return its prime and seconde derivate
    this->duration = duration;
    inited = false;
}

void BezierMotion::update_state(const double &t)
{
    // verification de la validité des paramètres
    if (t < 0 || t > duration)
    {
        throw "t must be between 0 and duration";
    }

    double alpha = 1 / duration;
    double t_ = t * alpha;
    Matrix position,velocity,acceleration;

    position = A * (1 - t_) * (1 - t_) * (1 - t_) + B * 3 * t_ * (1 - t_) * (1 - t_) + C * 3 * t_ * t_ * (1 - t_) + D * t_ * t_ * t_;
    velocity = alpha * (-3 * (A * (1 - t_) * (1 - t_) + t_ * (-2 * C + 3 * C * t_ - D * t_) + B * (-1 + 4 * t_ - 3 * t_ * t_)));
    acceleration = alpha * (-6 * (B * (2 - 3 * t_) - C - A * (1 - t_) + 3 * C * t_ - D * t_));
    
    for(unsigned int i=0; i<A.get_nb_rows();i++)
    {
        for( unsigned int j=0;j<A.get_nb_cols();j++)
        {
            state.set(i,j,position(i,j));
            state.set(i,j+A.get_nb_cols(),velocity(i,j));
            state.set(i,j+2*A.get_nb_cols(),acceleration(i,j));
        }
    }
}

void BezierMotion::init()
{
    update_state(duration);
    finalState = state;

    update_state(0);
    initialState = state;

    state = initialState;

    inited = true;
}