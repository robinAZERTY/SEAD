#include <iostream>
#include "../prototype\matrix\matrix.h"

using namespace std;

void print(Matrix& m)
{
    for (int i = 0; i < m.get_rows(); i++)
    {
        for (int j = 0; j < m.get_cols(); j++)
        {
            cout << m.data[i][j] << " ";
        }
        cout << endl;
    }
}

//test all the functions

void construction_test()
{
    cout << "Construction test" << endl;
    cout << "CTOR0" << endl;
    Matrix m0 = Matrix();
    print(m0);

    cout << "CTOR1" << endl;
    Matrix m1 = Matrix(2, 3);
    print(m1);

    cout << "CTOR2" << endl;
    Matrix m2 = Matrix(m1);
    print(m2);
}

void fill_test()
{
    cout << "Fill test" << endl;
    Matrix m1 = Matrix(2, 3);
    m1.fill(1);
    print(m1);

    cout << "add rows test" << endl;
    Matrix m2 = Matrix(1, 3);
    m1.append_rows(m2);
    print(m1);

    cout << "add cols test" << endl;
    Matrix m3 = Matrix(3, 1);
    m1.append_cols(m3);
    print(m1);
}

void operator_test()
{
    cout << "Operator test" << endl;
    Matrix m1 = Matrix(2, 3);
    Matrix m2 = Matrix(3, 3);
    m1.fill(1);
    m2.fill(2);
    print(m1);
    print(m2);
    cout << "operator =" << endl;
    Matrix m3 = m1 ;
    print(m3);


}


int main()
{   
    construction_test();
    fill_test();
    operator_test();
    return 0;
}