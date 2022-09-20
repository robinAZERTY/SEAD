/*
cree par : robinAZERTY
version du : 18/09/2022
*/

#include "low_pass_filter.cpp"
#include <iostream>
using namespace std;

void visualize()
{
    cout << "visualize" << endl;
    //creat a low pass filter with order 1, cut off frequency 1 and sampling frequency 10
    low_pass_filter my_low_pass_filter(1, 1, 10);
    
    //creat a impluse signal with 50+50 samples
    doubleORfloat signal[100];
    for (unsigned short i = 0; i < 50; i++)
    {
        signal[i] = 0;
    }
    for (unsigned short i = 50; i < 100; i++)
    {
        signal[i] = 1;
    }

    //filter the signal
    for (unsigned short i = 0; i < 100; i++)
    {   
        cout<<signal[i]<<"\t";
        my_low_pass_filter.compute(signal[i]);
        cout <<" _ "<< my_low_pass_filter.get_output() << endl;
    }
    
}

int main()
{
    visualize();
    return 0;
}