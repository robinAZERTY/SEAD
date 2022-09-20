/*
cree par : robinAZERTY
version du : 18/09/2022
*/

#include "digital_filter.cpp"
#include <iostream>
using namespace std;

void visualize()
{
    //creat a low pass filter 
    const unsigned short M = 2;
    const double a[M] = {-0.9,-1};
    const unsigned short N = 2;
    const double b[N] = {0.1};
    digital_filter filter(M, a, N, b);
    //creat a pulse signal 0 to 1
    const unsigned short Ns = 100;
    double s[Ns];
    s[0] = 1;
    for (unsigned short i = 1; i < Ns; i++)
    {
        s[i] = 0;
    }

    //compute the filter
    for (unsigned short i = 0; i < Ns; i++)
    {
        filter.compute(s[i]);
        cout << filter.get_output() << endl;
    }

}
int main()
{
    visualize();
    return 0;
}