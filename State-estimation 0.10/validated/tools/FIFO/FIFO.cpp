/*
cree par : robinAZERTY
version du : 19/09/2022
*/

#pragma once
#include "FIFO.h"

FIFO_double::FIFO_double()
{
    this->size = 0;
    this->head = 0;
    this->buffer = nullptr;
}

FIFO_double::FIFO_double(const unsigned short &size)
{
    this->init(size);
}

FIFO_double::~FIFO_double()
{
    delete[] this->buffer;
}

void FIFO_double::init(const unsigned short &size)
{
    this->size = 0;
    this->head = 0;
    this->size = size;
    this->head = (size - 1) % size;
    this->buffer = new double[size];
    // fill by NaN
    for (unsigned short i = 0; i < size; i++)
    {
        this->buffer[i] = 0 / 0.0; // NaN
    }
}

void FIFO_double::push(const double &input)
{
    if (size > 1)
    {
        this->head = (this->head + 1) % this->size;
    }
    this->hANDs = this->head + this->size;
    this->buffer[this->head] = input;
}

double &FIFO_double::operator[](const unsigned short &index)
{
    // index = 0 => last element
    // index = 1 => last but one element
    // index = 2 => last but two element
    // ...
    // change the sign of index
    return this->buffer[(this->hANDs - index) % this->size];
}

FIFO_double &FIFO_double::operator=(const FIFO_double &input)
{
    this->size = input.size;
    this->head = input.head;
    this->hANDs = input.hANDs;
    this->buffer = new double[this->size];
    for (unsigned short i = 0; i < this->size; i++)
    {
        this->buffer[i] = input.buffer[i];
    }
    return *this;
}