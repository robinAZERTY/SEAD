/*
cree par : robinAZERTY
date : 17/09/2022
*/

#pragma once

class LowHighPass {
    public : 
        LowHighPass(const double &cutOffFreq, const double &sampleRate);
        ~LowHighPass();
        const double* process(const double &input);
        const double get_output_low();
        const double get_output_high();
        const double get_alpha();
        void setCutOffFreq(const double &cutOffFreq);
        void setSampleRate(const double &sampleRate);

    private :
        bool inited=false;
        double cutOffFreq;
        double sampleRate;
        double output_low;
        double output_high;
        double alpha;
};