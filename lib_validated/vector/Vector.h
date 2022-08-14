#pragma once

#include "../../lib_validated\math\matrix\matrix.cpp"
//#include "matrix.cpp"

class Vector : public Matrix
{
public:
    Vector();
    Vector(const unsigned int &dim);

    const Vector normalize() const;
    const float norm() const;
    virtual void set(const unsigned int &index, const double &value);
    virtual void resize(const unsigned int &dim);
    //operateur ()
    const double &operator()(const unsigned int &index)const;

    //operateur = liste de double
    Vector &operator=(const double* data);

};
