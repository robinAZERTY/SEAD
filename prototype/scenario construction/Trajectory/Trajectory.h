#pragma once

#include <MotionBase.cpp>

class Trajectory
{
public:
    Trajectory();
    ~Trajectory();

    void add_motion(MotionBase motion);
    State get_state(const double &t);

private:

    MotionBase *motions;
    double *t_separations;//help to chose wich motion we compute
    unsigned int nb_motions;
    unsigned int current_motion = 0;
};