#pragma once

#include <MotionBase.cpp>

class MotionConstruction
{
public:
    MotionConstruction();
    ~MotionConstruction();

    State get_state(const double &t);

private:

    MotionBase *motions;
    double *t_separations;//help to chose wich motion we compute
    unsigned int nb_motions;
};