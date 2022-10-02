/*
cree par : robinAZERTY
version du : 30/09/2022
*/

#include <iostream>
#include "gradient_descent.cpp"

#include <math.h>

const double my_function(const double x[2])
{
    return cos(x[0]) * sin(x[1]);
}

void visualize()
{
    const unsigned int n = 2;
    double x[n] = {0.1, 0.1};
    const double *ret = gradDescent(my_function, x, n, 1e-10, 0.2);
    for (unsigned int i = 0; i <= n; i++)
    {
        std::cout << ret[i] << std::endl;
    }
}

// minimize a function in a strucuture
// the function must be a function of the structure
struct my_struct
{
    inline static double p[1];

    static const double my_function2(const double x[1])
    {   
        //2 local minima 
        return x[0]*x[0]*x[0]*x[0] - x[0]*x[0] + 0.1*x[0];
    }

    void fit()
    {
        const double *ret = gradDescent(my_function2, p, 1, 1e-10, 0.2);
        for (unsigned int i = 0; i <= 1; i++)
        {
            std::cout << ret[i] << std::endl;
        }
    }
};


void visualize2()
{
    my_struct my_struc;
    my_struc.p[0] = -0.1;

    my_struc.fit();
}

int main()
{
    //visualize();
    visualize2();
    return 0;
}