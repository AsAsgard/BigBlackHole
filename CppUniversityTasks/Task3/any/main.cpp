#include "any.h"
#include <iostream>

using namespace NewAny;

int main()
{
    any b;
    std::cout << b.type().name() << std::endl;
    any a = 4;
    std::cout << a.type().name() << " : " << any_cast<int>(a) << std::endl;
    a = 3.14;
    std::cout << a.type().name() << " : " << any_cast<double>(a) << std::endl;
    a = std::string("455");
    std::cout << a.type().name() << " : " << any_cast<std::string>(a) << std::endl;
    a = std::shared_ptr<int>(new int(56));
    std::cout << a.type().name() << " : " << *any_cast<std::shared_ptr<int>>(a) << std::endl;
    a = std::shared_ptr<double>(new double(134.55));
    std::cout << a.type().name() << " : " << *any_cast<std::shared_ptr<double>>(a) << std::endl;
    std::weak_ptr<double> x = any_cast<std::shared_ptr<double>>(a);
    a = x;
    if (any_cast<std::weak_ptr<double>>(a).expired()) { std::cout << a.type().name() << " : expired" << std::endl;}
    else { std::cout << a.type().name() << " : "<< *any_cast<std::weak_ptr<double>>(a).lock() << std::endl;}
    return 0;
}
