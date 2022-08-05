#ifndef matrix_h
#define matrix_h

//creat a Matrix class
//Matrix is a class that can be used to create a matrix of any size
//the class use a 2D array to store the data

#include <math.h>
//#include <iostream>

class Matrix
{
    public:
        Matrix();
        Matrix(int rows, int cols);
        Matrix(const Matrix& other);
        ~Matrix();

        void fill(const float &value);
        void append_rows(Matrix& other);
        void append_cols(Matrix& other);

        Matrix& operator=(const Matrix& other);
        Matrix operator+(const Matrix& other);
        Matrix operator-(const Matrix& other);
        Matrix operator*(const Matrix& other);
        Matrix operator*(const float& other);
        Matrix operator/(const float& other);
        Matrix operator-();

        Matrix operator+= (const Matrix& other)
        {
            return *this = *this + other;
        }
        Matrix operator-= (const Matrix& other)
        {
            return *this = *this - other;
        }
        Matrix operator*= (const Matrix& other)
        {
            return *this = *this * other;
        }
        Matrix operator*= (const float& other)
        {
            return *this = *this * other;
        }
        Matrix operator/= (const float& other) 
        {
            return *this = *this / other;
        }
        
        Matrix eigenvalue();
        Matrix eigenvector();

        Matrix transpose();
        float determinant();
        Matrix inverse();
        float trace();
        int get_rows();
        int get_cols();
        //void print();
        float **data;

    private:
    
        int rows;
        int cols;
};
#include "matrix.cpp"
#endif