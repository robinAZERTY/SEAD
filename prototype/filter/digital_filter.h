/*
cree par : robinAZERTY
version du : 18/09/2022
*/

#define doubleORfloat double

//https://en.wikipedia.org/wiki/Digital_filter
#pragma once

class digit_filter
{
public:
    digit_filter();
    digit_filter(const unsigned short &M, const doubleORfloat *a, const unsigned short &N, const doubleORfloat *b);//can be a IIR or FIR filter
    ~digit_filter();

    void compute(const doubleORfloat &input);
    const doubleORfloat &get_output() const;
    const doubleORfloat &get_y(const unsigned short &i) const;
    const doubleORfloat &get_x(const unsigned short &i) const;
    
private:
    unsigned short M;
    doubleORfloat *a;
    unsigned short N;
    doubleORfloat *b;

    doubleORfloat *x;
    doubleORfloat *y;
};
