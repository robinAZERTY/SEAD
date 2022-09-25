/*
cree par : robinAZERTY
version du : 25/09/2022
*/

#pragma once
#define IS_VALIDATED false

#if IS_VALIDATED
#include "../digital_filter/digital_filter.cpp"
#include "../high_pass_filter/high_pass_filter.cpp"
#include "../low_pass_filter/low_pass_filter.cpp"

#else
#include "..\..\../validated/filter/digital_filter/digital_filter.cpp"
#include "..\..\../validated/filter/high_pass/high_pass_filter.cpp"
#include "..\..\../validated/filter/low_pass/low_pass_filter.cpp"
#endif

class complementary_filter
{
public:
    complementary_filter();
    complementary_filter(const doubleORfloat &cut_off_frequency, const doubleORfloat &sampling_frequency);
    ~complementary_filter();

    void set_sampling_frequency(const doubleORfloat &sampling_frequency);
    void set_cut_off_frequency(const doubleORfloat &cut_off_frequency);
    void compute_coefficients(const doubleORfloat &cut_off_frequency, const doubleORfloat &sampling_frequency);
    void compute(const doubleORfloat &HPF_input, const doubleORfloat &LPF_input);
    doubleORfloat get_output();

private:
    void init(const doubleORfloat &input);
    doubleORfloat cut_off_frequency = 0;
    doubleORfloat sampling_frequency = 0;
    high_pass_filter HPF;
    low_pass_filter LPF;
    doubleORfloat output = 0;
    bool inited = false;
};