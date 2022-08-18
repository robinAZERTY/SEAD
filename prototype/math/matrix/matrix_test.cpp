#include <iostream>
#include "matrix.cpp"

using namespace std;

void visualisation_CTOR_DTOR()
{
    cout << "visualisation_CTOR_DTOR" << endl;
    Matrix m;
    cout << m.to_str() << endl;
    Matrix m1(2, 2);
    m1.set_description("m1");
    cout << m1.to_str() << endl;
    Matrix m2(m1);
    cout << m2.to_str() << endl;
    Matrix m3;
    m1.set(0, 0, 1);
    m3 = m1;
    cout << m3.to_str() << endl;

    Matrix m4(2, 2);
    cout << m4.to_str() << endl;
}

void visualisation_resize()
{
    cout << "visualisation_resize" << endl;
    Matrix m1(2, 2);
    m1.set(1, 1, 1);
    m1.set_description("m1");
    cout << m1.to_str() << endl;
    m1.resize(3, 3);
    m1.set(2, 2, 1);
    cout << m1.to_str() << endl;
    m1.resize(1, 4);
    cout << m1.to_str() << endl;

    Matrix m2(2, 2);
    Matrix m3(3, 3);
    m2 = m3;
    m2.set_description("m2");
    cout << m2.to_str() << endl;
}

// operators visualisation
void visualisation_operator()
{
    cout << "visualisation_operator" << endl;
    Matrix m1(3, 3);
    m1.fill(1);
    m1.set_description("m1");
    cout << m1.to_str() << endl;
    Matrix m2(3, 3);

    // m2.fill(2);
    m2.set(1, 1, 3);
    m2.set_description("m2");
    cout << m2.to_str() << endl;

    Matrix m3(3, 3);
    m3 = m1 + m2;
    m3.set_description("m3 = m1+m2");
    cout << m3.to_str() << endl;

    m3 = m1 - m2;
    m3.set_description("m3 = m1-m2");
    cout << m3.to_str() << endl;

    m3 = m1 * m2;
    m3.set_description("m3 = m1*m2");
    cout << m3.to_str() << endl;

    m3 = m1 * 2;
    m3.set_description("m3 = m1*2");
    cout << m3.to_str() << endl;

    m3 = m1 * 1.5 + m2;
    m3.set_description("m3 = 1.5*m1+m2");
    cout << m3.to_str() << endl;

    m3 = 2 * m1;
    m3.set_description("m3 = 2*m1");
    cout << m3.to_str() << endl;

    m1.resize(3, 2);
    m2.resize(2, 3);
    m3.resize(1, 1);
    m3 = m1 * m2;

    m3.set_description("m3 = m1*m2");
    cout << m3.to_str() << endl;

    // access operator
    cout << "m3(0,0) = ";
    cout << m3(0, 0) << endl
         << endl;
}

void visualisation_function()
{
    cout << "visualisation_function" << endl;
    Matrix m1(3, 3);
    m1.fill(1);
    m1.set(0, 1, 3);
    m1.set(1, 0, 3);
    m1.set(1, 1, 2);
    m1.set_description("m1");
    cout << m1.to_str() << endl;

    Matrix m2 = m1.transpose();
    m2.set_description("m1.transpose()");
    cout << m2.to_str() << endl;

    double det = m1.determinant();
    cout << "m1.determinant()=" << det << endl;

    Matrix m3 = m1.inverse();
    m3.set_description("m3 = m1.inverse()");
    cout << m3.to_str() << endl;

    double trace = m1.trace();
    cout << "m1.trace()=" << trace << endl;

    Matrix m4 = m1 * IdentityMatrix(m1.get_nb_cols());
    m4.set_description("m4 = m1*IdentityMatridx(m1.get_nb_cols())");
    cout << m4.to_str() << endl;
}

bool show(string name, const bool res)
{
    
    if (!res)
    {
        cout << name << " : ";
        cout << "wrong";
        cout << endl;
    }
    
    return res;
}

bool unitary_test_CTOR_DTOR()
{
    bool ret=true;

    if(!show("default Matrix equal Matrix 0 by 0" ,Matrix()==Matrix(0,0))) ret=false;
    if(!show("default Matrix not different to Matrix 0 by 0" ,!(Matrix()!=Matrix(0,0)))) ret=false;


    return ret;
}

int main()
{
    // visualisation
    /*
    visualisation_CTOR_DTOR();
    visualisation_resize();
    visualisation_operator();
    visualisation_function();
    */

    if(unitary_test_CTOR_DTOR()) cout<<"all test_CTOR_DTOR passed"<<endl;

    return 0;
}