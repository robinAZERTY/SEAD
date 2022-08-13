#pragma once

#include "MotionBase.cpp"

class BezierMotion : public MotionBase
{
    public :
        BezierMotion(const Matrix points[4], const double &duration);
        BezierMotion(const Matrix initialPosition, const Matrix initialVelocity, const Matrix new_points[2], const double &duration);
        virtual void set_parameters(const Matrix points[4], const double &duration);

    private :

        Matrix A,B,C,D;
        virtual void update_state(const double &t);
        virtual void init();
};