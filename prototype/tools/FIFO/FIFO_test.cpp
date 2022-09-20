/*
cree par : robinAZERTY
version du : 20/09/2022
*/

#include "FIFO.cpp"

#include <iostream>
using namespace std;

void visu()
{

    FIFO_double fifo(10);
    //remplir la fifo
    for (int i = 0; i < 10; i++)
    {
        fifo.push(i);
    }

    //afficher la fifo
    for (int i = 0; i > -10; i--)
    {
        cout<<"fifo["<<i<<"] = "<<fifo[i]<<endl;
    }

    //add a new element
    cout<<"add a new element"<<endl;
    fifo.push(10);

    for(int i = 0; i > -10; i--)
    {
        cout<<"fifo["<<i<<"]  ="<<fifo[i]<<endl;
    }
    

}

int main ()
{   
    visu();
    return 0;
}