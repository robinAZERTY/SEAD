/*
cree par : robinAZERTY
version du : 18/09/2022
*/

#define is_VALIDATED false

//https://en.wikipedia.org/wiki/Digital_filter
#pragma once
#define doubleORfloat double

#if !is_VALIDATED
#include "../../..\validated\tools\FIFO\FIFO.cpp"

#else
#include "../../..\tools\FIFO\FIFO.h"
#endif


class digital_filter
{
public:
    digital_filter();
    digital_filter(const unsigned short &M, const doubleORfloat *a, const unsigned short &N, const doubleORfloat *b);//can be a IIR or FIR filter
    ~digital_filter();

    void compute(const doubleORfloat &input);
    const doubleORfloat &get_output() const;
    const doubleORfloat &get_y(const unsigned short &i) const;
    const doubleORfloat &get_x(const unsigned short &i) const;
    
protected:
    bool inited=false;

    unsigned short M;
    doubleORfloat *a;
    unsigned short N;
    doubleORfloat *b;

    FIFO_double x;
    FIFO_double y;
};
