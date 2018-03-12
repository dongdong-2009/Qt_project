#include "builder.h"
#include "product.h"
#include "director.h"

int main(int argc, char *argv[])
{
    ConcreteBuilder* cb = new ConcreteBuilder();
    Director* d = new Director(cb);
    d->Construct();

    delete d;
    d = 0;
    delete cb;
    cb = 0;

    return 0;
}
/*
 * Builder 模式的关键是其中的 Director 对象并不直接返回对象,而是通过一步步
(BuildPartA,BuildPartB,BuildPartC)来一步步进行对象的创建。当然这里 Director 可以
提供一个默认的返回对象的接口(即返回通用的复杂对象的创建,即不指定或者特定唯一指
定 BuildPart 中的参数)

代码说明
Builder 模式的示例代码中,BuildPart 的参数是通过客户程序员传入的,这里为了简单
说明问题,使用“user-defined”代替,实际的可能是在 Construct 方法中传入这 3 个参数,
这样就可以得到不同的细微差别的复杂对象了。

讨论
GoF 在《设计模式》一书中给出的关于 Builder 模式的意图是非常容易理解、间接的:
将一个复杂对象的构建与它的表示分离,使得同样的构建过程可以创建不同的表示(在示例
代码中可以通过传入不同的参数实现这一点)。
Builder 模式和 AbstractFactory 模式在功能上很相似,因为都是用来创建大的复杂的对
象,它们的区别是:Builder 模式强调的是一步步创建对象,并通过相同的创建过程可以获
得不同的结果对象,一般来说 Builder 模式中对象不是直接返回的。而在 AbstractFactory 模
式中对象是直接返回的,AbstractFactory 模式强调的是为创建多个相互依赖的对象提供一个
同一的接口。
*/