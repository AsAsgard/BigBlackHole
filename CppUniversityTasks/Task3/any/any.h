#ifndef ANY_H
#define ANY_H

#include <memory>
#include <typeindex>

namespace NewAny {
    class any
    {
    public:
        any() {pvalue = 0;}
        ~any() {}
        template<typename T> any(const T &newObject);
        template<typename T> any& operator=(const T &newObject);
        template<typename T> T any_cast() const;
        const std::type_info& type() const;
    private:
        class base
        {
        public:
            virtual const std::type_info& type() const = 0;
            virtual ~base() {}
        };
        template<typename T>
        class data : public base
        {
        public:
            T value;
            data() : value() {}
            ~data() override {}
            data(const T &newValue) : value(newValue) {}
            const std::type_info& type() const override { return typeid(T);}
        };
        std::shared_ptr<base> pvalue;
    };
}

#include "any.hpp"

#endif // ANY_H
