#ifndef PRODUCT_H
#define PRODUCT_H


class Product
{
public:
    Product();
    ~Product();
    void ProducePart();

protected:

private:

};

class ProductPart
{
public:
    ProductPart();
    ~ProductPart();
    ProductPart* BuildPart();

protected:

private:
};

#endif // PRODUCT_H
