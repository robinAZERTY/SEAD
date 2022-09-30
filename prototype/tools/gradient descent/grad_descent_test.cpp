/*
cree par : robinAZERTY
version du : 30/09/2022
*/

#include <iostream>
#include "gradient_descent.cpp"

const double my_function(const double x[2])
{
    return cos(x[0])*sin(x[1]);
}

void visualize()
{
    const unsigned int n = 2;
    double x[n] = {0.1,0.1};
    const double* ret = gradDescent(my_function,x,n,1e-10,0.2);
    for (unsigned int i = 0; i <= n; i++)
    {
        std::cout << ret[i] << std::endl;
    }
}

int main()
{
    visualize();
    return 0;
}