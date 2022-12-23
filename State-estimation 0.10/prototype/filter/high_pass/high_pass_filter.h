/*
cree par : robinAZERTY
version du : 18/09/2022
*/

#pragma once

#if false//true if the code is validated
#include "../digital_filter/digital_filter.cpp"
#else
#include "..\..\../validated/filter/digital_filter/digital_filter.cpp"
#endif


class high_pass_filter : public digital_filter
{
    public :
        high_pass_filter();
        high_pass_filter(const unsigned short &order, const doubleORfloat &cut_off_frequency, const doubleORfloat &sampling_frequency);
        ~high_pass_filter();

        void set_sampling_frequency(const doubleORfloat &sampling_frequency);
        void set_cut_off_frequency(const doubleORfloat &cut_off_frequency);
        void set_order(const unsigned short &order);
        void init(const doubleORfloat &input);
        void compute_coefficients(const unsigned short &order, const doubleORfloat &cut_off_frequency, const doubleORfloat &sampling_frequency);

    private :
        unsigned short order=0;
        doubleORfloat cut_off_frequency=0;
        doubleORfloat sampling_frequency=0;

};