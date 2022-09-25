/*
cree par : robinAZERTY
version du : 18/09/2022
*/
#pragma once

#include "low_pass_filter.h"


// https://en.wikipedia.org/wiki/Low-pass_filter#Discrete-time_realization
// http://w3.cran.univ-lorraine.fr/perso/hugues.garnier/Enseignement/TdS/J-TdS_Conception_filtres_num.pdf
low_pass_filter::low_pass_filter()
{
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

void low_pass_filter::compute_coefficients(const unsigned short &order, const doubleORfloat &cut_off_frequency, const doubleORfloat &sampling_frequency)
{
    if (order != 1)
        return;

    doubleORfloat RC = 1 / (2 * 3.14159265358979323846 * cut_off_frequency);
    doubleORfloat alpha = 1 - RC / (RC + 1 / sampling_frequency);
    this->M = 2;
    this->N = 1;
    this->a = new doubleORfloat[M];
    this->b = new doubleORfloat[N];
    this->a[0] = 1.0;
    this->a[1] = alpha - 1.0;
    this->b[0] = alpha;
    this->x = FIFO_double(N);
    this->y = FIFO_double(M);
}