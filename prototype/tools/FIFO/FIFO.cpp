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
    this->tail = 0;
    this->buffer = nullptr;
}

FIFO_double::FIFO_double(const unsigned short &size)
{
    this->size = size;
    this->head = 0;
    this->tail = 0;
    this->buffer = new double[size];
}

FIFO_double::~FIFO_double()
{
    delete[] this->buffer;
}

void FIFO_double::init(const unsigned short &size)
{
    this->size = size;
    this->head = 0;
    this->tail = 0;
    this->buffer = new double[size];
}

void FIFO_double::push(const double &input)
{
    this->buffer[this->head] = input;
    this->head = (this->head + 1) % this->size;
    if (this->head == this->tail)
    {
        this->tail = (this->tail + 1) % this->size;
    }
}

double FIFO_double::operator[](const short &index)
{
// index = 0 => last element
// index = -1 => last but one element
// index = -2 => last but two element
// ...
    return this->buffer[(this->head + index + this->size-1) % this->size];
}