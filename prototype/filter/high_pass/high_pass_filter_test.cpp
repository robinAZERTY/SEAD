/*
cree par : robinAZERTY
version du : 18/09/2022
*/

#include "high_pass_filter.cpp"
#include <iostream>
using namespace std;

void visualize()
{
    cout << "visualize" << endl;
    //creat a high pass filter with order 1, cut off frequency 1 and sampling frequency 10
    high_pass_filter my_high_pass_filter(1, 1, 10);

    
    //creat a impluse signal with N samples
    const unsigned short N = 30;
    doubleORfloat signal[N];
    signal[0] = 1;
    my_high_pass_filter.init(signal[0]);
    for (unsigned short i = 1; i < N; i++)
    {
        signal[i] = 0;
    }


    //filter the signal
    
    for (unsigned short i = 0; i < N; i++)
    {   
        cout<<signal[i]<<"\t";
        my_high_pass_filter.compute(signal[i]);
        cout <<"\t"<< my_high_pass_filter.get_output() << endl;
    }
    
}

int main()
{
    visualize();
    return 0;
}