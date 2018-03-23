#include "abstractfactory.h"
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
    AbstractFactory* cf1 = new ConcreteFactory1();
    AbstractProductA* aba =  cf1->CreateProductA();
    AbstractProductB* abb =  cf1->CreateProductB();

    AbstractFactory* cf2 = new ConcreteFactory2();
    AbstractProductA* aba2 =  cf2->CreateProductA();
    AbstractProductB* abb2 = cf2->CreateProductB();

    delete cf1;
    cf1 = 0;
    delete cf2;
    cf2 = 0;

    delete aba;
    aba = 0;
    delete abb;
    abb = 0;

    delete aba2;
    aba2 = 0;
    delete abb2;
    abb2 = 0;

    return 0;
}
