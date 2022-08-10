#pragma once

#include "MotionBase.cpp"

class StaticMotion : public MotionBase
{
public:
    void set_parameters(const Matrix &position, const Quaternion &orientation);

private:
    static struct INPUT
    {
        Matrix position;
        Quaternion orientation;
    };

    struct INPUT parameters; // input
    void init();
};