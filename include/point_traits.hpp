#pragma once

#include <cstddef>
#include <type_traits>

namespace useful
{

template <class PointType, class, class, class>
struct point_traits_;


template <class PointType>
using point_traits = point_traits_<PointType, void, void, void>;


template <class T, std::size_t N>
struct point_traits_<T[N], void, void, void>
{
    static const std::size_t dimensions = N;

    template <std::size_t U>
    using value_type = T;

    template <std::size_t U>
    static value_type<U>& get(T (&pt)[N])
    {
        return pt[U];
    }
};


template <class PointType, class Y, class Z>
struct point_traits_<PointType,
                     std::void_t<decltype(std::declval<PointType>().x)>,
                     Y,
                     Z>
{
    static const std::size_t dimensions = 1;
};


template <class PointType, class Z>
struct point_traits_<PointType,
                     std::void_t<decltype(std::declval<PointType>().x)>,
                     std::void_t<decltype(std::declval<PointType>().y)>,
                     Z>
{
    static const std::size_t dimensions = 2;
};


template <class PointType>
struct point_traits_<PointType,
                     std::void_t<decltype(std::declval<PointType>().x)>,
                     std::void_t<decltype(std::declval<PointType>().y)>,
                     std::void_t<decltype(std::declval<PointType>().z)>>
{
    static const std::size_t dimensions = 3;
};

} // namespace useful
