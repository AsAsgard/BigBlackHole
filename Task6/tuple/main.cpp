#include <iostream>
#include "tuple.h"

using namespace NewTuple;

int main()
{
    tuple<int, double, std::string> t;
    Get<0>(t) = 23;
    Get<1>(t) = 154.32;
    Get<2>(t) = "Test";
    std::cout << Get<0>(t) << std::endl;
    std::cout << Get<1>(t) << std::endl;
    std::cout << Get<2>(t) << std::endl;
    return 0;
}
