/*
cree par : robinAZERTY
version du : 18/09/2022
*/
#pragma once

//https://en.wikipedia.org/wiki/Digital_filter

#define doubleORfloat double

#if true // true if the code is validated
#include "../..\tools\FIFO\FIFO.cpp"
#else
#include "../../..\validated\tools\FIFO\FIFO.cpp"
#endif



class digital_filter
{
public:
    digital_filter();
    digital_filter(const unsigned short &M, const doubleORfloat *a, const unsigned short &N, const doubleORfloat *b);//can be a IIR or FIR filter
    ~digital_filter();

    void compute(const doubleORfloat &input);
    const doubleORfloat &get_output() const;
    
//protected:
    bool inited=false;

    unsigned short M;
    doubleORfloat *a;
    unsigned short N;
    doubleORfloat *b;

    FIFO_double x;
    FIFO_double y;
    doubleORfloat* output; 
};
