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
    return 0;
}
