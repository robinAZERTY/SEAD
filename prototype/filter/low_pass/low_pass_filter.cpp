/*
cree par : robinAZERTY
version du : 18/09/2022
*/
#pragma once

#include "low_pass_filter.h"
#include "math.h"

// https://en.wikipedia.org/wiki/Low-pass_filter#Discrete-time_realization

low_pass_filter::low_pass_filter()
{
    *this = low_pass_filter();
}

low_pass_filter::low_pass_filter(const unsigned short &order, const doubleORfloat &cut_off_frequency, const doubleORfloat &sampling_frequency)
{
    this->set_order(order);
    this->set_cut_off_frequency(cut_off_frequency);
    this->set_sampling_frequency(sampling_frequency);
    compute_coefficients(order, cut_off_frequency, sampling_frequency);
}

low_pass_filter::~low_pass_filter()
{
}

void low_pass_filter::set_sampling_frequency(const doubleORfloat &sampling_frequency)
{
    this->sampling_frequency = sampling_frequency;
    this->compute_coefficients(this->order, this->cut_off_frequency, this->sampling_frequency);
}

void low_pass_filter::set_cut_off_frequency(const doubleORfloat &cut_off_frequency)
{
    this->cut_off_frequency = cut_off_frequency;
    this->compute_coefficients(this->order, this->cut_off_frequency, this->sampling_frequency);
}

void low_pass_filter::set_order(const unsigned short &order)
{
    this->order = order;
    this->compute_coefficients(this->order, this->cut_off_frequency, this->sampling_frequency);
}

#define M_PI 3.14159265358979323846

void low_pass_filter::compute_coefficients(const unsigned short &order, const doubleORfloat &cut_off_frequency, const doubleORfloat &sampling_frequency)
{
    if (order == 1)
    {
        doubleORfloat RC = 1 / (2 * M_PI * cut_off_frequency);
        doubleORfloat alpha = 1 - RC / (RC + 1 / sampling_frequency);
        this->M = 1;
        this->N = 1;
        delete[] this->a;
        delete[] this->b;
        this->a = new doubleORfloat[1];
        this->b = new doubleORfloat[1];
        this->a[0] = alpha - 1;
        this->b[0] = alpha;
    }

    /*
    doubleORfloat omega = 2 * M_PI * cut_off_frequency / sampling_frequency;
    doubleORfloat alpha = sin(omega) / (2 * order);
    doubleORfloat k = (1 - cos(omega)) / 2;
    this->b[0] = k;
    for (unsigned short i = 1; i < this->N; i++)
    {
        this->b[i] = k * cos((2 * M_PI * i) / (2 * order));
    }
    this->b[0] = this->b[0] + alpha;
    this->b[order] = this->b[order] + alpha;
    doubleORfloat sum = 0;
    for (unsigned short i = 0; i < this->N; i++)
    {
        sum += this->b[i];
    }
    for (unsigned short i = 0; i < this->N; i++)
    {
        this->b[i] = this->b[i] / sum;
    }
    this->x = new doubleORfloat[this->N];
    for (unsigned short i = 0; i < this->N; i++)
    {
        this->x[i] = 0;
    }
    this->y = new doubleORfloat[this->M];
    for (unsigned short i = 0; i < this->M; i++)
    {
        this->y[i] = 0;
    }
    */
}