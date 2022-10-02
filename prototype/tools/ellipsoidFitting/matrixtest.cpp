//fixing bug with inverse of matrix

#include "../../..\common\math\vector\Vector.cpp"
#include <iostream>
using namespace std;

int main()
{   
    Matrix A(3,3);
    A.set(0,0,3);
    A.set(0,1,0);
    A.set(0,2,2);
    A.set(1,0,2);
    A.set(1,1,0);
    A.set(1,2,-2);
    A.set(2,0,0);
    A.set(2,1,1);
    A.set(2,2,1);

    cout << A.to_str() << endl;
    cout<<A.inverse().to_str()<<endl;
    cout<<A.transpose().to_str()<<endl;
    cout<<A.determinant()<<endl;
    return 0;
}