/*
cree par : robinAZERTY
version du : 19/09/2022
*/

#pragma once

class FIFO_double
{
private:
    unsigned short size;
    unsigned short head;
    unsigned short hANDs;
    double *buffer;
public:
    FIFO_double();
    FIFO_double(const unsigned short &size);
    ~FIFO_double();
    void init(const unsigned short &size);
    void push(const double &input);
    double operator[](const unsigned short &index);
};