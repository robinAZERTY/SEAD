/*
cree par : robinAZERTY
version du : 30/09/2022
*/

#include <math.h>

const double* grad(const double f(const double *x),const double *x, const unsigned int &n)
{
    // f is the function to minimize
    // n is the number of variable (dimension)
    // x is the vector of variable

    const double eps = 1e-6; // precision for derivative calculation

    //copy x 2 times
    double *X1 = new double[n];
    double *X2 = new double[n];
    for (unsigned int j = 0; j < n; j++)
    {
        X1[j] = x[j];
        X2[j] = x[j];
    }

    double *ret = new double[n];
    for (unsigned int i = 0; i < n; i++)
    {
        //change the i-th variable
        X1[i] += eps/2;
        X2[i] -= eps/2;
        ret[i] = (f(X1) - f(X2))/eps;
        X1[i] -= eps/2;
        X2[i] += eps/2;
    }
    return ret;
}

const double* gradDescent(const double f(const double *x),const double *x0, const unsigned int &n,const double &eps, const double &alpha)
{
    // f is the function to minimize
    // n is the number of variable (dimension)
    // x0 is the initial vector of variable
    // eps is the stop condition (when all terms of the gradient are less than eps)
    // alpha is the learning rate (step size)

    /*
    the idea is to find a minimum of f by moving in the opposite direction of the gradient
    warning : the final result is not guaranteed to be the absolute minimum
    */

    double *x = new double[n+1];
    for (unsigned int i = 0; i < n; i++)
    {
        x[i] = x0[i];
    }

    unsigned int count=0;
    while (true)
    {
        const double *g = grad(f,x,n);
        
        bool stop = true;
        for (unsigned int i = 0; i < n; i++)
        {
            if (abs(g[i]) > eps)
            {
                stop = false;
                break;
            }
        }
        if (stop)
        {
            break;
        }
        for (unsigned int i = 0; i < n; i++)
        {
            x[i] -= alpha*g[i];
        }
        count++;
    }
    x[n] = count;
    return x;
}