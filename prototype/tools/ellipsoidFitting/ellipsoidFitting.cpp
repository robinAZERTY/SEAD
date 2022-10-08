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
#include "../../..\validated\tools\gradient descent\gradient_descent.cpp"

class Ellipsoid
{
public:
    // constructor
    Ellipsoid();
    ~Ellipsoid();

    inline static Matrix A;
    inline static Vector B;
    inline static double *points;
    inline static unsigned int n;
    inline static double minimum_cost;

    const double *fit();

private:
    static const double cost_funtion(const double parameters[12]);
};

Ellipsoid::Ellipsoid()
{
    A = IdentityMatrix(3);
    B = Vector(3);
}

Ellipsoid::~Ellipsoid()
{
}

const double *Ellipsoid::fit()
{
    // const double initialparameters[12] = {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0};
    const double initialparameters[12] = {0.77, 0.22, 0, 0.02, 0.94, -0.13, 0.19, 0.31, 1.28, -0.47296, -0.89519, 0.2};
    const double *parameters = gradDescent(Ellipsoid::cost_funtion, initialparameters, 12, 0.3e-4, 1);
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
    minimum_cost=Ellipsoid::cost_funtion(parameters);
    return parameters;
}

const double Ellipsoid::cost_funtion(const double parameters[12])
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

    double ret = 0;

    for (unsigned int i = 0; i < n; i++)
    {
        Vector x(3);
        x.set(0, points[3 * i]);
        x.set(1, points[3 * i + 1]);
        x.set(2, points[3 * i + 2]);

        Vector R = A.inverse() * (x - B);
        double norm = R.norm();
        double error = norm - 1; // the distance between the point and the ellipsoid surface
        ret += error * error;
    }
    ret /= n;
    return ret;
}