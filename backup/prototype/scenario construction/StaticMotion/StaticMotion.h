#pragma once

#include "MotionBase.cpp"

class StaticMotion : public MotionBase
{
public:
    StaticMotion(const Matrix Position);
    virtual void set_parameters(const Matrix &position);

private:

    Matrix position;
    virtual void init();
};