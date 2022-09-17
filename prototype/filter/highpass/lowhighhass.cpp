/*
cree par : robinAZERTY
date : 17/09/2022
*/

#pragma once
#include "lowhighhass.h"

LowHighPass::LowHighPass(const double &cutOffFreq, const double &sampleRate)
{
    this->cutOffFreq = cutOffFreq;
    this->sampleRate = sampleRate;
    this->alpha = this->cutOffFreq / (this->cutOffFreq + this->sampleRate);
}

LowHighPass::~LowHighPass()
{
}

const double *LowHighPass::process(const double &input)
{
    double ret[2];
    if (!inited)
    {
        this->output_low = input;
        this->output_high = 0;
        inited = true;
    }

    this->output_low = this->output_low + this->alpha * (input - this->output_low);
    this->output_high = input - this->output_low;

    ret[0] = this->output_low;
    ret[1] = this->output_high;

    return ret;
}

const double LowHighPass::get_output_low()
{
    return this->output_low;
}

const double LowHighPass::get_output_high()
{
    return this->output_high;
}

const double LowHighPass::get_alpha()
{
    return this->alpha;
}

void LowHighPass::setCutOffFreq(const double &cutOffFreq)
{
    this->cutOffFreq = cutOffFreq;
    this->alpha = this->cutOffFreq / (this->cutOffFreq + this->sampleRate);
}

void LowHighPass::setSampleRate(const double &sampleRate)
{
    this->sampleRate = sampleRate;
    this->alpha = this->cutOffFreq / (this->cutOffFreq + this->sampleRate);
}

