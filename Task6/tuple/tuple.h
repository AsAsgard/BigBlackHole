#ifndef TUPLE_H
#define TUPLE_H

#include <cstddef>

namespace NewTuple {

    template<std::size_t N, typename... Types>
    class tupleImp;

    template<size_t N, typename FirstType>
    class tupleValue {
        template<size_t Number, typename FirstType1, typename... Types1> friend FirstType1& Get(tupleImp<Number, FirstType1, Types1...> &t);
    protected:
        FirstType Value;
    };

    template<std::size_t N, typename FirstType, typename... Types>
    class tupleImp<N, FirstType, Types...> : public tupleImp<N+1,Types...>, public tupleValue<N, FirstType> {};

    template<size_t N, typename FirstType>
    class tupleImp<N, FirstType> : public tupleValue<N, FirstType> { };

    template<> class tupleImp<0> {};


    template<size_t Number, typename FirstType, typename... Types>
    FirstType& Get(tupleImp<Number, FirstType, Types...> &t)
    {
        return t.tupleValue<Number, FirstType>::Value;
    }

    template<typename... Types>
    using tuple = tupleImp<0,Types...>;
}

#endif // TUPLE_H
