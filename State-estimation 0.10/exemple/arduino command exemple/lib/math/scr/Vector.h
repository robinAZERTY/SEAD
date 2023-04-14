#ifndef VECTOR_H
#define VECTOR_H
#include "matrix.h" // when this header is not a prototype

class Vector : public Matrix
{
public:
    Vector();
    Vector(const unsigned int &dim);
    Vector(const Matrix &m);
    ~Vector(){};

    const Vector normalize() const;
    const float norm() const;
    virtual void set(const unsigned int &index, const double &value);
    virtual void resize(const unsigned int &dim);
    //operateur ()
    const double &operator()(const unsigned int &index)const;

    //operateur = liste de double
    Vector &operator=(const double* data);
};
#endif