#include "State.cpp"
#include <iostream>
using namespace std;

void visualisation_CTOR_DTOR()
{
    cout << "visualisation_CTOR_DTOR" << endl;
    State state;
    cout << state.to_str() << endl;
}

int main()
{
    visualisation_CTOR_DTOR();
    return 0;
}