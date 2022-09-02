/*
créé par RobinAZERTY
le 01/09/2022
*/

#pragma once

// je pose ça là

//ça permet de dériver à l'ordre n une fonction totalement inconue
// avec pour entrée un dx entre chaque dérivée
const double f_der(const double f(const double &x), const double &x, const unsigned short &n, const double &dx)
{
    if (n == 0)
        return f(x);
    else
        return (f_der(f, x + dx / 2, n - 1, dx) - f_der(f, x - dx / 2, n - 1, dx)) / dx;
}

// même chose que f_der sauf que Dx correspond à l'écart max qui englobe tous les calculs sur f
//plus on dérive, plus il faut augmenter Dx
const double f_der_bis(const double f(const double &x), const double &x, const unsigned short &n, const double &Dx)
{
    return f_der(f, x, n, Dx / n);
}

#include "math.h"

// dans le cas ou on connait exatement f' fait une approximation du meilleur Dx à choisir
double *search_best_dx(const double f(const double &x), const double true_der(const double &x, const unsigned short &n), const double &x, const unsigned short &n, const double &min_dx, const double &max_dx, const double &step)
{
    double best_error = 9.9e100;
    double best_dx = (min_dx + max_dx) / 2;
    for (double new_dx = max_dx; new_dx >= min_dx; new_dx -= step)
    {
        const double new_error = abs(f_der_bis(f, x, n, new_dx) - true_der(x, n));

        if (best_error >= new_error)
        {
            best_error = new_error;
            best_dx = new_dx;
        }
    }
    double *ret = new double[2];
    ret[0] = best_dx;
    ret[1] = best_error;
    return ret;
}