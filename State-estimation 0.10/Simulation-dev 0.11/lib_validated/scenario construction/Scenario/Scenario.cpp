/*
créé par : robinAZERTY
version du : 03/09/2022 - 0
*/
#include "Scenario.h"

Scenario::Scenario()
{
}

Scenario::~Scenario()
{
    if (n_position_motions > 0)
        delete[] PositionMotion;
    if (n_orientation_motions > 0)
        delete[] this->OrientationMotion;
}

Scenario::Scenario(const Scenario &another)
{
    PositionMotion = another.PositionMotion;
    OrientationMotion = another.OrientationMotion;
}

Scenario::Scenario(const State initial_state)
{
    this->initial_state = initial_state;
    this->state = initial_state;
    this->final_state = initial_state;
}

Scenario::Scenario(const PositionState initial_state)
{
    this->initial_state.positionState = initial_state;
    this->state.positionState = initial_state;
    this->final_state.positionState = initial_state;
}

const bool position_is_continuous(BezierPositionMotion &m1, BezierPositionMotion &m2)
{
    Vector a = m1.get_final_state().position - m2.get_initial_state().position;
    Vector b = m1.get_final_state().velocity - m2.get_initial_state().velocity;
    const bool same_position = a.norm() < 1.0e-5;
    const bool same_velocity = b.norm() < 1.0e-5;
    return same_position && same_velocity;
}

const bool orientation_is_continuous(BezierOrientationMotion &m1, BezierOrientationMotion &m2)
{
    Quaternion a = m1.get_final_state().q - m2.get_initial_state().q;
    Quaternion b = m1.get_final_state().q_velocity - m2.get_initial_state().q_velocity;

    const bool same_position = a.norm() < 1.0e-5;
    const bool same_velocity = b.norm() < 1.0e-5;
    return same_position && same_velocity;
}

const bool Scenario::add_PositionMotion(BezierPositionMotion &new_PositionMotion)
{
    // verification de la continuité
    if (n_position_motions > 0 && !position_is_continuous(this->PositionMotion[n_position_motions - 1], new_PositionMotion))
    {
        throw "bad transition";
    }

    if (n_position_motions == 0)
    {
        PositionMotion = new BezierPositionMotion[1];
        PositionMotion[0] = new_PositionMotion;
    }
    else
    {
        BezierPositionMotion *copy = new BezierPositionMotion[n_position_motions];
        for (int i = 0; i < n_position_motions; i++)
        {
            copy[i] = PositionMotion[i];
        }
        delete[] PositionMotion;

        PositionMotion = new BezierPositionMotion[n_position_motions + 1];
        for (int i = 0; i < n_position_motions; i++)
        {
            PositionMotion[i] = copy[i];
        }
        PositionMotion[n_position_motions] = new_PositionMotion;
    }

    n_position_motions++;
    position_scenario_duration += new_PositionMotion.get_duration();
    total_duration = max(position_scenario_duration, orientation_scenario_duration);
    final_state.positionState = new_PositionMotion.get_state(new_PositionMotion.get_duration());

    return true;
}

const bool Scenario::add_BezierPositionMotion(PositionState finalState, const double &duration)
{
    BezierPositionMotion new_motion(this->final_state.positionState, finalState, duration);
    return this->add_PositionMotion(new_motion);
}

const bool Scenario::add_OrientationMotion(BezierOrientationMotion &new_OrientationMotion)
{
    // verification de la continuité
    if (n_orientation_motions > 0 && !orientation_is_continuous(this->OrientationMotion[n_orientation_motions - 1], new_OrientationMotion))
    {
        throw "bad transition";
    }

    if (n_orientation_motions == 0)
    {
        OrientationMotion = new BezierOrientationMotion[1];
        OrientationMotion[0] = new_OrientationMotion;
    }
    else
    {
        BezierOrientationMotion *copy = new BezierOrientationMotion[n_orientation_motions];
        for (int i = 0; i < n_orientation_motions; i++)
        {
            copy[i] = OrientationMotion[i];
        }
        delete[] OrientationMotion;

        OrientationMotion = new BezierOrientationMotion[n_orientation_motions + 1];
        for (int i = 0; i < n_orientation_motions; i++)
        {
            OrientationMotion[i] = copy[i];
        }
        OrientationMotion[n_orientation_motions] = new_OrientationMotion;
    }

    n_orientation_motions++;
    orientation_scenario_duration += new_OrientationMotion.get_duration();
    total_duration = max(position_scenario_duration, orientation_scenario_duration);
    final_state.orientationState = new_OrientationMotion.get_state(new_OrientationMotion.get_duration());

    return true;
}

const bool Scenario::add_BezierOrientationMotion(OrientationState finalState, const double &duration)
{
    BezierOrientationMotion new_motion;
    new_motion = BezierOrientationMotion(this->final_state.orientationState, finalState, duration);
    return this->add_OrientationMotion(new_motion);
}

const State Scenario::get_state()
{
    return state;
}
void Scenario::init()
{
    update_position_state(position_scenario_duration);
    final_state.positionState = state.positionState;
    if(n_position_motions > 0)
        initial_state.positionState = PositionMotion[0].get_state(0);

    update_orientation_state(orientation_scenario_duration);
    final_state.orientationState = state.orientationState;
    update_orientation_state(0);
    initial_state.orientationState = state.orientationState;
    inited = true;
}

void Scenario::update_position_state(const double &t)
{
    if (n_position_motions == 0)
        return;
    short index_positionMotion = -1;
    double cumulate_duration = 0;
    while (t > cumulate_duration)
    {
        index_positionMotion++;
        if (index_positionMotion >= n_position_motions)
        {
            break;
        }
        if (index_positionMotion >= 0)
            cumulate_duration += PositionMotion[index_positionMotion].get_duration();
    }
    cumulate_duration -= PositionMotion[index_positionMotion].get_duration();
    const double t_pos = t - cumulate_duration;
    if (index_positionMotion < 0)
        state.positionState = initial_state.positionState;
    else if (index_positionMotion >= n_position_motions)
    {
        final_state.positionState.velocity.fill(0);
        final_state.positionState.acceleration.fill(0);
        state.positionState = final_state.positionState;
    }
    else
    {
        state.positionState = PositionMotion[index_positionMotion].get_state(t_pos);
    }
}

void Scenario::update_orientation_state(const double &t)
{
    if (n_orientation_motions == 0)
        return;
    short index_orientationMotion = -1;
    double cumulate_duration = 0;
    while (t > cumulate_duration)
    {
        index_orientationMotion++;
        if (index_orientationMotion >= n_orientation_motions)
        {
            break;
        }
        if (index_orientationMotion >= 0)
            cumulate_duration += OrientationMotion[index_orientationMotion].get_duration();
    }
    cumulate_duration -= OrientationMotion[index_orientationMotion].get_duration();
    const double t_ = t - cumulate_duration;
    if (index_orientationMotion < 0)
        state.orientationState = initial_state.orientationState;
    else if (index_orientationMotion >= n_orientation_motions)
    {
        final_state.orientationState.q_velocity *= 0;
        final_state.orientationState.q_acceleration *= 0;
        state.orientationState = final_state.orientationState;
    }
    else
    {
        state.orientationState = OrientationMotion[index_orientationMotion].get_state(t_);
    }
}

void Scenario::update_state(const double &t)
{
    if (!inited)
        init();

    if (t < 0 || t > total_duration)
        throw "t must be between 0 and total_duration";

    update_position_state(t);
    update_orientation_state(t);
}