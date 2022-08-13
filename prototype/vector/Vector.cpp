#include "Vector.h"

Vector::Vector(unsigned int &dim) : Matrix(dim, 1)
{
}

//operateur []
const double &Vector::operator[](const unsigned int &index) const
{
    return this->data()[index][0];
}