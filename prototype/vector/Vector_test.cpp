#include <iostream>
#include "Vector.cpp"

using namespace std;

void test_CTOR_DTOR()
{

}

void test_resize()
{

}

// operators test
void test_operator()
{

}

void test_function()
{
Vector v1=Vector(3);
Vector v2 =Vector(3);
v1.set(0,1);
v1.set(1,2);
v1.set(2,3);
v2.set(0,1);
v2.set(1,1);
v2.set(2,1);

cout<<"v1="<<v1.to_str()<<endl;
cout<<"dot(v1,v2)="<<dot(v1,v2)<<endl;

Vector v3 = cross(v1,v2);
cout<<"v3="<<v3.to_str()<<endl;
}
int main()
{
    test_CTOR_DTOR();
    test_resize();
    test_operator();
    test_function();
    return 0;
}