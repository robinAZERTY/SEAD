//fixing bug with inverse of matrix

#include "../../..\common\math\vector\Vector.cpp"
#include <iostream>
using namespace std;

int main()
{   
    Matrix A=IdentityMatrix(3);

    cout << A.to_str() << endl;
    cout<<A.inverse().to_str()<<endl;
    cout<<A.transpose().to_str()<<endl;
    cout<<A.determinant()<<endl;
    return 0;
}