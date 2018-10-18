#include <iostream>

std::ostream& operator<<(std::ostream &out, std::string str)
{
    printf("init \n");
    printf(str.c_str());
    printf("final \n");
    return out;
}

std::ostream& operator<<(std::ostream &out, const char* str)
{
    printf("init \n");
    printf(str);
    printf("final \n");
    return out;
}

int main()
{
    std::cout << std::string("xax \n");
    std::cout << "Hello \n";
    return 0;
}
