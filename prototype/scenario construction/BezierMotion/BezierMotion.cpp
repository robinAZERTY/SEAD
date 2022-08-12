#include "BezierMotion.h"

BezierMotion::BezierMotion(const Matrix points[4], const double &duration)
{
    set_parameters(points, duration);
}

void BezierMotion::set_parameters(const Matrix points[4], const double &duration)
{
    //verification de la validité des paramètres
    for (int i = 0; i < 4; i++)
    {
        if (points[i].get_nb_rows() != 3 || points[i].get_nb_cols() != 1)
        {
            throw "points must be a 3x1 matrix";
        }
    }
    
    A = points[0];
    B = points[1];
    C = points[2];
    D = points[3];
    this->duration = duration;
    inited = false;
}

void BezierMotion::update_state(const double &t)
{   
    //verification de la validité des paramètres
    if(t<0 || t>duration)
    {
        throw "t must be between 0 and duration";
    }

    double alpha = 1 / duration;
    double t_ = t * alpha;

    state.position= A*(1-t_)*(1-t_)*(1-t_) + B*3*t_*(1-t_)*(1-t_) + C*3*t_*t_*(1-t_) + D*t_*t_*t_;
    state.velocity= alpha*(3*(A-3*B+3*C-D)*(1-t_)*(1-t_) + 3*(B-2*C+D)*2*t_*(1-t_) + 3*(C-D)*t_*t_ + 3*D*t_*t_);
    state.acceleration= alpha*(6*(A-2*B+C)*(1-t_) + 6*(B-C)*2*t_ + 6*C*t_ + 6*D*t_);
}

void BezierMotion::init()
{
    update_state(duration);
    finalState = state;

    update_state(0);
    initialState = state;

    state=initialState;

    inited = true;
}