/*
créé par : robinAZERTY
version du : 20/08/2022 - 0
*/
#pragma once
#include "BezierPositionMotion.h"

BezierPositionMotion::BezierPositionMotion() : PositionMotionBase()
{
    this->description = "default BezierPositionMotion";
}

BezierPositionMotion::BezierPositionMotion(const Vector points_3D[4], const double &duration)
{
    // verifier si les vecteurs sont bien de dim 3
    for (int i = 0; i < 4; i++)
    {
        if (points_3D[i].get_nb_rows() != 3)
            throw "vectors must be 3D";
    }
    this->A = points_3D[0];
    this->B = points_3D[1];
    this->C = points_3D[2];
    this->D = points_3D[3];
    this->duration = duration;
    this->alpha = 1 / duration;
    this->description = "BezierPositionMotion with 4 Points input";
}

BezierPositionMotion::BezierPositionMotion(const PositionState &initialState, const Vector points_3D[2], const double &duration)
{
    // verifier si les vecteurs sont bien de dim 3
    for (int i = 0; i < 2; i++)
    {
        if (points_3D[i].get_nb_rows() != 3)
            throw "vectors must be 3D";
    }
    this->duration = duration;
    this->alpha = 1 / duration;
    this->A = initialState.position;
    this->B = A + initialState.velocity * duration / 3.0;
    this->C = points_3D[0];
    this->D = points_3D[1];
    this->description = "BezierPositionMotion with initialState and 2 Points input";
}

BezierPositionMotion::BezierPositionMotion(const PositionState &initialState, const PositionState &finalState, const double &duration)
{
    this->duration = duration;
    this->alpha = 1 / duration;
    this->A = initialState.position;
    this->D = finalState.position;
    this->B = A + initialState.velocity * duration / 3.0;
    this->C = D - finalState.velocity * duration / 3.0;
    this->description = "BezierPositionMotion with initialState and finalState";
}

void BezierPositionMotion::update_state(const double &t)
{
    // verification de la validité des paramètres
    if (t < 0 || t > duration)
        throw "t must be between 0 and duration";

    const double t_ = t * alpha;

    state.position = A * (1 - t_) * (1 - t_) * (1 - t_) + B * 3 * t_ * (1 - t_) * (1 - t_) + C * 3 * t_ * t_ * (1 - t_) + D * t_ * t_ * t_;
    state.velocity = alpha * (-3 * (A * (1 - t_) * (1 - t_) + t_ * (-2 * C + 3 * C * t_ - D * t_) + B * (-1 + 4 * t_ - 3 * t_ * t_)));
    state.acceleration = alpha * (-6 * (B * (2 - 3 * t_) - C - A * (1 - t_) + 3 * C * t_ - D * t_));
}

PositionState BezierPositionMotion::get_state(const double &t)
{
    if (!inited)
        init();

    update_state(t);
    return this->state;
}

PositionState BezierPositionMotion::get_initial_state()
{
    if (!inited)
        init();

    return this->initialState;
}
PositionState BezierPositionMotion::get_final_state()
{
    if (!inited)
        init();

    return this->finalState;
}

void BezierPositionMotion::init()
{
    update_state(duration);
    finalState = state;

    update_state(0);
    initialState = state;

    state = initialState;

    inited = true;
}