#include "matrix.h"


Matrix::Matrix()
{
    rows = 0;
    cols = 0;
    data = NULL;
}

Matrix::~Matrix()
{
    if (data != NULL)
    {
        for (int i = 0; i < rows; i++)
        {
            delete[] data[i];
        }
        delete[] data;
    }
}

Matrix::Matrix(int rows, int cols)
{
    this->rows = rows;
    this->cols = cols;
    data = new float*[rows];
    for (int i = 0; i < rows; i++)
    {
        data[i] = new float[cols];
    }
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            data[i][j] = 0;
        }
    }
}

Matrix::Matrix(const Matrix& other)
{
    rows = other.rows;
    cols = other.cols;
    data = new float*[rows];
    for (int i = 0; i < rows; i++)
    {
        data[i] = new float[cols];
    }
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            data[i][j] = other.data[i][j];
        }
    }
}

void Matrix::fill(const float &value)
{
    for (int i = 0; i < this->get_rows(); i++)
    {
        for (int j = 0; j < this->get_cols(); j++)
        {
            this->data[i][j] = value;
        }
    }
}

void Matrix::append_rows(Matrix& other)
{
    //other must have the same number of columns as this
    if(other.get_cols()!=this->get_cols())
    {
        //throw exception
        throw "Matrix::append_rows: Matrix dimensions do not match";
    }
    else
    {   
        //create new matrix with the new number of rows
        Matrix new_matrix(this->get_rows()+other.get_rows(),this->get_cols());
        //copy the data from this to the new matrix
        for(int i=0;i<this->get_rows();i++)
        {
            for(int j=0;j<this->get_cols();j++)
            {
                new_matrix.data[i][j]=this->data[i][j];
            }
        }
        //copy the data from other to the new matrix
        for(int i=0;i<other.get_rows();i++)
        {
            for(int j=0;j<other.get_cols();j++)
            {
                new_matrix.data[i+this->get_rows()][j]=other.data[i][j];
            }
        }
        //copy the new matrix to this
        *this = new_matrix;

    }
}

void Matrix::append_cols(Matrix& other)
{
    //other must have the same number of rows as this
    if(other.get_rows()!=this->get_rows())
    {
        //throw exception
        throw "Matrix::append_cols: Matrix dimensions do not match";
    }
    else
    {   
        //create new matrix with the new number of columns
        Matrix new_matrix(this->get_rows(),this->get_cols()+other.get_cols());
        //copy the data from this to the new matrix
        for(int i=0;i<this->get_rows();i++)
        {
            for(int j=0;j<this->get_cols();j++)
            {
                new_matrix.data[i][j]=this->data[i][j];
            }
        }
        //copy the data from other to the new matrix
        for(int i=0;i<other.get_rows();i++)
        {
            for(int j=0;j<other.get_cols();j++)
            {
                new_matrix.data[i][j+this->get_cols()]=other.data[i][j];
            }
        }
        //copy the new matrix to this
        *this = new_matrix;
    }
}


Matrix& Matrix::operator=(const Matrix& other)
{
    if (this != &other)
    {
        if (data != NULL)
        {
            for (int i = 0; i < rows; i++)
            {
                delete[] data[i];
            }
            delete[] data;
        }
        rows = other.rows;
        cols = other.cols;
        data = new float*[rows];
        for (int i = 0; i < rows; i++)
        {
            data[i] = new float[cols];
        }
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                data[i][j] = other.data[i][j];
            }
        }
    }
    return *this;
}

Matrix Matrix::operator+(const Matrix& other)
{
    Matrix ret(rows, cols);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            ret.data[i][j] = data[i][j] + other.data[i][j];
        }
    }
    return ret;
}

Matrix Matrix::operator-(const Matrix& other)
{
    Matrix ret(rows, cols);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            ret.data[i][j] = data[i][j] - other.data[i][j];
        }
    }
    return ret;
}

Matrix Matrix::operator*(const Matrix& other)
{
    Matrix ret(rows, other.cols);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < other.cols; j++)
        {
            for (int k = 0; k < cols; k++)
            {
                ret.data[i][j] += data[i][k] * other.data[k][j];
            }
        }
    }
    return ret;
}
Matrix Matrix::operator*(const float& other)
{
    Matrix ret(rows, cols);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            ret.data[i][j] = data[i][j] * other;
        }
    }
    return ret;
}

Matrix Matrix::operator/(const float& other)
{
    Matrix ret(rows, cols);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            ret.data[i][j] = data[i][j] / other;
        }
    }
    return ret;
}
Matrix Matrix::operator-()
{
    Matrix ret(rows, cols);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            ret.data[i][j] = -data[i][j];
        }
    }
    return ret;
}

Matrix Matrix::transpose()
{
    Matrix ret(cols, rows);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            ret.data[j][i] = data[i][j];
        }
    }
    return ret;
}

float Matrix::determinant()
{
    if (rows != cols)
    {
        return 0;
    }
    if (rows == 1)
    {
        return data[0][0];
    }
    if (rows == 2)
    {
        return data[0][0] * data[1][1] - data[0][1] * data[1][0];
    }
    float ret = 0;
    for (int i = 0; i < cols; i++)
    {
        Matrix minor = Matrix(rows - 1, cols - 1);
        for (int j = 1; j < rows; j++)
        {
            for (int k = 0; k < i; k++)
            {
                minor.data[j - 1][k] = data[j][k];
            }
            for (int k = i + 1; k < cols; k++)
            {
                minor.data[j - 1][k - 1] = data[j][k];
            }
        }
        ret += data[0][i] * pow(-1, i) * minor.determinant();
    }
    return ret;
}




Matrix Matrix::inverse()
{
    if (rows != cols)
    {
        return Matrix(0, 0);
    }
    if (rows == 1)
    {
        Matrix ret(1, 1);
        ret.data[0][0] = 1 / data[0][0];
        return ret;
    }
    if (rows == 2)
    {
        Matrix ret(2, 2);
        ret.data[0][0] = data[1][1];
        ret.data[0][1] = -data[0][1];
        ret.data[1][0] = -data[1][0];
        ret.data[1][1] = data[0][0];
        return ret / determinant();
    }
    Matrix ret(rows, cols);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            Matrix minor = Matrix(rows - 1, cols - 1);
            for (int k = 0; k < i; k++)
            {
                minor.data[k][0] = data[k][0];
            }
            for (int k = i + 1; k < rows; k++)
            {
                minor.data[k - 1][0] = data[k][0];
            }
            for (int k = 1; k < j; k++)
            {
                minor.data[0][k - 1] = data[0][k];
            }
            for (int k = j + 1; k < cols; k++)
            {
                minor.data[0][k - 1] = data[0][k];
            }
            ret.data[i][j] = pow(-1, i + j) * minor.determinant();
        }
    }
    return ret.transpose() * (1 / determinant());
}

float Matrix::trace()
{
    if (rows != cols)
    {
        return 0;
    }
    float ret = 0;
    for (int i = 0; i < rows; i++)
    {
        ret += data[i][i];
    }
    return ret;
}


int Matrix::get_rows()
{
    return rows;
}

int Matrix::get_cols()
{
    return cols;
}


/*
void Matrix::print()
{
    std::cout << description << std::endl;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            std::cout << data[i][j] << " ";
        }
        std::cout << std::endl;
    }
}
*/