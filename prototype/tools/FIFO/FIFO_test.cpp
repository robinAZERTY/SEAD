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
    //afficher la fifo
    cout<<"initialisation de la fifo"<<endl;
    for (int i = 0; i < 10; i++)
    {
        cout<<"fifo["<<i<<"] = "<<fifo[i]<<endl;
    }
    cout<<endl;
    //remplir la fifo
    for (int i = 1; i <= 10; i++)
    {
        fifo.push(i);
    }

    //afficher la fifo
    for (int i = 0; i < 10; i++)
    {
        cout<<"fifo["<<i<<"] = "<<fifo[i]<<endl;
    }

    //add a new element
    cout<<"push a new element"<<endl;
    fifo.push(11);
    fifo[0] += 1;

    for(unsigned short i = 0; i < 10; i++)
    {
        cout<<"fifo["<<i<<"]  ="<<fifo[i]<<"\t"<<fifo[-i]<<endl;
    }
    

}

int main ()
{   
    visu();
    return 0;
}