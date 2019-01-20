#pragma once

#include <type_traits>


namespace useful
{
////////////////////////////////////////////////////////////////////////////////
// negates a unary metapredicate
template <template <class> class UnaryPredicate>
struct negate
{
    template <class T>
    struct negative_predicate
    {
        static const bool value = !UnaryPredicate<T>::value;
    };

    template <class T>
    using type = negative_predicate<T>;
};


////////////////////////////////////////////////////////////////////////////////
// strip a type down to its pure base type
template <class T>
struct base_
{
    typedef T type;
};

template <class T>
struct base_<T*> : base_<T>
{
};

template <class T>
struct base_<T* const> : base_<T>
{
};

template <class T>
struct base_<T* volatile> : base_<T>
{
};

template <class T>
struct base_<T* const volatile> : base_<T>
{
};

template <class T>
struct base_<T&> : base_<T>
{
};

template <class T>
struct base_<T&&> : base_<T>
{
};

template <class T>
using base = std::remove_cv<typename base_<T>::type>;

template <class T>
using base_t = typename base<T>::type;
} // namespace useful
