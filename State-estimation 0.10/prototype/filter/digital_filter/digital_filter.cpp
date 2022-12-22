/*
cree par : robinAZERTY
version du : 18/09/2022
*/

#pragma once
#include "digital_filter.h"

digital_filter::digital_filter()
{
    this->M = 0;
    this->a = nullptr;
    this->N = 0;
    this->b = nullptr;
}

digital_filter::digital_filter(const unsigned short &M, const doubleORfloat *a, const unsigned short &N, const doubleORfloat *b)
{
    this->M = M;
    this->a = new doubleORfloat[M];
    for (unsigned short i = 0; i < M; i++)
    {
        this->a[i] = a[i];
    }
    this->N = N;
    this->b = new doubleORfloat[N];
    for (unsigned short i = 0; i < N; i++)
    {
        this->b[i] = b[i];
    }
    this->x = FIFO_double(N);
    this->y = FIFO_double(M);
}

digital_filter::~digital_filter()
{
    delete[] this->a;
    delete[] this->b;
}

void digital_filter::compute(const doubleORfloat &input)
{   
    
    if(!inited)
    {
        for(unsigned short i=0;i<N;i++)
        {
            this->x.push(input);
        }

        for(unsigned short i=0;i<M;i++)
        {
            this->y.push(input);
        }
        inited=true;
    }

    this->x.push(input);
    this->y.push(0);
    
    
    for (unsigned short i = 0; i < this->N; i++)
    {
        this->y[0] += this->b[i] * this->x[i];
    }
    for (unsigned short i = 1; i < this->M; i++)
    {
        this->y[0] -= this->a[i] * this->y[i];
    }
    this->y[0] /= this->a[0];
    this->output = &this->y[0];
    
}

const doubleORfloat &digital_filter::get_output() const
{   
    return *this->output;
}