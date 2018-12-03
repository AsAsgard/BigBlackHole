#ifndef PRINT_TUPLE_H
#define PRINT_TUPLE_H

#include <ostream>
#include <tuple>

template<std::size_t N, typename... Types>
    void print_tuple(std::ostream &out, const std::tuple<Types...> &t)
{
    if constexpr (N > 1)
    {
        print_tuple<N - 1>(out, t);
        out << ", " << std::get<N - 1>(t);
    }
    else if constexpr (N == 1)
        out << std::get<0>(t);
}

template<typename... Types>
std::ostream& operator<<(std::ostream& out, const std::tuple<Types...>& t)
{
    print_tuple<sizeof...(Types)>(out, t);
    return out;
}

#endif // PRINT_TUPLE_H
