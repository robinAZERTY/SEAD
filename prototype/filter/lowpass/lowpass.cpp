/*
cree : robinAZERTY
date : 17/09/2022
*/

#pragma once
#include "lowpass.h"

LowPass::LowPass(const double &cutOffFreq, const double &sampleRate)
{
    this->cutOffFreq = cutOffFreq;
    this->sampleRate = sampleRate;
    this->output = 0;
    this->alpha = this->cutOffFreq / (this->cutOffFreq + this->sampleRate);
}

LowPass::~LowPass()
{
}

const double LowPass::process(const double &input)
{
    if (!inited)
    {
        this->output = input;
        inited = true;
        return this->output;
    }

    this->output = this->output + this->alpha * (input - this->output);
    return this->output;
}

const double LowPass::get_output()
{
    return this->output;
}

const double LowPass::get_alpha()
{
    return this->alpha;
}

void LowPass::setCutOffFreq(const double &cutOffFreq)
{
    this->cutOffFreq = cutOffFreq;
    this->alpha = this->cutOffFreq / (this->cutOffFreq + this->sampleRate);
}

void LowPass::setSampleRate(const double &sampleRate)
{
    this->sampleRate = sampleRate;
    this->alpha = this->cutOffFreq / (this->cutOffFreq + this->sampleRate);
}