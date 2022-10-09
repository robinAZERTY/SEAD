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
#include "../../..\common\math\quaternion\Quaternion.cpp"
#include <iostream>

class Ellipsoid
{
public:
    // constructor
    Ellipsoid();
    ~Ellipsoid();

    inline static Matrix A;
    inline static Vector B;

    // explicites A and B as : [radius,gains,nonOrtho,offsets]
    inline static double radius;
    inline static Vector gains;
    inline static Matrix nonOrtho;
    inline static Vector offsets;

    inline static double *points;
    inline static unsigned int n;
    inline static double minimum_cost;
    inline static double minimum_rotation_cost;

    const double *fit_grad_descent();
    const double *fit_basic();
    const double *rotation_optimization();

private:
    static void explicit_A_B();
    static const double cost_funtion(const double parameters[12]);
    static const double angular_cost_funtion(const double parameters[4]);

    void set_A_B(const double parameters[12]);
    void set_A_B(const double radius, const Vector gains, const Matrix nonOrtho, const Vector offsets);
};

Ellipsoid::Ellipsoid()
{
    A = IdentityMatrix(3);
    B = Vector(3);

    radius = 1;
    gains = Vector(3);
    nonOrtho = IdentityMatrix(3);
    offsets = Vector(3);
}

Ellipsoid::~Ellipsoid()
{
}

void Ellipsoid::explicit_A_B()
{
    offsets = B;
    const double nx = sqrt(A(0, 0) * A(0, 0) + A(0, 1) * A(0, 1) + A(0, 2) * A(0, 2));
    const double ny = sqrt(A(1, 0) * A(1, 0) + A(1, 1) * A(1, 1) + A(1, 2) * A(1, 2));
    const double nz = sqrt(A(2, 0) * A(2, 0) + A(2, 1) * A(2, 1) + A(2, 2) * A(2, 2));
    radius = (nx + ny + nz) / 3;
    gains.set(0, nx / radius);
    gains.set(1, ny / radius);
    gains.set(2, nz / radius);
    nonOrtho.set(0, 0, A(0, 0) / nx);
    nonOrtho.set(0, 1, A(0, 1) / nx);
    nonOrtho.set(0, 2, A(0, 2) / nx);
    nonOrtho.set(1, 0, A(1, 0) / ny);
    nonOrtho.set(1, 1, A(1, 1) / ny);
    nonOrtho.set(1, 2, A(1, 2) / ny);
    nonOrtho.set(2, 0, A(2, 0) / nz);
    nonOrtho.set(2, 1, A(2, 1) / nz);
    nonOrtho.set(2, 2, A(2, 2) / nz);
}

void Ellipsoid::set_A_B(const double parameters[12])
{
    // set A and B
    Ellipsoid::A.set(0, 0, parameters[0]);
    Ellipsoid::A.set(0, 1, parameters[1]);
    Ellipsoid::A.set(0, 2, parameters[2]);
    Ellipsoid::A.set(1, 0, parameters[3]);
    Ellipsoid::A.set(1, 1, parameters[4]);
    Ellipsoid::A.set(1, 2, parameters[5]);
    Ellipsoid::A.set(2, 0, parameters[6]);
    Ellipsoid::A.set(2, 1, parameters[7]);
    Ellipsoid::A.set(2, 2, parameters[8]);
    Ellipsoid::B.set(0, parameters[9]);
    Ellipsoid::B.set(1, parameters[10]);
    Ellipsoid::B.set(2, parameters[11]);
}

void Ellipsoid::set_A_B(const double radius, const Vector gains, const Matrix nonOrtho, const Vector offsets)
{
    Ellipsoid::radius = radius;
    Ellipsoid::gains = gains;
    Ellipsoid::nonOrtho = nonOrtho;
    Ellipsoid::offsets = offsets;

    // set A and B
    Ellipsoid::A.set(0, 0, radius * gains(0) * nonOrtho(0, 0));
    Ellipsoid::A.set(0, 1, radius * gains(0) * nonOrtho(0, 1));
    Ellipsoid::A.set(0, 2, radius * gains(0) * nonOrtho(0, 2));
    Ellipsoid::A.set(1, 0, radius * gains(1) * nonOrtho(1, 0));
    Ellipsoid::A.set(1, 1, radius * gains(1) * nonOrtho(1, 1));
    Ellipsoid::A.set(1, 2, radius * gains(1) * nonOrtho(1, 2));
    Ellipsoid::A.set(2, 0, radius * gains(2) * nonOrtho(2, 0));
    Ellipsoid::A.set(2, 1, radius * gains(2) * nonOrtho(2, 1));
    Ellipsoid::A.set(2, 2, radius * gains(2) * nonOrtho(2, 2));
    Ellipsoid::B = offsets;
}

const double *Ellipsoid::fit_basic()
{

    // here, we suppose that axis are aligned with the world axis (and orthogonal)
    // do the average of the points (to find approx of B)
    // and the average of gain
    double averageX = 0;
    double averageY = 0;
    double averageZ = 0;
    double maxX = -1.7976931348623158e+308, maxY = maxX, maxZ = maxX;
    double minX = 1.7976931348623158e+308, minY = minX, minZ = minX;
    for (unsigned int i = 0; i < n; i++)
    {
        averageX += points[i * 3];
        averageY += points[i * 3 + 1];
        averageZ += points[i * 3 + 2];
        if (points[i * 3] > maxX)
            maxX = points[i * 3];
        if (points[i * 3] < minX)
            minX = points[i * 3];
        if (points[i * 3 + 1] > maxY)
            maxY = points[i * 3 + 1];
        if (points[i * 3 + 1] < minY)
            minY = points[i * 3 + 1];
        if (points[i * 3 + 2] > maxZ)
            maxZ = points[i * 3 + 2];
        if (points[i * 3 + 2] < minZ)
            minZ = points[i * 3 + 2];
    }
    averageX /= n;
    averageY /= n;
    averageZ /= n;

    const double gainX = (maxX - minX) / 2;
    const double gainY = (maxY - minY) / 2;
    const double gainZ = (maxZ - minZ) / 2;

    double *parameters = new double[12];
    parameters[0] = gainX;
    parameters[1] = 0;
    parameters[2] = 0;
    parameters[3] = 0;
    parameters[4] = gainY;
    parameters[5] = 0;
    parameters[6] = 0;
    parameters[7] = 0;
    parameters[8] = gainZ;
    parameters[9] = averageX;
    parameters[10] = averageY;
    parameters[11] = averageZ;

    set_A_B(parameters);

    return parameters;
}

const double *Ellipsoid::fit_grad_descent()
{

    const double *initialparameters = Ellipsoid::fit_basic();
    const double *parameters = gradDescent(Ellipsoid::cost_funtion, initialparameters, 12, 1e-4, 30);
    set_A_B(parameters);
    minimum_cost = Ellipsoid::cost_funtion(parameters);
    return parameters;
}

const double *Ellipsoid::rotation_optimization()
{
    // here, we want to minimize the angles between the axis of the ellipsoid and the world axis
    // just find the unique quaternion that minimize the sum of the square of the angles
    // we suppose that the ellipsoid is already fitted

    const double initialparameters[4] = {1, 0, 0, 0}; // identity quaternion
    const double *parameters = gradDescent(Ellipsoid::angular_cost_funtion, initialparameters, 4, 1e-4, 2);
    Quaternion q(parameters[0], parameters[1], parameters[2], parameters[3]);
    q=q.normalize().inverse();
    //q=Quaternion();
    Matrix newNonOrtho = q.rotation_matrix().transpose()*Ellipsoid::nonOrtho*q.rotation_matrix();
    std::cout << "q: " << q.to_str() << std::endl;
    std::cout << "NonOrtho: " << nonOrtho.to_str() << std::endl;
    std::cout << "newNonOrtho: " << newNonOrtho.to_str() << std::endl;
    //Ellipsoid::set_A_B(Ellipsoid::radius, Ellipsoid::gains, newNonOrtho, Ellipsoid::offsets);

    minimum_rotation_cost = Ellipsoid::angular_cost_funtion(parameters);
    return parameters;
}

const double Ellipsoid::cost_funtion(const double parameters[12])
{
    Matrix AA(3, 3);
    Vector BB(3);
    AA.set(0, 0, parameters[0]);
    AA.set(0, 1, parameters[1]);
    AA.set(0, 2, parameters[2]);
    AA.set(1, 0, parameters[3]);
    AA.set(1, 1, parameters[4]);
    AA.set(1, 2, parameters[5]);
    AA.set(2, 0, parameters[6]);
    AA.set(2, 1, parameters[7]);
    AA.set(2, 2, parameters[8]);
    BB.set(0, parameters[9]);
    BB.set(1, parameters[10]);
    BB.set(2, parameters[11]);

    double ret = 0;

    for (unsigned int i = 0; i < n; i++)
    {
        Vector x(3);
        x.set(0, points[3 * i]);
        x.set(1, points[3 * i + 1]);
        x.set(2, points[3 * i + 2]);

        Vector R = AA.inverse() * (x - BB);
        double norm = R.norm();
        double error = norm - 1; // the distance between the point and the sphere surface
        ret += (error * error) / n;
    }
    return ret;
}

const double Ellipsoid::angular_cost_funtion(const double parameters[4])
{
    explicit_A_B();
    Vector axis_X(3);
    axis_X.set(0, 1);
    Vector axis_Y(3);
    axis_Y.set(1, 1);
    Vector axis_Z(3);
    axis_Z.set(2, 1);

    Quaternion q;
    q.a = parameters[0];
    q.b = parameters[1];
    q.c = parameters[2];
    q.d = parameters[3];
    q = q.normalize();

    Matrix R = q.rotation_matrix();
    Vector new_axis_X = R * axis_X;
    Vector new_axis_Y = R * axis_Y;
    Vector new_axis_Z = R * axis_Z;

    Vector current_axis_X(3);
    Vector current_axis_Y(3);
    Vector current_axis_Z(3);

    current_axis_X.set(0, nonOrtho(0, 0));
    current_axis_X.set(1, nonOrtho(0, 1));
    current_axis_X.set(2, nonOrtho(0, 2));

    current_axis_Y.set(0, nonOrtho(1, 0));
    current_axis_Y.set(1, nonOrtho(1, 1));
    current_axis_Y.set(2, nonOrtho(1, 2));

    current_axis_Z.set(0, nonOrtho(2, 0));
    current_axis_Z.set(1, nonOrtho(2, 1));
    current_axis_Z.set(2, nonOrtho(2, 2));

    // the sum of the square of the angles
    // angle between 2 vectors are : acos((u.v)/(|u||v|))
    // but |u|=|v|=1
    // so : theta = acos(u.v)
    // and acos(u.v)^2 ~= 2 - 2(u.v) at small angle (at (u.v) close to 1)

    double angle_X_s;
    double angle_Y_s;
    double angle_Z_s;

    /*
    angle_X_s = 2 - 2 * (dot(new_axis_X, current_axis_X));
    angle_Y_s = 2 - 2 * (dot(new_axis_Y, current_axis_Y));
    angle_Z_s = 2 - 2 * (dot(new_axis_Z, current_axis_Z));
    */
   
    angle_X_s = acos(dot(new_axis_X, current_axis_X)) * acos(dot(new_axis_X, current_axis_X));
    angle_Y_s = acos(dot(new_axis_Y, current_axis_Y)) * acos(dot(new_axis_Y, current_axis_Y));
    angle_Z_s = acos(dot(new_axis_Z, current_axis_Z)) * acos(dot(new_axis_Z, current_axis_Z));

    return (angle_X_s + angle_Y_s + angle_Z_s) / 3.0;
}