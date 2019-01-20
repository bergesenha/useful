#pragma once

#include <type_traits>
#include <utility>

#include <type_list.hpp>
#include <void_t.hpp>

namespace useful
{
////////////////////////////////////////////////////////////////////////////////
// returns true if specialization is defined for a class Target with IntType N,
// where Target is a class taking one Integral non-type template parameter
template <class IntType,
          template <IntType>
          class Target,
          IntType N,
          class = void>
struct integral_specialization_defined : std::false_type
{
};

template <class IntType, template <IntType> class Target, IntType N>
struct integral_specialization_defined<IntType,
                                       Target,
                                       N,
                                       void_t<decltype(Target<N>())>>
    : std::true_type
{
};

template <class IntType, template <IntType> class Target, IntType N>
constexpr bool integral_specialization_defined_v =
    integral_specialization_defined<IntType, Target, N>::value;


////////////////////////////////////////////////////////////////////////////////
// returns true if specialization Target<T> is defined, otherwise returns false
template <class, template <class...> class Target, class... Args>
struct specialization_defined_impl : std::false_type
{
};

template <template <class...> class Target, class... Args>
struct specialization_defined_impl<void_t<decltype(Target<Args...>())>,
                                   Target,
                                   Args...> : std::true_type
{
};

template <template <class...> class Target, class... Args>
using specialization_defined =
    specialization_defined_impl<void, Target, Args...>;

template <template <class...> class Target, class... Args>
constexpr bool specialization_defined_v =
    specialization_defined<Target, Args...>::value;
} // namespace useful
