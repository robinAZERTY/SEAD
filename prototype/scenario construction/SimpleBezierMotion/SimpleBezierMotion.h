#pragma once

#include "MotionBase.cpp"

class SimpleBezierMotion : public MotionBase
{
    public :

        void set_parameters(const Matrix points[4]);

    private :

    static struct INPUT
    {
        Matrix points[4];
    };
    struct INPUT parameters; // input

    void update_state(const double &t);
};