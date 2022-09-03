/*
créé par : robinAZERTY
version du : 03/09/2022 - 0
*/

#pragma once
#define IS_VALIDATED false

// inlude all motions

#if !IS_VALIDATED
#include "..\..\../lib_validated/scenario construction/StaticOrientationMotion/StaticOrientationMotion.cpp"
#include "..\..\../lib_validated/scenario construction/BezierOrientationMotion/BezierOrientationMotion.cpp"
#include "..\..\../lib_validated/scenario construction/StaticPositionMotion/StaticPositionMotion.cpp"
#include "..\..\../lib_validated/scenario construction/BezierPositionMotion/BezierPositionMotion.cpp"

#else
#include "../StaticOrientationMotion/StaticOrientationMotion.cpp"
#include "../BezierOrientationMotion/BezierOrientationMotion.cpp"
#include "../StaticPositionMotion/StaticPositionMotion.cpp"
#include "../BezierPositionMotion/BezierPositionMotion.cpp"
#endif

class Scenario
{
public:
    Scenario();
    Scenario(const State initial_state);
    Scenario(const PositionState initial_state);
    Scenario(const OrientationState initial_state);
    Scenario(const Scenario &another);
    ~Scenario();
    const bool add_PositionMotion(PositionMotionBase PositionMotion);
    const bool add_BezierPositionMotion(PositionState finalState, const double &duration);

    const bool add_OrientationMotion(OrientationMotionBase PositionMotion);
    const bool add_BezierOrientationMotion(OrientationState finalState, const double &duration);
    const State get_state(const double &t);

    const double get_total_duration() { return total_duration; }
    const double get_position_scenario_duration() { return position_scenario_duration; }
    const double get_orientation_scenario_duration() { return orientation_scenario_duration; }

private:
    State initial_state, state, final_state;
    double position_scenario_duration = 0, orientation_scenario_duration = 0, total_duration = 0;
    void update_state(const double &t);
    void init();
    bool inited = false;
    PositionMotionBase *PositionMotion;
    OrientationMotionBase *OrientationMotion;
    unsigned int n_position_motions = 0, n_orientation_motions = 0;
    string description = "default Scenario";
};