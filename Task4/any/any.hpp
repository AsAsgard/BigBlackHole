#include "any.h"

using namespace NewAny;

template<typename T>
any::any(const T &newObject)
{
    pvalue.reset(new data<T>(newObject));
}

template<typename T>
any& any::operator=(const T &newObject)
{
    pvalue.reset(new data<T>(newObject));
    return *this;
}

const std::type_info& any::type() const
{
    if (pvalue)
    {
        return pvalue->type();
    } else {
        return typeid(nullptr);
    }
}

template<typename T> T& any::any_cast() const
{
    if (typeid (T) == type() && pvalue)
    {
        return dynamic_cast<data<T>*>(pvalue.get())->value;
    } else {
        throw std::bad_cast();
    }
}

template<typename T> T& any_cast(const any &a)
{
    return a.any_cast<T>();
}
