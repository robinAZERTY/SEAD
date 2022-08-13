#include "Vector.h"

Vector::Vector():Matrix()
{
this->description="Vector";
}

Vector::Vector(const unsigned int &dim) : Matrix(dim, 1)
{
    this->description="Vector";
}

//operateur ()
const double &Vector::operator()(const unsigned int &index) const
{
    if(index>this->rows)
    {
        throw "out of range";
    }

    return Data[index][0];
}

void Vector::set(const unsigned int &index, const double &value)
{
    if(index>this->rows)
    {
        throw "out of range";
    }
    Data[index][0] = value;
}

//operateur = liste de double
Vector &Vector::operator=(const double* data)
{
    for(unsigned int i=0; i<this->rows; i++)
    {
        this->Data[i][0] = data[i];
    }
    return *this;
}

//norm 
const float Vector::norm() const
{
    float norm = 0;
    for(unsigned int i=0; i<this->rows; i++)
    {
        norm += this->Data[i][0]*this->Data[i][0];
    }
    return sqrt(norm);
}

//normalize
const Vector Vector::normalize() const
{
    Vector v = *this;
    float norm = v.norm();
    for(unsigned int i=0; i<this->rows; i++)
    {
        v.Data[i][0] /= norm;
    }
    return v;
}

//dot
const float dot(const Vector &v1, const Vector &v2)
{
    if(v1.get_nb_rows() != v2.get_nb_rows())
    {
        throw "vectors must have the same size";
    }
    float dot = 0;
    for(unsigned int i=0; i<v1.get_nb_rows(); i++)
    {
        dot += v1(i)*v2(i);
    }
    return dot;
}

//cross
const Vector cross(const Vector &v1, const Vector &v2)
{
    //check if the vectors are 3D
    if(v1.get_nb_rows() != 3 || v2.get_nb_rows() != 3)
    {
        throw "vectors must be 3D";
    }
    Vector v = Vector(3);
    double data[3];
    data[0] = v1(1)*v2(2) - v1(2)*v2(1);
    data[1] = v1(2)*v2(0) - v1(0)*v2(2);
    data[2] = v1(0)*v2(1) - v1(1)*v2(0);
    v = data;
    return v;
}