#ifndef PRINT_TUPLE_H
#define PRINT_TUPLE_H

#include <iostream>
#include <sstream>
#include <tuple>

class TPrinterPrivate{
    std::stringstream ss;
    template<std::size_t N, typename... Types> friend void print_tuple(const std::tuple<Types...> &t);
    template<typename... Types> friend std::ostream& operator<<(std::ostream& out, const std::tuple<Types...>& t);
public:
    TPrinterPrivate() : ss("") {}
    template<typename... Types> TPrinterPrivate(Types... Args) = delete;
};

static TPrinterPrivate pr;

template<std::size_t N, typename... Types>
void print_tuple(const std::tuple<Types...> &t)
{
    if constexpr (N > 1)
    {
        print_tuple<N-1, Types...>(t);
        pr.ss << ", " << std::get<N-1>(t);
    } else if constexpr (N == 1) {
        pr.ss << std::get<0>(t);
    }
}

template<typename... Types>
std::ostream& operator<<(std::ostream& out, const std::tuple<Types...>& t)
{
    pr.ss.str("");
    print_tuple<sizeof...(Types), Types...>(t);
    out << pr.ss.str();
    return out;
}

#endif // PRINT_TUPLE_H
