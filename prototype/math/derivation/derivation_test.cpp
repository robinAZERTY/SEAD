#include "derivation.h"
#include <iostream>
#include "math.h"

const double my_f(const double &x)
{
    // std::cout << "sin(" << x << ")=" << sin(x) << std::endl;
    return sin(x);
}

const double my_sin_der(const double &x, const unsigned short &n, const double &dx)
{
    return f_der_bis(my_f, x, n, dx);
}


const double real_sin_der(const double &x, const unsigned short &n)
{
    if (n % 4 == 0)
        return sin(x);
    if (n % 4 == 1)
        return cos(x);
    if (n % 4 == 2)
        return -sin(x);
    if (n % 4 == 3)
        return -cos(x);

    return nan("not modulo 4");
}


void test(double x, short n_max)
{
    
    std::cout<<"order | best_dx | best_error"<<std::endl;

    for (int i = 0; i < n_max; i++)
    {   
        double *ret;
        ret=new double[2];
        const double dx_max=2*pow((i+1)/10.0,2);
        ret=search_best_dx(my_f,real_sin_der,x,i+1,0,dx_max,dx_max/1000000);
        std::cout << (i + 1) << '\t' << ret[0] << '\t' << ret[1] <<'\t'<<dx_max<< std::endl;
    }
}
int main()
{
    test(0.3,5);
    //search_best_dx(0.2,3,0,0.1);
    // std::cout << "5nd derivate of sin(0) = " << f_der(my_function, 0, 5, 1.0e-3)<< std::endl;
    // std::cout << "5nd derivate_bis of sin(0) = " << f_der_bis(my_function, 0, 5, 1.0e-6)<< std::endl;
    return 0;
}
