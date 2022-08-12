#pragma once

#include "MotionBase.cpp"

class StaticMotion : public MotionBase
{
public:
    virtual void set_parameters(const Matrix &position, const Quaternion &orientation);

private:

    Matrix position;
    Quaternion orientation;

    virtual void init();
};