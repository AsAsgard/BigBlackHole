#include "print_tuple.h"
#include <string>
#include <iostream>

using namespace std;

int main()
{
    auto t1 = make_tuple(10, "Test", 3.14);
    int k = 43;
    double z = 6.55;
    string y = "Hello";
    auto t2 = tuple_cat(t1, tie(k,z,y));
    cout << t2 << std::endl;
    cout << make_tuple(5) << endl;
    cout << make_tuple() << endl;
    cout << make_tuple("xxx", 332, 2.333, 2323.43, std::string("FFFFF")) << endl;
}
