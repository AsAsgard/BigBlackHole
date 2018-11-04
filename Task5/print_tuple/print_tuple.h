#ifndef PRINT_TUPLE_H
#define PRINT_TUPLE_H

#include <iostream>
#include <sstream>
#include <tuple>

class TPrinterPrivate{
    std::stringstream ss;
    template<std::size_t N, typename... Types> friend class TPrinter;
    template<typename... Types> friend std::ostream& operator<<(std::ostream& out, const std::tuple<Types...>& t);
public:
    TPrinterPrivate() : ss("") {}
    template<typename... Types> TPrinterPrivate(Types... Args) = delete;
};

static TPrinterPrivate pr;


template<std::size_t N, typename... Types>
class TPrinter {
public:
    static /*std::stringstream& */ void print_tuple(/*std::stringstream& ss, */const std::tuple<Types...> &t)
    {
//        ss << TPrinter<N-1, Types...>::print_tuple(ss,t).str() << ", " << std::get<N-1>(t);
        TPrinter<N-1, Types...>::print_tuple(t);
        pr.ss << ", " <<std::get<N-1>(t);
    }
};

template<typename... Types>
class TPrinter<1, Types...>
{
public:
    static /*std::stringstream&*/void print_tuple(/*std::stringstream& ss, */const std::tuple<Types...> &t)
    {
        pr.ss << std::get<0>(t);
//        return ss;
    }
};

template<typename... Types>
class TPrinter<0, Types...>
{
public:
    static /*std::stringstream&*/void print_tuple(/*std::stringstream& ss, */const std::tuple<Types...> &t)
    {
//        return ss;
    }
};

template<typename... Types>
std::ostream& operator<<(std::ostream& out, const std::tuple<Types...>& t)
{
    pr.ss.str("");
    TPrinter<sizeof...(Types), Types...>::print_tuple(t);
    out << pr.ss.str();
    return out;
}

#endif // PRINT_TUPLE_H
