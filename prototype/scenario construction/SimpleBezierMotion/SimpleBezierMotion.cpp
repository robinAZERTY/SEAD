#include "SimpleBezierMotion.h"

void SimpleBezierMotion::set_parameters(const Matrix points[4])
{
    parameters.points[0] = points[0];
    parameters.points[1] = points[1];
    parameters.points[2] = points[2];
    parameters.points[3] = points[3];
}

void SimpleBezierMotion::update_state(const double &t)
{
    if (t < 0 || t > 1)
    {
        return;
    }
    else
    {
        Matrix &A = parameters.points[0];
        Matrix &B = parameters.points[1];
        Matrix &C = parameters.points[2];
        Matrix &D = parameters.points[3];

        // P(t) = A*(1-t)^3 + B*3*t*(1-t)² + C*3*t²*(1-t) + D*t^3
        state.position = A * (1 - t) * (1 - t) * (1 - t) + B * 3 * t * (1 - t) * (1 - t) + C * 3 * t * t * (1 - t) + D * t * t * t;

        // derivate of position :dP/dt = 3(A-3B+3C-D)*(1-t)² + 3(B-2C+D)*2t*(1-t) + 3(C-D)*t² + 3(D)*t²
        state.velocity = 3 * (A - 3 * B + 3 * C - D) * (1 - t) * (1 - t) + 3 * (B - 2 * C + D) * 2 * t * (1 - t) + 3 * (C - D) * t * t + 3 * D * t * t;

        // derivate of velocity :dV/dt = 6(A-3B+3C-D)*(1-t) + 6(B-2C+D)*2t + 6(C-D)*t + 6(D)
        state.acceleration = 6 * (A - 3 * B + 3 * C - D) * (1 - t) + 6 * (B - 2 * C + D) * 2 * t + 6 * (C - D) * t + 6 * D;
    }
}