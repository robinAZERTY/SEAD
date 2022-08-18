#include <iostream>
#include "Vector.cpp"

using namespace std;

void test_CTOR_DTOR()
{
    cout << "test_CTOR_DTOR" << endl;
    Vector m;
    cout << m.to_str() << endl;
    Vector m1(2);
    m1.set_description("m1");
    cout << m1.to_str() << endl;
    Vector m2(m1);
    cout << m2.to_str() << endl;
    Vector m3;
    m1.set(0, 1);
    m3 = m1;
    cout << m3.to_str() << endl;
    Vector m4(2);
    cout << m4.to_str() << endl;
}

void test_resize()
{

    cout << "test_resize" << endl;
    Vector m1(2);
    m1.set(1, 1);
    m1.set_description("m1");
    cout << m1.to_str() << endl;
    m1.resize(3);
    m1.set(2, 1);
    cout << m1.to_str() << endl;
    m1.resize(4);
    cout << m1.to_str() << endl;

    Vector m2(2);
    Vector m3(3);
    m2 = m3;
    m2.set_description("m2");
    cout << m2.to_str() << endl;
}

// operators test
void test_operator()
{
    cout << "test_operator" << endl;
    Vector v1(2);
    Matrix m1(2, 2);
    v1.set(0, 1);
    v1.set(1, 2);
    v1.set_description("v1");
    m1.set(1, 1, 1);
    m1.set(0, 0, 2);
    m1.set_description("m1");
    cout << "Matrix(2*2) * Vector(2) " << endl;
    cout << (m1 * v1).to_str() << endl;

    cout << "Vector(2) * Matrix(2*2) " << endl;
    cout << (v1 * m1).to_str() << endl;
}

void test_function()
{
    cout << "test_function" << endl;
    Vector v1 = Vector(3);
    Vector v2 = Vector(3);
    v1.set(0, 1);
    v1.set(1, 2);
    v1.set(2, 3);
    v2.set(0, 1);
    v2.set(1, 1);
    v2.set(2, 1);

    cout << "v1=" << v1.to_str() << endl;
    cout << "dot(v1,v2)=" << dot(v1, v2) << endl;

    Vector v3 = cross(v1, v2);
    cout << "v3=" << v3.to_str() << endl;
}
int main()
{
    test_CTOR_DTOR();
    test_resize();
    test_operator();
    test_function();
    return 0;
}