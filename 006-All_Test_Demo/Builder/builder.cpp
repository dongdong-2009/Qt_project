#include "builder.h"
#include "product.h"
#include <iostream>
using namespace std;

Builder::~Builder()
{

}

Builder::Builder()
{

}

ConcreteBuilder::ConcreteBuilder()
{

}

ConcreteBuilder::~ConcreteBuilder()
{

}

void ConcreteBuilder::BuildPartA(const string &buildPara)
{
    cout<<"Step1:BuildParaA..."<<buildPara<<endl;
}

void ConcreteBuilder::BuildPartB(const string &buildPara)
{
    cout<<"Step2:BuildPartB..."<<buildPara<<endl;
}

void ConcreteBuilder::BuildPartC(const string &buildPara)
{
    cout<<"Step3:BuildPartC..."<<buildPara<<endl;
}

Product *ConcreteBuilder::GetProduct()
{
    BuildPartA("pre-defined");
    BuildPartB("pre-defined");
    BuildPartC("pre-defined");
    return new Product();
}
