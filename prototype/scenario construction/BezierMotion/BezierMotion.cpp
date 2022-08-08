#include "BezierMotion.h"

void BezierMotion::set_parameters(const Matrix points[4])
{
    parameters.points[0] = points[0];
    parameters.points[1] = points[1];
    parameters.points[2] = points[2];
    parameters.points[3] = points[3];
}

void BezierMotion::update_state(const double &t)
{
    if(t<0 || t>1)
    {
        return;
    }
    else
    {   
        Matrix &A = parameters.points[0];
        Matrix &B = parameters.points[1];
        Matrix &C = parameters.points[2];
        Matrix &D = parameters.points[3];
        /*
        state.position = parameters.points[0]*(1-t)*(1-t)*(1-t) + parameters.points[1]*3*t*(1-t)*(1-t) + parameters.points[2]*3*t*t*(1-t) + parameters.points[3]*t*t*t;
        state.velocity = parameters.points[0]*3*(1-t)*(1-t) + parameters.points[1]*3*2*t*(1-t) + parameters.points[2]*3*t*t + parameters.points[3]*3*t*t;
        state.acceleration = parameters.points[0]*6*(1-t) + parameters.points[1]*6*2*t + parameters.points[2]*6*t + parameters.points[3]*6*t;   
        */
       state.position = A*(1-t)*(1-t)*(1-t) + B*3*t*(1-t)*(1-t) + C*3*t*t*(1-t) + D*t*t*t;

       //derivate of position :dP/dt = 3(A-3B+3C-D)*(1-t)*(1-t) + 3(B-2C+D)*2t*(1-t) + 3(C-D)*t*t + 3(D)*t*t
       state.velocity= 3*(A-3*B+3*C-D)*(1-t)*(1-t) + 3*(B-2*C+D)*2*t*(1-t) + 3*(C-D)*t*t + 3*D*t*t;
    }
}