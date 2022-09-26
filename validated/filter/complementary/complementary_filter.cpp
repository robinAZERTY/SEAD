/*
cree par : robinAZERTY
version du : 25/09/2022
*/

#pragma once
#include "complementary_filter.h"

complementary_filter::complementary_filter()
{
    this->cut_off_frequency=0;
    this->sampling_frequency=0;

}

complementary_filter::complementary_filter(const doubleORfloat &cut_off_frequency, const doubleORfloat &sampling_frequency)
{
    this->cut_off_frequency=cut_off_frequency;
    this->sampling_frequency=sampling_frequency;
    this->HPF.compute_coefficients((unsigned short)1, cut_off_frequency, sampling_frequency);
    this->LPF.compute_coefficients((unsigned short)1, cut_off_frequency, sampling_frequency);
}

complementary_filter::~complementary_filter()
{
}

void complementary_filter::set_sampling_frequency(const doubleORfloat &sampling_frequency)
{
    this->sampling_frequency=sampling_frequency;
    this->HPF.set_sampling_frequency(sampling_frequency);
    this->LPF.set_sampling_frequency(sampling_frequency);
}

void complementary_filter::set_cut_off_frequency(const doubleORfloat &cut_off_frequency)
{
    this->cut_off_frequency=cut_off_frequency;
    this->HPF.set_cut_off_frequency(cut_off_frequency);
    this->LPF.set_cut_off_frequency(cut_off_frequency);
}

void complementary_filter::init(const doubleORfloat &HPF_input)
{
    this->HPF.init(HPF_input);
}

void complementary_filter::compute_coefficients(const doubleORfloat &cut_off_frequency, const doubleORfloat &sampling_frequency)
{
    this->HPF.compute_coefficients(1, cut_off_frequency, sampling_frequency);
    this->LPF.compute_coefficients(1, cut_off_frequency, sampling_frequency);
}

void complementary_filter::compute(const doubleORfloat &HPF_input, const doubleORfloat &LPF_input)
{
    if (!inited)
    {
        this->init(HPF_input);
    }
    
    this->HPF.compute(HPF_input);
    this->LPF.compute(LPF_input);
    this->output=this->LPF.get_output()+this->HPF.get_output();
}

doubleORfloat complementary_filter::get_output()
{
    return this->output;
}