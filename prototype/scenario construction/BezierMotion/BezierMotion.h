#pragma once

#include "MotionBase.cpp"

class BezierMotion : public MotionBase
{
    public :
        BezierMotion(const Matrix points[4], const double &duration);
        virtual void set_parameters(const Matrix points[4], const double &duration);

    private :

        Matrix A=Matrix(3,1),B=A,C=A,D=A;
        virtual void update_state(const double &t);
        virtual void init();
};