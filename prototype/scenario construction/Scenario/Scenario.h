#pragma once

#include <MotionBase.cpp>

class Scenario
{
public:
    Scenario();
    ~Scenario();

    State get_state(const double &t);

private:

    MotionBase *motions;
    double *t_separations;//help to chose wich motion may be computed
};