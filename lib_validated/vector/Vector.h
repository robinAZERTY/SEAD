#pragma once

#include "matrix.cpp"

class Vector : public Matrix
{
public:
    Vector();
    Vector(const unsigned int &dim);

    const Vector normalize() const;
    const float norm() const;
    void set(const unsigned int &index, const double &value);

    //operateur ()
    const double &operator()(const unsigned int &index)const;

    //operateur = liste de double
    Vector &operator=(const double* data);
};