/*
cree par : robinAZERTY
version du : 18/09/2022
*/

#pragma once
#include "digital_filter.h"

digit_filter::digit_filter()
{
    this->M = 0;
    this->a = nullptr;
    this->N = 0;
    this->b = nullptr;
    this->x = nullptr;
    this->y = nullptr;
}

digit_filter::digit_filter(const unsigned short &M, const doubleORfloat *a, const unsigned short &N, const doubleORfloat *b)
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
    this->x = new doubleORfloat[N];
    for (unsigned short i = 0; i < N; i++)
    {
        this->x[i] = 0;
    }
    this->y = new doubleORfloat[M];
    for (unsigned short i = 0; i < M; i++)
    {
        this->y[i] = 0;
    }
}

digit_filter::~digit_filter()
{
    delete[] this->a;
    delete[] this->b;
    delete[] this->x;
    delete[] this->y;
}

void digit_filter::compute(const doubleORfloat &input)
{
    for (unsigned short i = 0; i < this->N - 1; i++)
    {
        this->x[i] = this->x[i + 1];
    }
    this->x[this->N - 1] = input;
    for (unsigned short i = 0; i < this->M - 1; i++)
    {
        this->y[i] = this->y[i + 1];
    }
    this->y[this->M - 1] = 0;
    for (unsigned short i = 0; i < this->N; i++)
    {
        this->y[this->M - 1] += this->b[i] * this->x[i];
    }
    for (unsigned short i = 0; i < this->M - 1; i++)
    {
        this->y[this->M - 1] -= this->a[i] * this->y[i];
    }
}

const doubleORfloat &digit_filter::get_output() const
{
    return this->y[this->M - 1];
}

const doubleORfloat &digit_filter::get_y(const unsigned short &i) const
{
    return this->y[i];
}

const doubleORfloat &digit_filter::get_x(const unsigned short &i) const
{
    return this->x[i];
}