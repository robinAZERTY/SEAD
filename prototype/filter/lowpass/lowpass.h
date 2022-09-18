/*
cree : robinAZERTY
date : 17/09/2022
*/

#pragma once

class LowPass {

public : 
    LowPass(const double &cutOffFreq, const double &sampleRate);
    ~LowPass();
    const double process(const double &input);
    const double get_output();
    const double get_alpha();
    void setCutOffFreq(const double &cutOffFreq);
    void setSampleRate(const double &sampleRate);
private : 
    bool inited=false;
    double cutOffFreq;
    double sampleRate;
    double output;
    double alpha;
};