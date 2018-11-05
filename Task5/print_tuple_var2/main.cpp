#include "print_tuple.h"

using namespace std;

int main()
{
    std::tuple<int, std::string, float> t1(10, "Test", 3.14);
    int k = 43;
    double z = 6.55;
    std::string y = "Hello";
    auto t2 = std::tuple_cat(t1, std::tie(k,z,y));
    cout << t2 << std::endl;
    cout << std::make_tuple(5) << std::endl;
    cout << std::make_tuple() << std::endl;
    cout << std::make_tuple("xxx", 332, 2.333, 2323.43, std::string("FFFFF")) << std::endl;
    return 0;
}
