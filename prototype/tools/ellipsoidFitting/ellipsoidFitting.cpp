/*
cree par : robinAZERTY
version du : 30/09/2022
*/

/*
an ellipsoid is can discribe by the following equation :
R=A^-1*(x-B)
where :
A is a 3x3 matrix (rotation and scaling) : 9 parameters to find
B is a 3x1 vector (translation) : 3 parameters to find
x is a 3x1 vector (point)
the goal of this program is to find the best A and B from a set of points
*/

#include "../../..\common\math\vector\Vector.cpp"

struct Ellipsoid
{
    Matrix A=IdentityMatrix(3);
    Vector B=Vector(3);
    double *points;
    unsigned int n;

    const double *fit()
    {
        const double initialparameters[12] = {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0};
        const double *parameters = gradDescent(initialparameters, 12, 1e-10, 0.2);
        A.set(0, 0, parameters[0]);
        A.set(0, 1, parameters[1]);
        A.set(0, 2, parameters[2]);
        A.set(1, 0, parameters[3]);
        A.set(1, 1, parameters[4]);
        A.set(1, 2, parameters[5]);
        A.set(2, 0, parameters[6]);
        A.set(2, 1, parameters[7]);
        A.set(2, 2, parameters[8]);
        B.set(0, parameters[9]);
        B.set(1, parameters[10]);
        B.set(2, parameters[11]);
        return parameters;
    }

private:
    const float cost_funtion(const double parameters[12])
    {
        A.set(0, 0, parameters[0]);
        A.set(0, 1, parameters[1]);
        A.set(0, 2, parameters[2]);
        A.set(1, 0, parameters[3]);
        A.set(1, 1, parameters[4]);
        A.set(1, 2, parameters[5]);
        A.set(2, 0, parameters[6]);
        A.set(2, 1, parameters[7]);
        A.set(2, 2, parameters[8]);

        B.set(0, parameters[9]);
        B.set(1, parameters[10]);
        B.set(2, parameters[11]);

        float ret = 0;

        for (unsigned int i = 0; i < n; i++)
        {
            Matrix x(3,1);
            x.set(0,0, points[3 * i]);
            x.set(1,0, points[3 * i + 1]);
            x.set(2,0, points[3 * i + 2]);

            Matrix R = (A.inverse()) * (x - B);
            float norm=sqrt(R(0,0)*R(0,0)+R(1,0)*R(1,0)+R(2,0)*R(2,0));
            float error = norm - 1;
            ret += error * error;
        }

        return ret / n;
    }
    const double *grad(const double *x, const unsigned int &n)
    {
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
            ret[i] = (cost_funtion(X1) - cost_funtion(X2)) / eps;
            X1[i] -= eps / 2;
            X2[i] += eps / 2;
        }
        return ret;
    }

    const double *gradDescent(const double *x0, const unsigned int &n, const double &eps, const double &alpha)
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

        double *x = new double[n + 1];
        for (unsigned int i = 0; i < n; i++)
        {
            x[i] = x0[i];
        }

        unsigned int count = 0;
        while (true)
        {
            const double *g = grad(x, n);

            bool stop = true;
            for (unsigned int i = 0; i < n; i++)
            {
                if (g[i] * g[i] > eps * eps)
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
                x[i] -= alpha * g[i];
            }
            count++;
        }
        x[n] = count;
        return x;
    }
};