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
    //https://en.wikipedia.org/wiki/Low-pass_filter
    const unsigned short M = 2;
    const double a[M] = {1,-0.5};
    const unsigned short N = 1;
    const double b[N] = {0.5};
    digital_filter low_pass_filter(M,a,N,b);
    low_pass_filter.compute(1);
    for (unsigned short i = 0; i < 10; i++)
    {
        cout<<low_pass_filter.get_output()<<endl; 
        low_pass_filter.compute(0);
    }
    cout<<"low pass filter visu ended"<<endl;
}
int main()
{
    visualize();
    return 0;
}