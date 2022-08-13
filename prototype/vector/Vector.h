#pragma once

#include "matrix.cpp"

class Vector : public Matrix
{
public:
    Vector(unsigned int &dim);

    const Vector normalize() const;
    const float norm() const;

    //operateur []
    const double &operator[](const unsigned int &index)const;
};
