#include "product.h"
#include <iostream>
using namespace std;

AbstractProductA::~AbstractProductA()
{

}

AbstractProductA::AbstractProductA()
{

}

AbstractProductB::~AbstractProductB()
{

}

AbstractProductB::AbstractProductB()
{

}

ProductA1::ProductA1()
{
    cout<<"ProductA1"<<"()..."<<endl;
    cout<<__FUNCTION__<<"()..."<<endl;
}

ProductA1::~ProductA1()
{

}

ProductA2::ProductA2()
{
    cout<<"ProductA2"<<"()..."<<endl;
    cout<<__FUNCTION__<<"()..."<<endl;
}

ProductA2::~ProductA2()
{

}

ProductB1::ProductB1()
{
    cout<<"ProductB1"<<"()..."<<endl;
    cout<<__FUNCTION__<<"()..."<<endl;
}

ProductB1::~ProductB1()
{

}

ProductB2::ProductB2()
{
    cout<<"ProductB2"<<"()..."<<endl;
    cout<<__FUNCTION__<<"()..."<<endl;
}

ProductB2::~ProductB2()
{

}
