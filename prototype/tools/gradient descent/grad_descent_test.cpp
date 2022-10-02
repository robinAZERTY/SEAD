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
    inline static double p[2];

    static const double my_function2(const double x[2])
    {   
        return cos(p[0]*x[0]+1) * sin(x[1]+p[1]);
        // - inf divergent example
       // return -x[0]*x[0]-x[1]*x[1];
    }

    void fit()
    {
        const double *ret = gradDescent(my_function2, p, 2, 1e-10, 0.2);
        for (unsigned int i = 0; i <= 2; i++)
        {
            std::cout << ret[i] << std::endl;
        }
    }
};


void visualize2()
{
    my_struct my_struc;
    my_struc.p[0] = 1;
    my_struc.p[1] = 0;
    const unsigned int n = 2;
    double x[n] = {0.1, -0.1};

    my_struc.fit();
}

int main()
{
    //visualize();
    visualize2();
    return 0;
}