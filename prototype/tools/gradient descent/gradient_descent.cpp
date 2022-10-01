/*
cree par : robinAZERTY
version du : 01/10/2022

can be used to minimize a function in a structure
*/

static const double *grad(const double (*f)(const double *x), const double *x, const unsigned int &n)
{
    // the gradient of a function f is a vector of partial derivatives
    // f is the function to minimize
    // n is the number of variable (dimension)
    // x is the vector of variable

    const double eps = 1e-6; // precision for derivative calculation

    // copy x 2 times
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
        // change the i-th variable
        X1[i] += eps / 2;
        X2[i] -= eps / 2;
        const double f1 = f(X1)/eps;
        const double f2 = f(X2)/eps;
        // make sure the function is not divergent
        if (f1 == 1.0 / 0.0 || f1 == -1.0 / 0.0 || f2 == 1.0 / 0.0 || f2 == -1.0 / 0.0)
            ret[i] = 1.0 / 0.0;
        else
            ret[i] = (f1 - f2); // calculate the partial derivative as the limit of (f(x-h) - f(x+h))/2h

        std::cout<<f1<<"\t"<<f2<<" "<<ret[i]<<"\t";
        X1[i] -= eps / 2;
        X2[i] += eps / 2;
    }
    std::cout<<std::endl;
    return ret;
}

static const double *gradDescent(const double (*f)(const double *x), const double *x0, const unsigned int &n, const double &eps, const double &alpha)
{
    // f is the function to minimize
    // n is the number of variable (dimension)
    // x0 is the initial vector of variable
    // eps is the stop condition (when all terms of the gradient are less than eps)
    // alpha is the learning rate (step size)
    // return the vector of variable at the minimum and the number of iteration that took to find it

    /*
    the idea is to find a minimum of f by moving in the opposite direction of the gradient
    warning : the final result is not guaranteed to be the absolute minimum
    */

    double *x = new double[n + 1];
    for (unsigned int i = 0; i < n; i++)
    {
        x[i] = x0[i];
    }

    unsigned int count = 0;
    while (true)
    {
        const double *g = grad(f, x, n);

        bool stop = true;
        for (unsigned int i = 0; i < n; i++)
        {
            if ((g[i] > eps || g[i] < -eps))
            {
                stop = false;
                break;
            }
            else
            {
                std::cout << "g[" << i << "] = " << g[i] << std::endl;
            }
        }
        if (stop)
        {
            std::cout << "stop" << std::endl;
            break;
        }

        for (unsigned int i = 0; i < n; i++)
        {
            x[i] -= alpha * g[i];
        }
        count++;
    }
    x[n] = count;
    return x;
}