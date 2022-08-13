#include "matrix.h"

Matrix::Matrix()
{
    rows = 0;
    cols = 0;
    Data = 0;
}

Matrix::Matrix(const unsigned int &nb_rows, const unsigned int &nb_cols)
{
    rows = nb_rows;
    cols = nb_cols;
    Data = new double *[rows];
    for (int i = 0; i < rows; i++)
    {
        Data[i] = new double[cols];
    }
    this->fill(0);
}

void Matrix::set_description(const string description)
{
    this->description = description;
}

string Matrix::to_str() const
{
    string ret = "";
    ret += description + " :\n";
    for (int i = 0; i < rows; i++)
    {
        string str_line = "";
        for (int j = 0; j < cols; j++)
        {
            ret += to_string(Data[i][j]) + "  ";
        }
        ret += "\n";
    }
    return ret;
}

Matrix::Matrix(const Matrix &other)
{
    *this = other;
}

Matrix::~Matrix()
{
    for (int i = 0; i < rows; i++)
    {
        delete[] Data[i];
    }
    delete[] Data;
}

void Matrix::resize(const int &nb_new_rows, const int &nb_new_cols)
{
    // on crée une Matrice retoraire
    Matrix ret(nb_new_rows, nb_new_cols);

    int nb_row_inter = nb_new_rows < rows ? nb_new_rows : rows;
    int nb_col_inter = nb_new_cols < cols ? nb_new_cols : cols;

    // on copie les données de la matrice dans la matrice retoraire
    for (int i = 0; i < nb_row_inter; i++)
    {
        for (int j = 0; j < nb_col_inter; j++)
        {
            ret.Data[i][j] = Data[i][j];
        }
    }
    *this = ret;
}

int Matrix::get_nb_rows() const
{
    return rows;
}

int Matrix::get_nb_cols() const
{
    return cols;
}

double **Matrix::data() const
{
    return Data;
}

Matrix Matrix::transpose() const
{
    Matrix ret(cols, rows);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            ret.Data[j][i] = Data[i][j];
        }
    }
    return ret;
}

double Matrix::trace() const
{
    double ret = 0;
    for (int i = 0; i < rows; i++)
    {
        ret += Data[i][i];
    }
    return ret;
}

double Matrix::determinant() const
{

    if (rows != cols)
    {
        return 0;
    }
    if (rows == 1)
    {
        return Data[0][0];
    }
    if (rows == 2)
    {
        return Data[0][0] * Data[1][1] - Data[0][1] * Data[1][0];
    }
    float ret = 0;
    for (int i = 0; i < cols; i++)
    {
        Matrix minor = Matrix(rows - 1, cols - 1);
        for (int j = 1; j < rows; j++)
        {
            for (int k = 0; k < i; k++)
            {
                minor.Data[j - 1][k] = Data[j][k];
            }
            for (int k = i + 1; k < cols; k++)
            {
                minor.Data[j - 1][k - 1] = Data[j][k];
            }
        }
        ret += Data[0][i] * pow(-1, i) * minor.determinant();
    }
    return ret;
}

Matrix Matrix::inverse() const
{
    if (rows != cols)
    {
        return Matrix(0, 0);
    }
    if (rows == 1)
    {
        Matrix ret(1, 1);
        ret.Data[0][0] = 1 / Data[0][0];
        return ret;
    }
    if (rows == 2)
    {
        Matrix ret(2, 2);
        ret.Data[0][0] = Data[1][1];
        ret.Data[0][1] = -Data[0][1];
        ret.Data[1][0] = -Data[1][0];
        ret.Data[1][1] = Data[0][0];
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
                minor.Data[k][0] = Data[k][0];
            }
            for (int k = i + 1; k < rows; k++)
            {
                minor.Data[k - 1][0] = Data[k][0];
            }
            for (int k = 1; k < j; k++)
            {
                minor.Data[0][k - 1] = Data[0][k];
            }
            for (int k = j + 1; k < cols; k++)
            {
                minor.Data[0][k - 1] = Data[0][k];
            }
            ret.Data[i][j] = pow(-1, i + j) * minor.determinant();
        }
    }
    return ret.transpose() * (1 / determinant());
}

void Matrix::set(const unsigned int &row, const unsigned int &col, const double &data)
{
    Data[row][col] = data;
}

void Matrix::fill(const double &data)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            Data[i][j] = data;
        }
    }
}
// operator =
Matrix &Matrix::operator=(const Matrix &other)
{
    description = other.description;
    rows = other.rows;
    cols = other.cols;
    Data = new double *[rows];
    for (int i = 0; i < rows; i++)
    {
        Data[i] = new double[cols];
    }
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            Data[i][j] = other.Data[i][j];
        }
    }
    return *this;
}

Matrix Matrix::operator+(const Matrix &other) const
{
    if (rows != other.rows || cols != other.cols)
    {
        Matrix ret;
        return ret;
    }
    Matrix ret(rows, cols);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            ret.Data[i][j] = Data[i][j] + other.Data[i][j];
        }
    }
    return ret;
}

Matrix Matrix::operator*(const Matrix &other) const
{
    if (cols != other.rows)
    {
        Matrix ret;
        return ret;
    }
    Matrix ret(rows, other.cols);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < other.cols; j++)
        {
            double sum = 0;
            for (int k = 0; k < cols; k++)
            {
                sum += Data[i][k] * other.Data[k][j];
            }
            ret.Data[i][j] = sum;
        }
    }
    return ret;
}

Matrix Matrix::operator*(const double &other) const
{
    Matrix ret(rows, cols);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            ret.Data[i][j] = Data[i][j] * other;
        }
    }
    return ret;
}

Matrix Matrix::operator-(const Matrix &other) const
{
    Matrix ret = *this + other * (-1);
    return ret;
}

Matrix Matrix::operator/(const double &other) const
{
    Matrix ret = *this * (1 / other);
    return ret;
}

Matrix Matrix::operator+=(const Matrix &other)
{
    *this = *this + other;
    return *this;
}

Matrix Matrix::operator*=(const Matrix &other)
{
    *this = *this * other;
    return *this;
}

Matrix Matrix::operator*=(const double &other)
{
    *this = *this * other;
    return *this;
}

Matrix Matrix::operator-=(const Matrix &other)
{
    *this = *this - other;
    return *this;
}

Matrix Matrix::operator/=(const double &other)
{
    *this = *this / other;
    return *this;
}

bool Matrix::operator==(const Matrix &other) const
{
    if (rows != other.rows || cols != other.cols)
    {
        return false;
    }
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (Data[i][j] != other.Data[i][j])
            {
                return false;
            }
        }
    }
    return true;
}

bool Matrix::operator!=(const Matrix &other) const
{
    return !(*this == other);
}

// access operator
double &Matrix::operator()(const unsigned int &row, const unsigned int &col) const
{
    // verify if out of range
    if(row>rows||col>cols)
    {
        throw "index out of range";
    }

    return Data[row][col];
}

// fonction non membre de la classe Matrix

Matrix IdentityMatrix(const unsigned int &size)
{
    Matrix ret(size, size);
    for (int i = 0; i < size; i++)
    {
        ret.set(i, i, 1);
    }
    return ret;
}